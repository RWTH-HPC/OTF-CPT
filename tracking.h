#ifndef TRACKING_H
#define TRACKING_H

#include <assert.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

// critical path tool only needs comm and request tracking
#define HANDLE_COMM 1
#define HANDLE_REQUEST 1
#define HANDLE_MESSAGE 1
// use memory management for tool data
#define REAL_DATAPOOL 1
#define USE_ERRHANDLER 1

#ifdef __cplusplus
#define _EXTERN_C_ extern "C"
#endif

#ifdef USE_ERRHANDLER
#define errHandlerComm(c) registerErrHandler(*c)
void createErrHandler();
void registerErrHandler(MPI_Comm comm);
#else
#define errHandlerComm(c) (void)c
#endif

#ifdef HANDLE_WIN
#define preWin(w) w = wf.getHandle(w)
#define postWin(w) *w = wf.newHandle(*w)
#else
#define preWin(w) (void)w
#define postWin(w) (void)w
#endif

#ifdef HANDLE_FILE
#define preFile(f) f = ff.getHandle(f)
#define postFile(f) *f = ff.newHandle(*f)
#else
#define preFile(f) (void)f
#define postFile(f) (void)f
#endif

#ifdef HANDLE_COMM
#define preComm(c) c = cf.getHandle(c)
#define postComm(c) *c = cf.newHandle(*c)
#else
#define preComm(c) (void)c
#define postComm(c) (void)c
#endif

#if defined(HAVE_SESSION) && defined(HANDLE_SESSION)
#define preSession(s) s = sf.getHandle(s)
#define postSession(s) *s = sf.newHandle(*s)
#else
#define preSession(s) (void)s
#define postSession(s) (void)s
#endif

#ifdef HANDLE_MESSAGE
#define preMessage(m) m = mf.getHandle(m)
#define postMessage(m) *m = mf.newHandle(*m)
#else
#define preMessage(m) (void)m
#define postMessage(m) (void)m
#endif

#ifdef HANDLE_REQUEST
#define postRequest(r) *r = rf.newRequest(*r)
#define postRequestInit(r) *r = rf.newRequest(*r, true)
#else
#define postRequest(r) (void)r
#define postRequestInit(r) (void)r
#endif

enum toolDataEnum {
  toolWinData,
  toolFileData,
  toolCommData,
  toolSessionData,
  toolMessageData,
  toolRequestData
};

#include "handle-data.h"

// Abstract interface of the HandleFactor
template <typename M, typename T> class AbstractHandleFactory {
protected:
  std::unordered_map<M, T> predefHandles{};
  virtual bool isPredefined(M handle) { return handle == T::nullHandle; }
  virtual T *findPredefinedData(M handle) {
    auto iter = predefHandles.find(handle);
    if (iter == predefHandles.end())
      return nullptr;
    return &(iter->second);
  }

public:
  virtual M newHandle(M &handle) = 0;
  virtual M newHandle(M &handle, T *data) = 0;
  virtual M freeHandle(M handle) = 0;
  virtual T *detachHandle(M handle) = 0;
  virtual T *findData(M handle) = 0;
  virtual T *newData() = 0;
  virtual M &getHandle(M &handle) = 0;
  virtual M &getHandleLocked(M &handle) = 0;
  virtual std::shared_lock<std::shared_mutex> getSharedLock() = 0;
  virtual ~AbstractHandleFactory<M, T>(){};
  virtual void initPredefined() {
    predefHandles[T::nullHandle].init(T::nullHandle);
  }
};

// Abstract interface of the RequestFactor
class AbstractRequestFactory
    : public virtual AbstractHandleFactory<MPI_Request, RequestData> {
public:
  virtual MPI_Request newRequest(MPI_Request req, bool persistent = false) = 0;
  virtual MPI_Request completeRequest(MPI_Request req, MPI_Status *status) = 0;
  virtual MPI_Request startRequest(MPI_Request req) = 0;
  virtual ~AbstractRequestFactory(){};
};

#ifdef REAL_DATAPOOL
// Memory management for ToolData
template <typename M, typename T> class DataPool {
private:
  mutable std::shared_mutex DPMutex{};
  std::vector<T *> dataPointer{};
  std::list<void *> memory;
  virtual void newDatas() {
    assert(sizeof(T) % 64 == 0);
    int ndatas = 4096 / sizeof(T);
    char *datas = (char *)malloc(ndatas * sizeof(T));
    memory.push_back(datas);
    for (int i = 0; i < ndatas; i++) {
      dataPointer.push_back(new (datas + i * sizeof(T)) T());
    }
  }

public:
  virtual T *getData() {
    std::unique_lock<std::shared_mutex> lock(DPMutex);
    if (dataPointer.empty())
      newDatas();
    T *ret = dataPointer.back();
    dataPointer.pop_back();
    return ret;
  }
  virtual void returnData(T *data) {
    std::unique_lock<std::shared_mutex> lock(DPMutex);
    dataPointer.emplace_back(data);
  }
  virtual ~DataPool() {
    for (auto i : this->dataPointer)
      if (i)
        i->~T();
    for (auto i : this->memory)
      if (i)
        free(i);
  }
};
#else
// No memory management for ToolData, just use new/delete
template <typename M, typename T> class DataPool {
public:
  virtual T *getData() { return new T(); }
  virtual void returnData(T *data) { delete data; }
};
#endif

// General template for HandleFactory / RequestFactory
// should not be instanciated
template <typename M, typename T, typename D>
class HandleFactory : public virtual AbstractHandleFactory<M, T> {
  // Instanciating the primary class should fail because of the private default
  // constructor
  HandleFactory() { /*static_assert(false, "primary template of HandleFactory
                       instanciated unexpectedly");*/
  }
};
template <typename D> class RequestFactoryInst {
  // Instanciating the primary class should fail because of the private default
  // constructor
  RequestFactoryInst() { /*static_assert(false, "primary template of
                            RequestFactory instanciated unexpectedly");*/
  }
};

// Specialized template of HandleFactory for pointer-type handles
template <typename M, typename T, typename MI>
class HandleFactory<M, T, MI *> : public virtual AbstractHandleFactory<M, T>,
                                  public DataPool<M, T> {
protected:
  mutable std::shared_mutex DummyMutex{};

public:
  T *newData() { return this->getData(); }
  M newHandle(M &handle) {
    if (this->isPredefined(handle))
      return handle;
    T *ret = this->getData();
    return newHandle(handle, ret);
  }
  M newHandle(M &handle, T *ret) {
    ret->init(handle);
    return (M)(uintptr_t)ret;
  }
  M freeHandle(M handle) {
    if (this->isPredefined(handle))
      return (M)T::nullHandle;
    ((T *)(uintptr_t)(handle))->fini();
    this->returnData((T *)(uintptr_t)(handle));
    return (M)T::nullHandle;
  }
  T *detachHandle(M handle) {
    assert (!this->isPredefined(handle));
    return (T *)(uintptr_t)(handle);
  }
  T *findData(M handle) {
    T *ret = this->findPredefinedData(handle);
    if (ret != nullptr)
      return ret;
    return (T *)(uintptr_t)(handle);
  }
  M &getHandle(M &handle) {
    if (this->isPredefined(handle))
      return handle;
    return (M &)(((T *)(uintptr_t)(handle))->handle);
  }
  M &getHandleLocked(M &handle) {
    if (this->isPredefined(handle))
      return handle;
    return (M &)(((T *)(uintptr_t)(handle))->handle);
  }
  std::shared_lock<std::shared_mutex> getSharedLock() {
    return std::shared_lock<std::shared_mutex>{DummyMutex, std::defer_lock};
  }
  virtual ~HandleFactory<M, T, MI *>() {}
};

// Specialized template of RequestFactory for pointer-type handles
template <typename MI>
class RequestFactoryInst<MI *>
    : public HandleFactory<MPI_Request, RequestData, MI *>,
      public AbstractRequestFactory {
  template <typename D>
  MPI_Request _newRequest(MPI_Request req, D &reqData, bool persistent) {
    if (req == MPI_REQUEST_NULL)
      return MPI_REQUEST_NULL;
    RequestData *ret = this->getData();
    ret->init(req, reqData, persistent);
    return (MPI_Request)(uintptr_t)ret;
  }

public:
  // get data from the pool
  MPI_Request newRequest(MPI_Request req, bool persistent) {
    const void *reqData = nullptr;
    return _newRequest(req, reqData, persistent);
  }

  MPI_Request completeRequest(MPI_Request req, MPI_Status *status) {
    if (req == MPI_REQUEST_NULL)
      return MPI_REQUEST_NULL;
    if (((RequestData *)(uintptr_t)(req))->isPersistent()) {
      ((RequestData *)(uintptr_t)(req))->complete(status);
      return req;
    }
    ((RequestData *)(uintptr_t)(req))->fini(status);
    this->returnData((RequestData *)(uintptr_t)(req));
    return MPI_REQUEST_NULL;
  }
  MPI_Request startRequest(MPI_Request req) {
    if (req == MPI_REQUEST_NULL)
      return MPI_REQUEST_NULL;
    RequestData *ret = ((RequestData *)(uintptr_t)(req));
    assert(ret->isPersistent());
    ret->start();
    return ret->handle;
  }
};

// Specialized template of HandleFactory for int-type handles
template <typename M, typename T>
class HandleFactory<M, T, int> : public virtual AbstractHandleFactory<M, T>,
                                 public DataPool<M, T> {
protected:
  mutable std::shared_mutex DTMutex{};
  mutable std::shared_mutex AHMutex{};
  std::vector<T *> dataTable{};
  std::vector<M> availableHandles{};

  void newAH() {
    int ndatas = 4096 / 64;
    size_t oldSize = dataTable.size(), newSize = oldSize + ndatas;
    {
      // Only the resize actually modifies the vector and
      // can lead to a reallocation of the elements
      std::unique_lock<std::shared_mutex> lock(DTMutex);
      dataTable.resize(newSize);
    }
    // Getting the lock here is not necessary:
    // unique_lock can only be taken while having unique AHMutex
    // std::shared_lock<std::shared_mutex> lock(DTMutex);
    for (size_t i = oldSize, j = 0; i < newSize; i++, j++) {
      dataTable[i] = nullptr;
      availableHandles.emplace_back((M)(i));
    }
  }

public:
  T *newData() { return this->getData(); }
  M newHandle(M &handle) {
    if (this->isPredefined(handle))
      return handle;
    T *ret = this->getData();
    return newHandle(handle, ret);
  }
  M newHandle(M &handle, T *dp) {
    M ret;
    {
      std::unique_lock<std::shared_mutex> lock(AHMutex);
      if (availableHandles.empty())
        newAH();
      ret = availableHandles.back();
      availableHandles.pop_back();
      // Getting the lock here is not necessary:
      // unique_lock can only be taken while having unique AHMutex
      // std::shared_lock<std::shared_mutex> lock(DTMutex);
      dataTable[(size_t)(ret)] = dp;
    }
    dp->init(handle);
    return ret;
  }

  M freeHandle(M handle) {
    if (this->isPredefined(handle))
      return T::nullHandle;
    T *ret;
    {
      std::unique_lock<std::shared_mutex> lock(AHMutex);
      ret = dataTable[(size_t)(handle)];
      availableHandles.emplace_back(handle);
    }
    ret->fini();
    this->returnData(ret);
    return T::nullHandle;
  }
  T *detachHandle(M handle) {
    assert (!this->isPredefined(handle));
    T *ret;
    {
      std::unique_lock<std::shared_mutex> lock(AHMutex);
      ret = dataTable[(size_t)(handle)];
      availableHandles.emplace_back(handle);
    }
    return ret;
  }
  T *findData(M handle) {
    T *ret = this->findPredefinedData(handle);
    if (ret != nullptr)
      return ret;
    std::shared_lock<std::shared_mutex> lock(DTMutex);
    return dataTable[(size_t)(handle)];
  }
  M &getHandle(M &handle) {
    if (this->isPredefined(handle))
      return handle;
    std::shared_lock<std::shared_mutex> lock(DTMutex);
    return dataTable[(size_t)(handle)]->handle;
  }
  M &getHandleLocked(M &handle) {
    if (this->isPredefined(handle))
      return handle;
    return dataTable[(size_t)(handle)]->handle;
  }
  std::shared_lock<std::shared_mutex> getSharedLock() {
    return std::shared_lock<std::shared_mutex>{DTMutex};
  }
  virtual ~HandleFactory<M, T, int>() {}
};

// Specialized template of RequestFactory for int-type handles
template <>
class RequestFactoryInst<int>
    : public HandleFactory<MPI_Request, RequestData, int>,
      public AbstractRequestFactory {
  template <typename D>
  MPI_Request _newRequest(MPI_Request req, D &reqData, bool persistent) {
    if (req == MPI_REQUEST_NULL)
      return MPI_REQUEST_NULL;
    MPI_Request ret;
    RequestData *dp = this->getData();
    {
      std::unique_lock<std::shared_mutex> lock(AHMutex);
      if (availableHandles.empty())
        newAH();
      ret = availableHandles.back();
      availableHandles.pop_back();
      dataTable[(size_t)(ret)] = dp;
    }
    dp->init(req, reqData, persistent);
    return ret;
  }

public:
  // get data from the pool
  MPI_Request newRequest(MPI_Request req, bool persistent, RequestData *data) {
    if (req == MPI_REQUEST_NULL)
      return MPI_REQUEST_NULL;
    MPI_Request ret;
    std::unique_lock<std::shared_mutex> lock(AHMutex);
    if (availableHandles.empty())
      newAH();
    ret = availableHandles.back();
    availableHandles.pop_back();
    dataTable[(size_t)(ret)] = data;
    return ret;
  }
  MPI_Request newRequest(MPI_Request req, bool persistent) {
    const void *reqData = nullptr;
    return _newRequest(req, reqData, persistent);
  }

  // returning to the datapool using lock
  MPI_Request completeRequest(MPI_Request req, MPI_Status *status) {
    if (req == MPI_REQUEST_NULL)
      return MPI_REQUEST_NULL;
    RequestData *ret;
    bool persistent{false};
    {
      std::unique_lock<std::shared_mutex> lock(AHMutex);
      ret = dataTable[(size_t)(req)];
      persistent = ret->isPersistent();
      if (!persistent) {
        availableHandles.emplace_back(req);
      }
    }
    if (persistent) {
      ret->complete(status);
      return req;
    }
    ret->fini(status);
    this->returnData(ret);
    return MPI_REQUEST_NULL;
  }
  MPI_Request startRequest(MPI_Request req) {
    if (req == MPI_REQUEST_NULL)
      return MPI_REQUEST_NULL;
    RequestData *ret;
    {
      std::shared_lock<std::shared_mutex> lock(DTMutex);
      ret = dataTable[(size_t)(req)];
    }
    assert(ret->isPersistent());
    ret->start();
    return ret->handle;
  }
};

using WinData = HandleData<MPI_Win, toolWinData>;
using FileData = HandleData<MPI_File, toolFileData>;
using MessageData = RequestData;
#ifdef HAVE_SESSION
using SessionData = HandleData<MPI_Session, toolSessionData>;
#endif

using WinFactory = HandleFactory<MPI_Win, WinData, MPI_Win>;

using FileFactory = HandleFactory<MPI_File, FileData, MPI_File>;

using CommFactory = HandleFactory<MPI_Comm, CommData, MPI_Comm>;

#ifdef HAVE_SESSION
using SessionFactory = HandleFactory<MPI_Session, SessionData, MPI_Session>;
#endif

using MessageFactory = HandleFactory<MPI_Message, MessageData, MPI_Message>;

using RequestFactory = RequestFactoryInst<MPI_Request>;

#ifdef HANDLE_WIN
extern WinFactory wf;
#endif
#ifdef HANDLE_FILE
extern FileFactory ff;
#endif
#ifdef HANDLE_COMM
template <>
bool AbstractHandleFactory<MPI_Comm, CommData>::isPredefined(MPI_Comm handle);

template <> void AbstractHandleFactory<MPI_Comm, CommData>::initPredefined();

extern CommFactory cf;
#endif
#ifdef HANDLE_MESSAGE
template <>
bool AbstractHandleFactory<MPI_Message, MessageData>::isPredefined(MPI_Message handle);

template <> void AbstractHandleFactory<MPI_Message, MessageData>::initPredefined();

extern MessageFactory mf;
#endif
#ifdef HANDLE_REQUEST
extern RequestFactory rf;
#endif
#if defined(HAVE_SESSION) && defined(HANDLE_SESSION)
extern SessionFactory sf;
#elif !defined(HAVE_SESSION)
typedef int MPI_Session;
#endif

#endif
