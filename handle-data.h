#include <cassert>
#include <functional>
#include <mpi.h>
#include <stdlib.h>

enum nbFunction {
  nbf_unknown = -1,
  nbf_MPI_Isend,
  nbf_MPI_Issend,
  nbf_MPI_Irsend,
  nbf_MPI_Ibsend,
  nbf_MPI_Irecv,

  nbf_MPI_Send_init,
  nbf_MPI_Ssend_init,
  nbf_MPI_Rsend_init,
  nbf_MPI_Bsend_init,
  nbf_MPI_Recv_init,

  nbf_MPI_Iallgather,
  nbf_MPI_Iallgatherv,
  nbf_MPI_Iallreduce,
  nbf_MPI_Ialltoall,
  nbf_MPI_Ialltoallv,
  nbf_MPI_Ialltoallw,
  nbf_MPI_Ibarrier,
  nbf_MPI_Ibcast,
  nbf_MPI_Iexscan,
  nbf_MPI_Igather,
  nbf_MPI_Igatherv,
  nbf_MPI_Ireduce,
  nbf_MPI_Ireduce_scatter,
  nbf_MPI_Ireduce_scatter_block,
  nbf_MPI_Iscan,
  nbf_MPI_Iscatter,
  nbf_MPI_Iscatterv,

  nbf_MPI_Allgather_init,
  nbf_MPI_Allgatherv_init,
  nbf_MPI_Allreduce_init,
  nbf_MPI_Alltoall_init,
  nbf_MPI_Alltoallv_init,
  nbf_MPI_Alltoallw_init,
  nbf_MPI_Barrier_init,
  nbf_MPI_Bcast_init,
  nbf_MPI_Exscan_init,
  nbf_MPI_Gather_init,
  nbf_MPI_Gatherv_init,
  nbf_MPI_Reduce_init,
  nbf_MPI_Reduce_scatter_init,
  nbf_MPI_Reduce_scatter_block_init,
  nbf_MPI_Scan_init,
  nbf_MPI_Scatter_init,
  nbf_MPI_Scatterv_init,

  nbf_MPI_Neighbor_iallgather,
  nbf_MPI_Neighbor_iallgatherv,
  nbf_MPI_Neighbor_iallreduce,
  nbf_MPI_Neighbor_ialltoall,
  nbf_MPI_Neighbor_ialltoallv,
  nbf_MPI_Neighbor_ialltoallw,
  nbf_MPI_Neighbor_ibarrier,
  nbf_MPI_Neighbor_ibcast,
  nbf_MPI_Neighbor_iexscan,
  nbf_MPI_Neighbor_igather,
  nbf_MPI_Neighbor_igatherv,
  nbf_MPI_Neighbor_ireduce,
  nbf_MPI_Neighbor_ireduce_scatter,
  nbf_MPI_Neighbor_ireduce_scatter_block,
  nbf_MPI_Neighbor_iscan,
  nbf_MPI_Neighbor_iscatter,
  nbf_MPI_Neighbor_iscatterv,

  nbf_MPI_Neighbor_allgather_init,
  nbf_MPI_Neighbor_allgatherv_init,
  nbf_MPI_Neighbor_allreduce_init,
  nbf_MPI_Neighbor_alltoall_init,
  nbf_MPI_Neighbor_alltoallv_init,
  nbf_MPI_Neighbor_alltoallw_init,
  nbf_MPI_Neighbor_barrier_init,
  nbf_MPI_Neighbor_bcast_init,
  nbf_MPI_Neighbor_exscan_init,
  nbf_MPI_Neighbor_gather_init,
  nbf_MPI_Neighbor_gatherv_init,
  nbf_MPI_Neighbor_reduce_init,
  nbf_MPI_Neighbor_reduce_scatter_init,
  nbf_MPI_Neighbor_reduce_scatter_block_init,
  nbf_MPI_Neighbor_scan_init,
  nbf_MPI_Neighbor_scatter_init,
  nbf_MPI_Neighbor_scatterv_init
};

#define NUM_UC_DOUBLE 4
#define NUM_UC_INT64 0

template <typename M, auto E> class alignas(64) HandleData {
protected:
public:
  static M nullHandle;
  M handle{nullHandle};
  HandleData() {}
  HandleData(const HandleData &o) { this->init(o.handle); }
  HandleData(HandleData &&o) noexcept { this->init(o.handle); }
  virtual void init(M group) { handle = group; }
  virtual void fini() {}

  // define new/delete operator to avoid dependency to libstdc++
  void *operator new(size_t size, void *ptr) { return ptr; }

private:
  void *operator new(size_t size) {
    static_assert(false, "operator new must never be called directly");
    return malloc(size);
  }
  void operator delete(void *p) {
    static_assert(false, "operator delete must never be called directly");
  }
};

class alignas(64) CommData {
  MPI_Comm dupComm{MPI_COMM_NULL};
  int size{0};
  int rank{-1};

public:
  static MPI_Comm nullHandle;
  MPI_Comm handle{nullHandle};
  void init(MPI_Comm comm) {
    this->handle = comm;
    if (comm == MPI_COMM_NULL)
      return;
    PMPI_Comm_dup(comm, &dupComm);
    PMPI_Comm_size(comm, &size);
    PMPI_Comm_rank(comm, &rank);
  }
  void fini() { PMPI_Comm_free(&dupComm); }
  int getSize() { return size; }
  int getRank() { return rank; }
  MPI_Comm &getDupComm() { return dupComm; }

  // define new/delete operator to avoid dependency to libstdc++
  void *operator new(size_t size, void *ptr) { return ptr; }

private:
  void *operator new(size_t size) { // static_assert(false, "operator new must
                                    // never be called directly");
    assert(false && "operator new must never be called directly");
    return malloc(size);
  }
  void operator delete(void *p) { // static_assert(false, "operator delete must
                                  // never be called directly");
    assert(false && "operator delete must never be called directly");
  }
};

typedef enum {
  ISEND = 0,
  IRECV,
  IMRECV,
  MPROBE,
  ICOLL,
  IREDUCE,
  IALLREDUCE,
  IBCAST,
  PSEND,
  PRECV,
  PCOLL,
  PREDUCE,
  PALLREDUCE,
  PBCAST
} KIND;

class ipcData {
public:
  double uc_double[NUM_UC_DOUBLE];
  int64_t uc_int64[NUM_UC_INT64];
  static int num_uc_double;
  static int num_uc_int64;
  static MPI_Datatype ipcMpiType;
  static void initIpcData();
  static void finiIpcData();
  void IBcast(int root, CommData *cData, MPI_Request *reqs) {
    PMPI_Ibcast(uc_double, 1, ipcData::ipcMpiType, root, cData->getDupComm(),
                reqs);
  }
  void IAllreduce(CommData *cData, MPI_Request *reqs) {
    if (num_uc_double > 0)
      PMPI_Iallreduce(MPI_IN_PLACE, uc_double, num_uc_double, MPI_DOUBLE,
                      MPI_MAX, cData->getDupComm(), reqs);
    if (num_uc_int64 > 0)
      PMPI_Iallreduce(MPI_IN_PLACE, uc_int64, num_uc_int64, MPI_INT64_T,
                      MPI_MAX, cData->getDupComm(), reqs + 1);
  }
  void Allreduce(CommData *cData) {
    if (num_uc_double > 0)
      PMPI_Allreduce(MPI_IN_PLACE, uc_double, num_uc_double, MPI_DOUBLE,
                     MPI_MAX, cData->getDupComm());
    if (num_uc_int64 > 0)
      PMPI_Allreduce(MPI_IN_PLACE, uc_int64, num_uc_int64, MPI_INT64_T, MPI_MAX,
                     cData->getDupComm());
  }
  void IReduce(int root, CommData *cData, MPI_Request *reqs) {
    if (root == cData->getRank()) {
      if (num_uc_double > 0)
        PMPI_Ireduce(MPI_IN_PLACE, uc_double, num_uc_double, MPI_DOUBLE,
                     MPI_MAX, root, cData->getDupComm(), reqs);
      if (num_uc_int64 > 0)
        PMPI_Ireduce(MPI_IN_PLACE, uc_int64, num_uc_int64, MPI_INT64_T, MPI_MAX,
                     root, cData->getDupComm(), reqs + 1);
    } else {
      if (num_uc_double > 0)
        PMPI_Ireduce(uc_double, NULL, num_uc_double, MPI_DOUBLE, MPI_MAX, root,
                     cData->getDupComm(), reqs);
      if (num_uc_int64 > 0)
        PMPI_Ireduce(uc_int64, NULL, num_uc_int64, MPI_INT64_T, MPI_MAX, root,
                     cData->getDupComm(), reqs + 1);
    }
  }
#ifdef HAVE_PCOLL
  void BcastInit(int root, CommData *cData, MPI_Request *reqs) {
    PMPI_Bcast_init(uc_double, 1, ipcData::ipcMpiType, root,
                    cData->getDupComm(), MPI_INFO_NULL, reqs);
  }
  void AllreduceInit(CommData *cData, MPI_Request *reqs) {
    if (num_uc_double > 0)
      PMPI_Allreduce_init(MPI_IN_PLACE, uc_double, num_uc_double, MPI_DOUBLE,
                          MPI_MAX, cData->getDupComm(), MPI_INFO_NULL, reqs);
    if (num_uc_int64 > 0)
      PMPI_Allreduce_init(MPI_IN_PLACE, uc_int64, num_uc_int64, MPI_INT64_T,
                          MPI_MAX, cData->getDupComm(), MPI_INFO_NULL,
                          reqs + 1);
  }
  void ReduceInit(int root, CommData *cData, MPI_Request *reqs) {
    if (root == cData->getRank()) {
      if (num_uc_double > 0)
        PMPI_Reduce_init(MPI_IN_PLACE, uc_double, num_uc_double, MPI_DOUBLE,
                         MPI_MAX, root, cData->getDupComm(), MPI_INFO_NULL,
                         reqs);
      if (num_uc_int64 > 0)
        PMPI_Reduce_init(MPI_IN_PLACE, uc_int64, num_uc_int64, MPI_INT64_T,
                         MPI_MAX, root, cData->getDupComm(), MPI_INFO_NULL,
                         reqs + 1);
    } else {
      if (num_uc_double > 0)
        PMPI_Reduce_init(uc_double, NULL, num_uc_double, MPI_DOUBLE, MPI_MAX,
                         root, cData->getDupComm(), MPI_INFO_NULL, reqs);
      if (num_uc_int64 > 0)
        PMPI_Reduce_init(uc_int64, NULL, num_uc_int64, MPI_INT64_T, MPI_MAX,
                         root, cData->getDupComm(), MPI_INFO_NULL, reqs + 1);
    }
  }
#endif
};

class alignas(64) RequestData : public ipcData {
  bool persistent{false};
  std::function<void(RequestData *)> startCallback{};
  std::function<void(RequestData *)> cancelCallback{};
  std::function<void(RequestData *, MPI_Status *)> completionCallback{};
  bool freed{true};
  bool cancelled{false};

protected:
public:
  KIND kind{ISEND};
  MPI_Request pb_reqs[2]{MPI_REQUEST_NULL, MPI_REQUEST_NULL};
  int remote{-42};
  int tag{-42};
  int root{-42};
  CommData *comm{nullptr};

  static MPI_Request nullHandle;
  RequestData() {}
  ~RequestData() {}
  MPI_Request handle{MPI_REQUEST_NULL};

  void init(MPI_Request request, KIND _kind, int _remote = -1, int _tag = -1,
            CommData *_comm = nullptr, int _root = -1,
            bool _persistent = false) {
    handle = request;
    kind = _kind;
    remote = _remote;
    tag = _tag;
    comm = _comm;
    root = _root;
    persistent = _persistent;
    freed = false;
    cancelled = false;
  }

  template <typename M> void init(M request, bool _persistent = false) {
    persistent = _persistent;
    handle = (MPI_Request)request;
    freed = false;
    cancelled = false;
  }

  void start() {
    assert(!freed);
    if (startCallback) {
      startCallback(this);
    }
  }
  void cancel() {
    assert(!freed);
    assert(!cancelled);
    cancelled = true;
    if (cancelCallback) {
      cancelCallback(this);
    }
  }
  void complete(MPI_Status *status) {
    assert(!freed);
    if (completionCallback) {
      completionCallback(this, status);
    }
  }
  void fini(MPI_Status *status) {
    complete(status);
    fini();
  }
  void fini() {
    handle = MPI_REQUEST_NULL;
    pb_reqs[0] = MPI_REQUEST_NULL;
    pb_reqs[1] = MPI_REQUEST_NULL;
    completionCallback = nullptr;
    startCallback = nullptr;
    cancelCallback = nullptr;
    remote = -42;
    tag = -42;
    root = -42;
    kind = ISEND;
    freed = true;
    cancelled = false;
  }
  bool isFreed() { return freed; }
  bool isPersistent() {
    assert(!freed);
    return persistent;
  }
  bool isCancelled() {
    bool ret = cancelled;
    cancelled = false;
    return ret;
  }
  void setStartCallback(std::function<void(RequestData *)> sc) {
    persistent = true;
    startCallback = sc;
  }
  void setCancelCallback(std::function<void(RequestData *)> cc) {
    cancelCallback = cc;
  }
  void
  setCompletionCallback(std::function<void(RequestData *, MPI_Status *)> cc) {
    completionCallback = cc;
  }
};
