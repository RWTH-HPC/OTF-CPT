#include "kmpc-intercept.h"

typedef void (*__kmpc_dispatch_init_4_t)(ident_t *loc, kmp_int32 gtid,
                                         enum sched_type schedule, kmp_int32 lb,
                                         kmp_int32 ub, kmp_int32 st,
                                         kmp_int32 chunk);
void __kmpc_dispatch_init_4(ident_t *loc, kmp_int32 gtid,
                            enum sched_type schedule, kmp_int32 lb,
                            kmp_int32 ub, kmp_int32 st, kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_dispatch_init_4_t __kmpc_dispatch_init_4_function = NULL;
  if (!__kmpc_dispatch_init_4_function)
    __kmpc_dispatch_init_4_function =
        (__kmpc_dispatch_init_4_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_init_4");

  __kmpc_dispatch_init_4_function(loc, gtid, schedule, lb, ub, st, chunk);
  EXITOMP();
}

typedef void (*__kmpc_dispatch_init_4u_t)(ident_t *loc, kmp_int32 gtid,
                                          enum sched_type schedule,
                                          kmp_uint32 lb, kmp_uint32 ub,
                                          kmp_int32 st, kmp_int32 chunk);
void __kmpc_dispatch_init_4u(ident_t *loc, kmp_int32 gtid,
                             enum sched_type schedule, kmp_uint32 lb,
                             kmp_uint32 ub, kmp_int32 st, kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_dispatch_init_4u_t __kmpc_dispatch_init_4u_function = NULL;
  if (!__kmpc_dispatch_init_4u_function)
    __kmpc_dispatch_init_4u_function =
        (__kmpc_dispatch_init_4u_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_init_4u");

  __kmpc_dispatch_init_4u_function(loc, gtid, schedule, lb, ub, st, chunk);
  EXITOMP();
}

typedef void (*__kmpc_dispatch_init_8_t)(ident_t *loc, kmp_int32 gtid,
                                         enum sched_type schedule, kmp_int64 lb,
                                         kmp_int64 ub, kmp_int64 st,
                                         kmp_int64 chunk);
void __kmpc_dispatch_init_8(ident_t *loc, kmp_int32 gtid,
                            enum sched_type schedule, kmp_int64 lb,
                            kmp_int64 ub, kmp_int64 st, kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_dispatch_init_8_t __kmpc_dispatch_init_8_function = NULL;
  if (!__kmpc_dispatch_init_8_function)
    __kmpc_dispatch_init_8_function =
        (__kmpc_dispatch_init_8_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_init_8");

  __kmpc_dispatch_init_8_function(loc, gtid, schedule, lb, ub, st, chunk);
  EXITOMP();
}

typedef void (*__kmpc_dispatch_init_8u_t)(ident_t *loc, kmp_int32 gtid,
                                          enum sched_type schedule,
                                          kmp_uint64 lb, kmp_uint64 ub,
                                          kmp_int64 st, kmp_int64 chunk);
void __kmpc_dispatch_init_8u(ident_t *loc, kmp_int32 gtid,
                             enum sched_type schedule, kmp_uint64 lb,
                             kmp_uint64 ub, kmp_int64 st, kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_dispatch_init_8u_t __kmpc_dispatch_init_8u_function = NULL;
  if (!__kmpc_dispatch_init_8u_function)
    __kmpc_dispatch_init_8u_function =
        (__kmpc_dispatch_init_8u_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_init_8u");

  __kmpc_dispatch_init_8u_function(loc, gtid, schedule, lb, ub, st, chunk);
  EXITOMP();
}

typedef int (*__kmpc_dispatch_next_4_t)(ident_t *loc, kmp_int32 gtid,
                                        kmp_int32 *p_last, kmp_int32 *p_lb,
                                        kmp_int32 *p_ub, kmp_int32 *p_st);
int __kmpc_dispatch_next_4(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                           kmp_int32 *p_lb, kmp_int32 *p_ub, kmp_int32 *p_st) {
  ENTEROMP();
  static __kmpc_dispatch_next_4_t __kmpc_dispatch_next_4_function = NULL;
  if (!__kmpc_dispatch_next_4_function)
    __kmpc_dispatch_next_4_function =
        (__kmpc_dispatch_next_4_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_next_4");

  int retval =
      __kmpc_dispatch_next_4_function(loc, gtid, p_last, p_lb, p_ub, p_st);
  EXITOMP();
  return retval;
}

typedef int (*__kmpc_dispatch_next_4u_t)(ident_t *loc, kmp_int32 gtid,
                                         kmp_int32 *p_last, kmp_uint32 *p_lb,
                                         kmp_uint32 *p_ub, kmp_int32 *p_st);
int __kmpc_dispatch_next_4u(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                            kmp_uint32 *p_lb, kmp_uint32 *p_ub,
                            kmp_int32 *p_st) {
  ENTEROMP();
  static __kmpc_dispatch_next_4u_t __kmpc_dispatch_next_4u_function = NULL;
  if (!__kmpc_dispatch_next_4u_function)
    __kmpc_dispatch_next_4u_function =
        (__kmpc_dispatch_next_4u_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_next_4u");

  int retval =
      __kmpc_dispatch_next_4u_function(loc, gtid, p_last, p_lb, p_ub, p_st);
  EXITOMP();
  return retval;
}

typedef int (*__kmpc_dispatch_next_8_t)(ident_t *loc, kmp_int32 gtid,
                                        kmp_int32 *p_last, kmp_int64 *p_lb,
                                        kmp_int64 *p_ub, kmp_int64 *p_st);
int __kmpc_dispatch_next_8(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                           kmp_int64 *p_lb, kmp_int64 *p_ub, kmp_int64 *p_st) {
  ENTEROMP();
  static __kmpc_dispatch_next_8_t __kmpc_dispatch_next_8_function = NULL;
  if (!__kmpc_dispatch_next_8_function)
    __kmpc_dispatch_next_8_function =
        (__kmpc_dispatch_next_8_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_next_8");

  int retval =
      __kmpc_dispatch_next_8_function(loc, gtid, p_last, p_lb, p_ub, p_st);
  EXITOMP();
  return retval;
}

typedef int (*__kmpc_dispatch_next_8u_t)(ident_t *loc, kmp_int32 gtid,
                                         kmp_int32 *p_last, kmp_uint64 *p_lb,
                                         kmp_uint64 *p_ub, kmp_int64 *p_st);
int __kmpc_dispatch_next_8u(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                            kmp_uint64 *p_lb, kmp_uint64 *p_ub,
                            kmp_int64 *p_st) {
  ENTEROMP();
  static __kmpc_dispatch_next_8u_t __kmpc_dispatch_next_8u_function = NULL;
  if (!__kmpc_dispatch_next_8u_function)
    __kmpc_dispatch_next_8u_function =
        (__kmpc_dispatch_next_8u_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_next_8u");

  int retval =
      __kmpc_dispatch_next_8u_function(loc, gtid, p_last, p_lb, p_ub, p_st);
  EXITOMP();
  return retval;
}

typedef void (*__kmpc_dispatch_fini_4_t)(ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_fini_4(ident_t *loc, kmp_int32 gtid) {
  ENTEROMP();
  static __kmpc_dispatch_fini_4_t __kmpc_dispatch_fini_4_function = NULL;
  if (!__kmpc_dispatch_fini_4_function)
    __kmpc_dispatch_fini_4_function =
        (__kmpc_dispatch_fini_4_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_fini_4");

  __kmpc_dispatch_fini_4_function(loc, gtid);
  EXITOMP();
}

typedef void (*__kmpc_dispatch_fini_8_t)(ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_fini_8(ident_t *loc, kmp_int32 gtid) {
  ENTEROMP();
  static __kmpc_dispatch_fini_8_t __kmpc_dispatch_fini_8_function = NULL;
  if (!__kmpc_dispatch_fini_8_function)
    __kmpc_dispatch_fini_8_function =
        (__kmpc_dispatch_fini_8_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_fini_8");

  __kmpc_dispatch_fini_8_function(loc, gtid);
  EXITOMP();
}

typedef void (*__kmpc_dispatch_fini_4u_t)(ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_fini_4u(ident_t *loc, kmp_int32 gtid) {
  ENTEROMP();
  static __kmpc_dispatch_fini_4u_t __kmpc_dispatch_fini_4u_function = NULL;
  if (!__kmpc_dispatch_fini_4u_function)
    __kmpc_dispatch_fini_4u_function =
        (__kmpc_dispatch_fini_4u_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_fini_4u");

  __kmpc_dispatch_fini_4u_function(loc, gtid);
  EXITOMP();
}

typedef void (*__kmpc_dispatch_fini_8u_t)(ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_fini_8u(ident_t *loc, kmp_int32 gtid) {
  ENTEROMP();
  static __kmpc_dispatch_fini_8u_t __kmpc_dispatch_fini_8u_function = NULL;
  if (!__kmpc_dispatch_fini_8u_function)
    __kmpc_dispatch_fini_8u_function =
        (__kmpc_dispatch_fini_8u_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_fini_8u");

  __kmpc_dispatch_fini_8u_function(loc, gtid);
  EXITOMP();
}

typedef void (*__kmpc_dispatch_deinit_t)(ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_deinit(ident_t *loc, kmp_int32 gtid) {
  ENTEROMP();
  static __kmpc_dispatch_deinit_t __kmpc_dispatch_deinit_function = NULL;
  if (!__kmpc_dispatch_deinit_function)
    __kmpc_dispatch_deinit_function =
        (__kmpc_dispatch_deinit_t)dlsym(RTLD_NEXT, "__kmpc_dispatch_deinit");

  __kmpc_dispatch_deinit_function(loc, gtid);
  EXITOMP();
}

#if 0
typedef int  (*__kmpc_invoke_task_func_t)(int gtid);
int  __kmpc_invoke_task_func(int gtid){
    ENTEROMP();
    static __kmpc_invoke_task_func_t __kmpc_invoke_task_func_function = NULL;    if (!__kmpc_invoke_task_func_function)        __kmpc_invoke_task_func_function = (__kmpc_invoke_task_func_t)dlsym(RTLD_NEXT,"__kmpc_invoke_task_func");

    int  retval = __kmpc_invoke_task_func_function(gtid);
    EXITOMP();
    return retval;
}

typedef kmp_event_t * (*__kmpc_task_allow_completion_event_t)(ident_t *loc_ref,int gtid,kmp_task_t *task);
kmp_event_t * __kmpc_task_allow_completion_event(ident_t *loc_ref,int gtid,kmp_task_t *task){
    ENTEROMP();
    static __kmpc_task_allow_completion_event_t __kmpc_task_allow_completion_event_function = NULL;    if (!__kmpc_task_allow_completion_event_function)        __kmpc_task_allow_completion_event_function = (__kmpc_task_allow_completion_event_t)dlsym(RTLD_NEXT,"__kmpc_task_allow_completion_event");

    kmp_event_t * retval = __kmpc_task_allow_completion_event_function(loc_ref,gtid,task);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_begin_t)(ident_t * arg0, kmp_int32 flags);
void  __kmpc_begin(ident_t * arg0, kmp_int32 flags){
    ENTEROMP();
    static __kmpc_begin_t __kmpc_begin_function = NULL;    if (!__kmpc_begin_function)        __kmpc_begin_function = (__kmpc_begin_t)dlsym(RTLD_NEXT,"__kmpc_begin");

    __kmpc_begin_function(arg0,flags);
    EXITOMP();
}

typedef void  (*__kmpc_end_t)(ident_t * arg0);
void  __kmpc_end(ident_t * arg0){
    ENTEROMP();
    static __kmpc_end_t __kmpc_end_function = NULL;    if (!__kmpc_end_function)        __kmpc_end_function = (__kmpc_end_t)dlsym(RTLD_NEXT,"__kmpc_end");

    __kmpc_end_function(arg0);
    EXITOMP();
}

typedef void  (*__kmpc_threadprivate_register_vec_t)(ident_t * arg0, void *data,kmpc_ctor_vec ctor,kmpc_cctor_vec cctor,kmpc_dtor_vec dtor,size_t vector_length);
void  __kmpc_threadprivate_register_vec(ident_t * arg0, void *data,kmpc_ctor_vec ctor,kmpc_cctor_vec cctor,kmpc_dtor_vec dtor,size_t vector_length){
    ENTEROMP();
    static __kmpc_threadprivate_register_vec_t __kmpc_threadprivate_register_vec_function = NULL;    if (!__kmpc_threadprivate_register_vec_function)        __kmpc_threadprivate_register_vec_function = (__kmpc_threadprivate_register_vec_t)dlsym(RTLD_NEXT,"__kmpc_threadprivate_register_vec");

    __kmpc_threadprivate_register_vec_function(arg0,data,ctor,cctor,dtor,vector_length);
    EXITOMP();
}

typedef void  (*__kmpc_threadprivate_register_t)(ident_t * arg0, void *data,kmpc_ctor ctor, kmpc_cctor cctor,kmpc_dtor dtor);
void  __kmpc_threadprivate_register(ident_t * arg0, void *data,kmpc_ctor ctor, kmpc_cctor cctor,kmpc_dtor dtor){
    ENTEROMP();
    static __kmpc_threadprivate_register_t __kmpc_threadprivate_register_function = NULL;    if (!__kmpc_threadprivate_register_function)        __kmpc_threadprivate_register_function = (__kmpc_threadprivate_register_t)dlsym(RTLD_NEXT,"__kmpc_threadprivate_register");

    __kmpc_threadprivate_register_function(arg0,data,ctor,cctor,dtor);
    EXITOMP();
}

typedef void * (*__kmpc_threadprivate_t)(ident_t * arg0, kmp_int32 global_tid,void *data, size_t size);
void * __kmpc_threadprivate(ident_t * arg0, kmp_int32 global_tid,void *data, size_t size){
    ENTEROMP();
    static __kmpc_threadprivate_t __kmpc_threadprivate_function = NULL;    if (!__kmpc_threadprivate_function)        __kmpc_threadprivate_function = (__kmpc_threadprivate_t)dlsym(RTLD_NEXT,"__kmpc_threadprivate");

    void * retval = __kmpc_threadprivate_function(arg0,global_tid,data,size);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_global_thread_num_t)(ident_t * arg0);
kmp_int32  __kmpc_global_thread_num(ident_t * arg0){
    ENTEROMP();
    static __kmpc_global_thread_num_t __kmpc_global_thread_num_function = NULL;    if (!__kmpc_global_thread_num_function)        __kmpc_global_thread_num_function = (__kmpc_global_thread_num_t)dlsym(RTLD_NEXT,"__kmpc_global_thread_num");

    kmp_int32  retval = __kmpc_global_thread_num_function(arg0);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_global_num_threads_t)(ident_t * arg0);
kmp_int32  __kmpc_global_num_threads(ident_t * arg0){
    ENTEROMP();
    static __kmpc_global_num_threads_t __kmpc_global_num_threads_function = NULL;    if (!__kmpc_global_num_threads_function)        __kmpc_global_num_threads_function = (__kmpc_global_num_threads_t)dlsym(RTLD_NEXT,"__kmpc_global_num_threads");

    kmp_int32  retval = __kmpc_global_num_threads_function(arg0);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_bound_thread_num_t)(ident_t * arg0);
kmp_int32  __kmpc_bound_thread_num(ident_t * arg0){
    ENTEROMP();
    static __kmpc_bound_thread_num_t __kmpc_bound_thread_num_function = NULL;    if (!__kmpc_bound_thread_num_function)        __kmpc_bound_thread_num_function = (__kmpc_bound_thread_num_t)dlsym(RTLD_NEXT,"__kmpc_bound_thread_num");

    kmp_int32  retval = __kmpc_bound_thread_num_function(arg0);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_bound_num_threads_t)(ident_t * arg0);
kmp_int32  __kmpc_bound_num_threads(ident_t * arg0){
    ENTEROMP();
    static __kmpc_bound_num_threads_t __kmpc_bound_num_threads_function = NULL;    if (!__kmpc_bound_num_threads_function)        __kmpc_bound_num_threads_function = (__kmpc_bound_num_threads_t)dlsym(RTLD_NEXT,"__kmpc_bound_num_threads");

    kmp_int32  retval = __kmpc_bound_num_threads_function(arg0);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_ok_to_fork_t)(ident_t * arg0);
kmp_int32  __kmpc_ok_to_fork(ident_t * arg0){
    ENTEROMP();
    static __kmpc_ok_to_fork_t __kmpc_ok_to_fork_function = NULL;    if (!__kmpc_ok_to_fork_function)        __kmpc_ok_to_fork_function = (__kmpc_ok_to_fork_t)dlsym(RTLD_NEXT,"__kmpc_ok_to_fork");

    kmp_int32  retval = __kmpc_ok_to_fork_function(arg0);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_fork_call_if_t)(ident_t *loc, kmp_int32 nargs,kmpc_micro microtask, kmp_int32 cond,void *args);
void  __kmpc_fork_call_if(ident_t *loc, kmp_int32 nargs,kmpc_micro microtask, kmp_int32 cond,void *args){
    ENTEROMP();
    static __kmpc_fork_call_if_t __kmpc_fork_call_if_function = NULL;    if (!__kmpc_fork_call_if_function)        __kmpc_fork_call_if_function = (__kmpc_fork_call_if_t)dlsym(RTLD_NEXT,"__kmpc_fork_call_if");

    __kmpc_fork_call_if_function(loc,nargs,microtask,cond,args);
    EXITOMP();
}

typedef void  (*__kmpc_serialized_parallel_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_serialized_parallel(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_serialized_parallel_t __kmpc_serialized_parallel_function = NULL;    if (!__kmpc_serialized_parallel_function)        __kmpc_serialized_parallel_function = (__kmpc_serialized_parallel_t)dlsym(RTLD_NEXT,"__kmpc_serialized_parallel");

    __kmpc_serialized_parallel_function(arg0,global_tid);
    EXITOMP();
}

typedef void  (*__kmpc_end_serialized_parallel_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_end_serialized_parallel(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_end_serialized_parallel_t __kmpc_end_serialized_parallel_function = NULL;    if (!__kmpc_end_serialized_parallel_function)        __kmpc_end_serialized_parallel_function = (__kmpc_end_serialized_parallel_t)dlsym(RTLD_NEXT,"__kmpc_end_serialized_parallel");

    __kmpc_end_serialized_parallel_function(arg0,global_tid);
    EXITOMP();
}

typedef void  (*__kmpc_flush_t)(ident_t * arg0);
void  __kmpc_flush(ident_t * arg0){
    ENTEROMP();
    static __kmpc_flush_t __kmpc_flush_function = NULL;    if (!__kmpc_flush_function)        __kmpc_flush_function = (__kmpc_flush_t)dlsym(RTLD_NEXT,"__kmpc_flush");

    __kmpc_flush_function(arg0);
    EXITOMP();
}

typedef void  (*__kmpc_barrier_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_barrier(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_barrier_t __kmpc_barrier_function = NULL;    if (!__kmpc_barrier_function)        __kmpc_barrier_function = (__kmpc_barrier_t)dlsym(RTLD_NEXT,"__kmpc_barrier");

    __kmpc_barrier_function(arg0,global_tid);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_master_t)(ident_t * arg0, kmp_int32 global_tid);
kmp_int32  __kmpc_master(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_master_t __kmpc_master_function = NULL;    if (!__kmpc_master_function)        __kmpc_master_function = (__kmpc_master_t)dlsym(RTLD_NEXT,"__kmpc_master");

    kmp_int32  retval = __kmpc_master_function(arg0,global_tid);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_master_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_end_master(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_end_master_t __kmpc_end_master_function = NULL;    if (!__kmpc_end_master_function)        __kmpc_end_master_function = (__kmpc_end_master_t)dlsym(RTLD_NEXT,"__kmpc_end_master");

    __kmpc_end_master_function(arg0,global_tid);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_masked_t)(ident_t * arg0, kmp_int32 global_tid,kmp_int32 filter);
kmp_int32  __kmpc_masked(ident_t * arg0, kmp_int32 global_tid,kmp_int32 filter){
    ENTEROMP();
    static __kmpc_masked_t __kmpc_masked_function = NULL;    if (!__kmpc_masked_function)        __kmpc_masked_function = (__kmpc_masked_t)dlsym(RTLD_NEXT,"__kmpc_masked");

    kmp_int32  retval = __kmpc_masked_function(arg0,global_tid,filter);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_masked_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_end_masked(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_end_masked_t __kmpc_end_masked_function = NULL;    if (!__kmpc_end_masked_function)        __kmpc_end_masked_function = (__kmpc_end_masked_t)dlsym(RTLD_NEXT,"__kmpc_end_masked");

    __kmpc_end_masked_function(arg0,global_tid);
    EXITOMP();
}

typedef void  (*__kmpc_ordered_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_ordered(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_ordered_t __kmpc_ordered_function = NULL;    if (!__kmpc_ordered_function)        __kmpc_ordered_function = (__kmpc_ordered_t)dlsym(RTLD_NEXT,"__kmpc_ordered");

    __kmpc_ordered_function(arg0,global_tid);
    EXITOMP();
}

typedef void  (*__kmpc_end_ordered_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_end_ordered(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_end_ordered_t __kmpc_end_ordered_function = NULL;    if (!__kmpc_end_ordered_function)        __kmpc_end_ordered_function = (__kmpc_end_ordered_t)dlsym(RTLD_NEXT,"__kmpc_end_ordered");

    __kmpc_end_ordered_function(arg0,global_tid);
    EXITOMP();
}

typedef void  (*__kmpc_critical_t)(ident_t * arg0, kmp_int32 global_tid,kmp_critical_name * arg2);
void  __kmpc_critical(ident_t * arg0, kmp_int32 global_tid,kmp_critical_name * arg2){
    ENTEROMP();
    static __kmpc_critical_t __kmpc_critical_function = NULL;    if (!__kmpc_critical_function)        __kmpc_critical_function = (__kmpc_critical_t)dlsym(RTLD_NEXT,"__kmpc_critical");

    __kmpc_critical_function(arg0,global_tid,arg2);
    EXITOMP();
}

typedef void  (*__kmpc_end_critical_t)(ident_t * arg0, kmp_int32 global_tid,kmp_critical_name * arg2);
void  __kmpc_end_critical(ident_t * arg0, kmp_int32 global_tid,kmp_critical_name * arg2){
    ENTEROMP();
    static __kmpc_end_critical_t __kmpc_end_critical_function = NULL;    if (!__kmpc_end_critical_function)        __kmpc_end_critical_function = (__kmpc_end_critical_t)dlsym(RTLD_NEXT,"__kmpc_end_critical");

    __kmpc_end_critical_function(arg0,global_tid,arg2);
    EXITOMP();
}

typedef void  (*__kmpc_critical_with_hint_t)(ident_t * arg0, kmp_int32 global_tid,kmp_critical_name * arg2, uint32_t hint);
void  __kmpc_critical_with_hint(ident_t * arg0, kmp_int32 global_tid,kmp_critical_name * arg2, uint32_t hint){
    ENTEROMP();
    static __kmpc_critical_with_hint_t __kmpc_critical_with_hint_function = NULL;    if (!__kmpc_critical_with_hint_function)        __kmpc_critical_with_hint_function = (__kmpc_critical_with_hint_t)dlsym(RTLD_NEXT,"__kmpc_critical_with_hint");

    __kmpc_critical_with_hint_function(arg0,global_tid,arg2,hint);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_barrier_master_t)(ident_t * arg0, kmp_int32 global_tid);
kmp_int32  __kmpc_barrier_master(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_barrier_master_t __kmpc_barrier_master_function = NULL;    if (!__kmpc_barrier_master_function)        __kmpc_barrier_master_function = (__kmpc_barrier_master_t)dlsym(RTLD_NEXT,"__kmpc_barrier_master");

    kmp_int32  retval = __kmpc_barrier_master_function(arg0,global_tid);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_barrier_master_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_end_barrier_master(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_end_barrier_master_t __kmpc_end_barrier_master_function = NULL;    if (!__kmpc_end_barrier_master_function)        __kmpc_end_barrier_master_function = (__kmpc_end_barrier_master_t)dlsym(RTLD_NEXT,"__kmpc_end_barrier_master");

    __kmpc_end_barrier_master_function(arg0,global_tid);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_barrier_master_nowait_t)(ident_t * arg0,kmp_int32 global_tid);
kmp_int32  __kmpc_barrier_master_nowait(ident_t * arg0,kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_barrier_master_nowait_t __kmpc_barrier_master_nowait_function = NULL;    if (!__kmpc_barrier_master_nowait_function)        __kmpc_barrier_master_nowait_function = (__kmpc_barrier_master_nowait_t)dlsym(RTLD_NEXT,"__kmpc_barrier_master_nowait");

    kmp_int32  retval = __kmpc_barrier_master_nowait_function(arg0,global_tid);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_single_t)(ident_t * arg0, kmp_int32 global_tid);
kmp_int32  __kmpc_single(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_single_t __kmpc_single_function = NULL;    if (!__kmpc_single_function)        __kmpc_single_function = (__kmpc_single_t)dlsym(RTLD_NEXT,"__kmpc_single");

    kmp_int32  retval = __kmpc_single_function(arg0,global_tid);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_single_t)(ident_t * arg0, kmp_int32 global_tid);
void  __kmpc_end_single(ident_t * arg0, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_end_single_t __kmpc_end_single_function = NULL;    if (!__kmpc_end_single_function)        __kmpc_end_single_function = (__kmpc_end_single_t)dlsym(RTLD_NEXT,"__kmpc_end_single");

    __kmpc_end_single_function(arg0,global_tid);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_sections_init_t)(ident_t *loc, kmp_int32 global_tid);
kmp_int32  __kmpc_sections_init(ident_t *loc, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_sections_init_t __kmpc_sections_init_function = NULL;    if (!__kmpc_sections_init_function)        __kmpc_sections_init_function = (__kmpc_sections_init_t)dlsym(RTLD_NEXT,"__kmpc_sections_init");

    kmp_int32  retval = __kmpc_sections_init_function(loc,global_tid);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_next_section_t)(ident_t *loc, kmp_int32 global_tid,kmp_int32 numberOfSections);
kmp_int32  __kmpc_next_section(ident_t *loc, kmp_int32 global_tid,kmp_int32 numberOfSections){
    ENTEROMP();
    static __kmpc_next_section_t __kmpc_next_section_function = NULL;    if (!__kmpc_next_section_function)        __kmpc_next_section_function = (__kmpc_next_section_t)dlsym(RTLD_NEXT,"__kmpc_next_section");

    kmp_int32  retval = __kmpc_next_section_function(loc,global_tid,numberOfSections);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_sections_t)(ident_t *loc, kmp_int32 global_tid);
void  __kmpc_end_sections(ident_t *loc, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_end_sections_t __kmpc_end_sections_function = NULL;    if (!__kmpc_end_sections_function)        __kmpc_end_sections_function = (__kmpc_end_sections_t)dlsym(RTLD_NEXT,"__kmpc_end_sections");

    __kmpc_end_sections_function(loc,global_tid);
    EXITOMP();
}

typedef void  (*__kmpc_copyprivate_t)(ident_t *loc, kmp_int32 global_tid,size_t cpy_size, void *cpy_data,kmp_redfun_t cpy_func,kmp_int32 didit);
void  __kmpc_copyprivate(ident_t *loc, kmp_int32 global_tid,size_t cpy_size, void *cpy_data,kmp_redfun_t cpy_func,kmp_int32 didit){
    ENTEROMP();
    static __kmpc_copyprivate_t __kmpc_copyprivate_function = NULL;    if (!__kmpc_copyprivate_function)        __kmpc_copyprivate_function = (__kmpc_copyprivate_t)dlsym(RTLD_NEXT,"__kmpc_copyprivate");

    __kmpc_copyprivate_function(loc,global_tid,cpy_size,cpy_data,cpy_func,didit);
    EXITOMP();
}

typedef void * (*__kmpc_copyprivate_light_t)(ident_t *loc, kmp_int32 gtid,void *cpy_data);
void * __kmpc_copyprivate_light(ident_t *loc, kmp_int32 gtid,void *cpy_data){
    ENTEROMP();
    static __kmpc_copyprivate_light_t __kmpc_copyprivate_light_function = NULL;    if (!__kmpc_copyprivate_light_function)        __kmpc_copyprivate_light_function = (__kmpc_copyprivate_light_t)dlsym(RTLD_NEXT,"__kmpc_copyprivate_light");

    void * retval = __kmpc_copyprivate_light_function(loc,gtid,cpy_data);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_omp_task_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *new_task);
kmp_int32  __kmpc_omp_task(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *new_task){
    ENTEROMP();
    static __kmpc_omp_task_t __kmpc_omp_task_function = NULL;    if (!__kmpc_omp_task_function)        __kmpc_omp_task_function = (__kmpc_omp_task_t)dlsym(RTLD_NEXT,"__kmpc_omp_task");

    kmp_int32  retval = __kmpc_omp_task_function(loc_ref,gtid,new_task);
    EXITOMP();
    return retval;
}

typedef kmp_task_t * (*__kmpc_omp_task_alloc_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 flags,size_t sizeof_kmp_task_t,size_t sizeof_shareds,kmp_routine_entry_t task_entry);
kmp_task_t * __kmpc_omp_task_alloc(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 flags,size_t sizeof_kmp_task_t,size_t sizeof_shareds,kmp_routine_entry_t task_entry){
    ENTEROMP();
    static __kmpc_omp_task_alloc_t __kmpc_omp_task_alloc_function = NULL;    if (!__kmpc_omp_task_alloc_function)        __kmpc_omp_task_alloc_function = (__kmpc_omp_task_alloc_t)dlsym(RTLD_NEXT,"__kmpc_omp_task_alloc");

    kmp_task_t * retval = __kmpc_omp_task_alloc_function(loc_ref,gtid,flags,sizeof_kmp_task_t,sizeof_shareds,task_entry);
    EXITOMP();
    return retval;
}

typedef kmp_task_t * (*__kmpc_omp_target_task_alloc_t)(ident_t *loc_ref, kmp_int32 gtid, kmp_int32 flags, size_t sizeof_kmp_task_t,size_t sizeof_shareds, kmp_routine_entry_t task_entry, kmp_int64 device_id);
kmp_task_t * __kmpc_omp_target_task_alloc(ident_t *loc_ref, kmp_int32 gtid, kmp_int32 flags, size_t sizeof_kmp_task_t,size_t sizeof_shareds, kmp_routine_entry_t task_entry, kmp_int64 device_id){
    ENTEROMP();
    static __kmpc_omp_target_task_alloc_t __kmpc_omp_target_task_alloc_function = NULL;    if (!__kmpc_omp_target_task_alloc_function)        __kmpc_omp_target_task_alloc_function = (__kmpc_omp_target_task_alloc_t)dlsym(RTLD_NEXT,"__kmpc_omp_target_task_alloc");

    kmp_task_t * retval = __kmpc_omp_target_task_alloc_function(loc_ref,gtid,flags,sizeof_kmp_task_t,sizeof_shareds,task_entry,device_id);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_omp_task_begin_if0_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *task);
void  __kmpc_omp_task_begin_if0(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *task){
    ENTEROMP();
    static __kmpc_omp_task_begin_if0_t __kmpc_omp_task_begin_if0_function = NULL;    if (!__kmpc_omp_task_begin_if0_function)        __kmpc_omp_task_begin_if0_function = (__kmpc_omp_task_begin_if0_t)dlsym(RTLD_NEXT,"__kmpc_omp_task_begin_if0");

    __kmpc_omp_task_begin_if0_function(loc_ref,gtid,task);
    EXITOMP();
}

typedef void  (*__kmpc_omp_task_complete_if0_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *task);
void  __kmpc_omp_task_complete_if0(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *task){
    ENTEROMP();
    static __kmpc_omp_task_complete_if0_t __kmpc_omp_task_complete_if0_function = NULL;    if (!__kmpc_omp_task_complete_if0_function)        __kmpc_omp_task_complete_if0_function = (__kmpc_omp_task_complete_if0_t)dlsym(RTLD_NEXT,"__kmpc_omp_task_complete_if0");

    __kmpc_omp_task_complete_if0_function(loc_ref,gtid,task);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_omp_task_parts_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *new_task);
kmp_int32  __kmpc_omp_task_parts(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *new_task){
    ENTEROMP();
    static __kmpc_omp_task_parts_t __kmpc_omp_task_parts_function = NULL;    if (!__kmpc_omp_task_parts_function)        __kmpc_omp_task_parts_function = (__kmpc_omp_task_parts_t)dlsym(RTLD_NEXT,"__kmpc_omp_task_parts");

    kmp_int32  retval = __kmpc_omp_task_parts_function(loc_ref,gtid,new_task);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_omp_taskwait_t)(ident_t *loc_ref, kmp_int32 gtid);
kmp_int32  __kmpc_omp_taskwait(ident_t *loc_ref, kmp_int32 gtid){
    ENTEROMP();
    static __kmpc_omp_taskwait_t __kmpc_omp_taskwait_function = NULL;    if (!__kmpc_omp_taskwait_function)        __kmpc_omp_taskwait_function = (__kmpc_omp_taskwait_t)dlsym(RTLD_NEXT,"__kmpc_omp_taskwait");

    kmp_int32  retval = __kmpc_omp_taskwait_function(loc_ref,gtid);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_omp_taskyield_t)(ident_t *loc_ref, kmp_int32 gtid,int end_part);
kmp_int32  __kmpc_omp_taskyield(ident_t *loc_ref, kmp_int32 gtid,int end_part){
    ENTEROMP();
    static __kmpc_omp_taskyield_t __kmpc_omp_taskyield_function = NULL;    if (!__kmpc_omp_taskyield_function)        __kmpc_omp_taskyield_function = (__kmpc_omp_taskyield_t)dlsym(RTLD_NEXT,"__kmpc_omp_taskyield");

    kmp_int32  retval = __kmpc_omp_taskyield_function(loc_ref,gtid,end_part);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_omp_task_begin_t)(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *task);
void  __kmpc_omp_task_begin(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *task){
    ENTEROMP();
    static __kmpc_omp_task_begin_t __kmpc_omp_task_begin_function = NULL;    if (!__kmpc_omp_task_begin_function)        __kmpc_omp_task_begin_function = (__kmpc_omp_task_begin_t)dlsym(RTLD_NEXT,"__kmpc_omp_task_begin");

    __kmpc_omp_task_begin_function(loc_ref,gtid,task);
    EXITOMP();
}

typedef void  (*__kmpc_omp_task_complete_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *task);
void  __kmpc_omp_task_complete(ident_t *loc_ref, kmp_int32 gtid,kmp_task_t *task){
    ENTEROMP();
    static __kmpc_omp_task_complete_t __kmpc_omp_task_complete_function = NULL;    if (!__kmpc_omp_task_complete_function)        __kmpc_omp_task_complete_function = (__kmpc_omp_task_complete_t)dlsym(RTLD_NEXT,"__kmpc_omp_task_complete");

    __kmpc_omp_task_complete_function(loc_ref,gtid,task);
    EXITOMP();
}

typedef void  (*__kmpc_taskgroup_t)(ident_t *loc, int gtid);
void  __kmpc_taskgroup(ident_t *loc, int gtid){
    ENTEROMP();
    static __kmpc_taskgroup_t __kmpc_taskgroup_function = NULL;    if (!__kmpc_taskgroup_function)        __kmpc_taskgroup_function = (__kmpc_taskgroup_t)dlsym(RTLD_NEXT,"__kmpc_taskgroup");

    __kmpc_taskgroup_function(loc,gtid);
    EXITOMP();
}

typedef void  (*__kmpc_end_taskgroup_t)(ident_t *loc, int gtid);
void  __kmpc_end_taskgroup(ident_t *loc, int gtid){
    ENTEROMP();
    static __kmpc_end_taskgroup_t __kmpc_end_taskgroup_function = NULL;    if (!__kmpc_end_taskgroup_function)        __kmpc_end_taskgroup_function = (__kmpc_end_taskgroup_t)dlsym(RTLD_NEXT,"__kmpc_end_taskgroup");

    __kmpc_end_taskgroup_function(loc,gtid);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_omp_task_with_deps_t)(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *new_task, kmp_int32 ndeps,kmp_depend_info_t *dep_list, kmp_int32 ndeps_noalias,kmp_depend_info_t *noalias_dep_list);
kmp_int32  __kmpc_omp_task_with_deps(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *new_task, kmp_int32 ndeps,kmp_depend_info_t *dep_list, kmp_int32 ndeps_noalias,kmp_depend_info_t *noalias_dep_list){
    ENTEROMP();
    static __kmpc_omp_task_with_deps_t __kmpc_omp_task_with_deps_function = NULL;    if (!__kmpc_omp_task_with_deps_function)        __kmpc_omp_task_with_deps_function = (__kmpc_omp_task_with_deps_t)dlsym(RTLD_NEXT,"__kmpc_omp_task_with_deps");

    kmp_int32  retval = __kmpc_omp_task_with_deps_function(loc_ref,gtid,new_task,ndeps,dep_list,ndeps_noalias,noalias_dep_list);
    EXITOMP();
    return retval;
}

typedef kmp_base_depnode_t * (*__kmpc_task_get_depnode_t)(kmp_task_t *task);
kmp_base_depnode_t * __kmpc_task_get_depnode(kmp_task_t *task){
    ENTEROMP();
    static __kmpc_task_get_depnode_t __kmpc_task_get_depnode_function = NULL;    if (!__kmpc_task_get_depnode_function)        __kmpc_task_get_depnode_function = (__kmpc_task_get_depnode_t)dlsym(RTLD_NEXT,"__kmpc_task_get_depnode");

    kmp_base_depnode_t * retval = __kmpc_task_get_depnode_function(task);
    EXITOMP();
    return retval;
}

typedef kmp_depnode_list_t * (*__kmpc_task_get_successors_t)(kmp_task_t *task);
kmp_depnode_list_t * __kmpc_task_get_successors(kmp_task_t *task){
    ENTEROMP();
    static __kmpc_task_get_successors_t __kmpc_task_get_successors_function = NULL;    if (!__kmpc_task_get_successors_function)        __kmpc_task_get_successors_function = (__kmpc_task_get_successors_t)dlsym(RTLD_NEXT,"__kmpc_task_get_successors");

    kmp_depnode_list_t * retval = __kmpc_task_get_successors_function(task);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_omp_wait_deps_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 ndeps,kmp_depend_info_t *dep_list,kmp_int32 ndeps_noalias,kmp_depend_info_t *noalias_dep_list);
void  __kmpc_omp_wait_deps(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 ndeps,kmp_depend_info_t *dep_list,kmp_int32 ndeps_noalias,kmp_depend_info_t *noalias_dep_list){
    ENTEROMP();
    static __kmpc_omp_wait_deps_t __kmpc_omp_wait_deps_function = NULL;    if (!__kmpc_omp_wait_deps_function)        __kmpc_omp_wait_deps_function = (__kmpc_omp_wait_deps_t)dlsym(RTLD_NEXT,"__kmpc_omp_wait_deps");

    __kmpc_omp_wait_deps_function(loc_ref,gtid,ndeps,dep_list,ndeps_noalias,noalias_dep_list);
    EXITOMP();
}

typedef void  (*__kmpc_omp_taskwait_deps_51_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 ndeps,kmp_depend_info_t *dep_list,kmp_int32 ndeps_noalias,kmp_depend_info_t *noalias_dep_list,kmp_int32 has_no_wait);
void  __kmpc_omp_taskwait_deps_51(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 ndeps,kmp_depend_info_t *dep_list,kmp_int32 ndeps_noalias,kmp_depend_info_t *noalias_dep_list,kmp_int32 has_no_wait){
    ENTEROMP();
    static __kmpc_omp_taskwait_deps_51_t __kmpc_omp_taskwait_deps_51_function = NULL;    if (!__kmpc_omp_taskwait_deps_51_function)        __kmpc_omp_taskwait_deps_51_function = (__kmpc_omp_taskwait_deps_51_t)dlsym(RTLD_NEXT,"__kmpc_omp_taskwait_deps_51");

    __kmpc_omp_taskwait_deps_51_function(loc_ref,gtid,ndeps,dep_list,ndeps_noalias,noalias_dep_list,has_no_wait);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_cancel_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 cncl_kind);
kmp_int32  __kmpc_cancel(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 cncl_kind){
    ENTEROMP();
    static __kmpc_cancel_t __kmpc_cancel_function = NULL;    if (!__kmpc_cancel_function)        __kmpc_cancel_function = (__kmpc_cancel_t)dlsym(RTLD_NEXT,"__kmpc_cancel");

    kmp_int32  retval = __kmpc_cancel_function(loc_ref,gtid,cncl_kind);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_cancellationpoint_t)(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 cncl_kind);
kmp_int32  __kmpc_cancellationpoint(ident_t *loc_ref, kmp_int32 gtid,kmp_int32 cncl_kind){
    ENTEROMP();
    static __kmpc_cancellationpoint_t __kmpc_cancellationpoint_function = NULL;    if (!__kmpc_cancellationpoint_function)        __kmpc_cancellationpoint_function = (__kmpc_cancellationpoint_t)dlsym(RTLD_NEXT,"__kmpc_cancellationpoint");

    kmp_int32  retval = __kmpc_cancellationpoint_function(loc_ref,gtid,cncl_kind);
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_cancel_barrier_t)(ident_t *loc_ref, kmp_int32 gtid);
kmp_int32  __kmpc_cancel_barrier(ident_t *loc_ref, kmp_int32 gtid){
    ENTEROMP();
    static __kmpc_cancel_barrier_t __kmpc_cancel_barrier_function = NULL;    if (!__kmpc_cancel_barrier_function)        __kmpc_cancel_barrier_function = (__kmpc_cancel_barrier_t)dlsym(RTLD_NEXT,"__kmpc_cancel_barrier");

    kmp_int32  retval = __kmpc_cancel_barrier_function(loc_ref,gtid);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_proxy_task_completed_t)(kmp_int32 gtid, kmp_task_t *ptask);
void  __kmpc_proxy_task_completed(kmp_int32 gtid, kmp_task_t *ptask){
    ENTEROMP();
    static __kmpc_proxy_task_completed_t __kmpc_proxy_task_completed_function = NULL;    if (!__kmpc_proxy_task_completed_function)        __kmpc_proxy_task_completed_function = (__kmpc_proxy_task_completed_t)dlsym(RTLD_NEXT,"__kmpc_proxy_task_completed");

    __kmpc_proxy_task_completed_function(gtid,ptask);
    EXITOMP();
}

typedef void  (*__kmpc_proxy_task_completed_ooo_t)(kmp_task_t *ptask);
void  __kmpc_proxy_task_completed_ooo(kmp_task_t *ptask){
    ENTEROMP();
    static __kmpc_proxy_task_completed_ooo_t __kmpc_proxy_task_completed_ooo_function = NULL;    if (!__kmpc_proxy_task_completed_ooo_function)        __kmpc_proxy_task_completed_ooo_function = (__kmpc_proxy_task_completed_ooo_t)dlsym(RTLD_NEXT,"__kmpc_proxy_task_completed_ooo");

    __kmpc_proxy_task_completed_ooo_function(ptask);
    EXITOMP();
}

typedef void  (*__kmpc_taskloop_t)(ident_t *loc, kmp_int32 gtid, kmp_task_t *task,kmp_int32 if_val, kmp_uint64 *lb,kmp_uint64 *ub, kmp_int64 st, kmp_int32 nogroup,kmp_int32 sched, kmp_uint64 grainsize,void *task_dup);
void  __kmpc_taskloop(ident_t *loc, kmp_int32 gtid, kmp_task_t *task,kmp_int32 if_val, kmp_uint64 *lb,kmp_uint64 *ub, kmp_int64 st, kmp_int32 nogroup,kmp_int32 sched, kmp_uint64 grainsize,void *task_dup){
    ENTEROMP();
    static __kmpc_taskloop_t __kmpc_taskloop_function = NULL;    if (!__kmpc_taskloop_function)        __kmpc_taskloop_function = (__kmpc_taskloop_t)dlsym(RTLD_NEXT,"__kmpc_taskloop");

    __kmpc_taskloop_function(loc,gtid,task,if_val,lb,ub,st,nogroup,sched,grainsize,task_dup);
    EXITOMP();
}

typedef void  (*__kmpc_taskloop_5_t)(ident_t *loc, kmp_int32 gtid,kmp_task_t *task, kmp_int32 if_val,kmp_uint64 *lb, kmp_uint64 *ub, kmp_int64 st,kmp_int32 nogroup, kmp_int32 sched,kmp_uint64 grainsize, kmp_int32 modifier,void *task_dup);
void  __kmpc_taskloop_5(ident_t *loc, kmp_int32 gtid,kmp_task_t *task, kmp_int32 if_val,kmp_uint64 *lb, kmp_uint64 *ub, kmp_int64 st,kmp_int32 nogroup, kmp_int32 sched,kmp_uint64 grainsize, kmp_int32 modifier,void *task_dup){
    ENTEROMP();
    static __kmpc_taskloop_5_t __kmpc_taskloop_5_function = NULL;    if (!__kmpc_taskloop_5_function)        __kmpc_taskloop_5_function = (__kmpc_taskloop_5_t)dlsym(RTLD_NEXT,"__kmpc_taskloop_5");

    __kmpc_taskloop_5_function(loc,gtid,task,if_val,lb,ub,st,nogroup,sched,grainsize,modifier,task_dup);
    EXITOMP();
}

typedef void * (*__kmpc_task_reduction_init_t)(int gtid, int num_data, void *data);
void * __kmpc_task_reduction_init(int gtid, int num_data, void *data){
    ENTEROMP();
    static __kmpc_task_reduction_init_t __kmpc_task_reduction_init_function = NULL;    if (!__kmpc_task_reduction_init_function)        __kmpc_task_reduction_init_function = (__kmpc_task_reduction_init_t)dlsym(RTLD_NEXT,"__kmpc_task_reduction_init");

    void * retval = __kmpc_task_reduction_init_function(gtid,num_data,data);
    EXITOMP();
    return retval;
}

typedef void * (*__kmpc_taskred_init_t)(int gtid, int num_data, void *data);
void * __kmpc_taskred_init(int gtid, int num_data, void *data){
    ENTEROMP();
    static __kmpc_taskred_init_t __kmpc_taskred_init_function = NULL;    if (!__kmpc_taskred_init_function)        __kmpc_taskred_init_function = (__kmpc_taskred_init_t)dlsym(RTLD_NEXT,"__kmpc_taskred_init");

    void * retval = __kmpc_taskred_init_function(gtid,num_data,data);
    EXITOMP();
    return retval;
}

typedef void * (*__kmpc_task_reduction_get_th_data_t)(int gtid, void *tg, void *d);
void * __kmpc_task_reduction_get_th_data(int gtid, void *tg, void *d){
    ENTEROMP();
    static __kmpc_task_reduction_get_th_data_t __kmpc_task_reduction_get_th_data_function = NULL;    if (!__kmpc_task_reduction_get_th_data_function)        __kmpc_task_reduction_get_th_data_function = (__kmpc_task_reduction_get_th_data_t)dlsym(RTLD_NEXT,"__kmpc_task_reduction_get_th_data");

    void * retval = __kmpc_task_reduction_get_th_data_function(gtid,tg,d);
    EXITOMP();
    return retval;
}

typedef void * (*__kmpc_task_reduction_modifier_init_t)(ident_t *loc, int gtid,int is_ws, int num,void *data);
void * __kmpc_task_reduction_modifier_init(ident_t *loc, int gtid,int is_ws, int num,void *data){
    ENTEROMP();
    static __kmpc_task_reduction_modifier_init_t __kmpc_task_reduction_modifier_init_function = NULL;    if (!__kmpc_task_reduction_modifier_init_function)        __kmpc_task_reduction_modifier_init_function = (__kmpc_task_reduction_modifier_init_t)dlsym(RTLD_NEXT,"__kmpc_task_reduction_modifier_init");

    void * retval = __kmpc_task_reduction_modifier_init_function(loc,gtid,is_ws,num,data);
    EXITOMP();
    return retval;
}

typedef void * (*__kmpc_taskred_modifier_init_t)(ident_t *loc, int gtid, int is_ws,int num, void *data);
void * __kmpc_taskred_modifier_init(ident_t *loc, int gtid, int is_ws,int num, void *data){
    ENTEROMP();
    static __kmpc_taskred_modifier_init_t __kmpc_taskred_modifier_init_function = NULL;    if (!__kmpc_taskred_modifier_init_function)        __kmpc_taskred_modifier_init_function = (__kmpc_taskred_modifier_init_t)dlsym(RTLD_NEXT,"__kmpc_taskred_modifier_init");

    void * retval = __kmpc_taskred_modifier_init_function(loc,gtid,is_ws,num,data);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_task_reduction_modifier_fini_t)(ident_t *loc, int gtid,int is_ws);
void  __kmpc_task_reduction_modifier_fini(ident_t *loc, int gtid,int is_ws){
    ENTEROMP();
    static __kmpc_task_reduction_modifier_fini_t __kmpc_task_reduction_modifier_fini_function = NULL;    if (!__kmpc_task_reduction_modifier_fini_function)        __kmpc_task_reduction_modifier_fini_function = (__kmpc_task_reduction_modifier_fini_t)dlsym(RTLD_NEXT,"__kmpc_task_reduction_modifier_fini");

    __kmpc_task_reduction_modifier_fini_function(loc,gtid,is_ws);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_omp_reg_task_with_affinity_t)(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *new_task, kmp_int32 naffins,kmp_task_affinity_info_t *affin_list);
kmp_int32  __kmpc_omp_reg_task_with_affinity(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *new_task, kmp_int32 naffins,kmp_task_affinity_info_t *affin_list){
    ENTEROMP();
    static __kmpc_omp_reg_task_with_affinity_t __kmpc_omp_reg_task_with_affinity_function = NULL;    if (!__kmpc_omp_reg_task_with_affinity_function)        __kmpc_omp_reg_task_with_affinity_function = (__kmpc_omp_reg_task_with_affinity_t)dlsym(RTLD_NEXT,"__kmpc_omp_reg_task_with_affinity");

    kmp_int32  retval = __kmpc_omp_reg_task_with_affinity_function(loc_ref,gtid,new_task,naffins,affin_list);
    EXITOMP();
    return retval;
}

typedef void ** (*__kmpc_omp_get_target_async_handle_ptr_t)(kmp_int32 gtid);
void ** __kmpc_omp_get_target_async_handle_ptr(kmp_int32 gtid){
    ENTEROMP();
    static __kmpc_omp_get_target_async_handle_ptr_t __kmpc_omp_get_target_async_handle_ptr_function = NULL;    if (!__kmpc_omp_get_target_async_handle_ptr_function)        __kmpc_omp_get_target_async_handle_ptr_function = (__kmpc_omp_get_target_async_handle_ptr_t)dlsym(RTLD_NEXT,"__kmpc_omp_get_target_async_handle_ptr");

    void ** retval = __kmpc_omp_get_target_async_handle_ptr_function(gtid);
    EXITOMP();
    return retval;
}

typedef bool  (*__kmpc_omp_has_task_team_t)(kmp_int32 gtid);
bool  __kmpc_omp_has_task_team(kmp_int32 gtid){
    ENTEROMP();
    static __kmpc_omp_has_task_team_t __kmpc_omp_has_task_team_function = NULL;    if (!__kmpc_omp_has_task_team_function)        __kmpc_omp_has_task_team_function = (__kmpc_omp_has_task_team_t)dlsym(RTLD_NEXT,"__kmpc_omp_has_task_team");

    bool  retval = __kmpc_omp_has_task_team_function(gtid);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_init_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
void  __kmpc_init_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_init_lock_t __kmpc_init_lock_function = NULL;    if (!__kmpc_init_lock_function)        __kmpc_init_lock_function = (__kmpc_init_lock_t)dlsym(RTLD_NEXT,"__kmpc_init_lock");

    __kmpc_init_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef void  (*__kmpc_init_nest_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
void  __kmpc_init_nest_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_init_nest_lock_t __kmpc_init_nest_lock_function = NULL;    if (!__kmpc_init_nest_lock_function)        __kmpc_init_nest_lock_function = (__kmpc_init_nest_lock_t)dlsym(RTLD_NEXT,"__kmpc_init_nest_lock");

    __kmpc_init_nest_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef void  (*__kmpc_destroy_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
void  __kmpc_destroy_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_destroy_lock_t __kmpc_destroy_lock_function = NULL;    if (!__kmpc_destroy_lock_function)        __kmpc_destroy_lock_function = (__kmpc_destroy_lock_t)dlsym(RTLD_NEXT,"__kmpc_destroy_lock");

    __kmpc_destroy_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef void  (*__kmpc_destroy_nest_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
void  __kmpc_destroy_nest_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_destroy_nest_lock_t __kmpc_destroy_nest_lock_function = NULL;    if (!__kmpc_destroy_nest_lock_function)        __kmpc_destroy_nest_lock_function = (__kmpc_destroy_nest_lock_t)dlsym(RTLD_NEXT,"__kmpc_destroy_nest_lock");

    __kmpc_destroy_nest_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef void  (*__kmpc_set_lock_t)(ident_t *loc, kmp_int32 gtid, void **user_lock);
void  __kmpc_set_lock(ident_t *loc, kmp_int32 gtid, void **user_lock){
    ENTEROMP();
    static __kmpc_set_lock_t __kmpc_set_lock_function = NULL;    if (!__kmpc_set_lock_function)        __kmpc_set_lock_function = (__kmpc_set_lock_t)dlsym(RTLD_NEXT,"__kmpc_set_lock");

    __kmpc_set_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef void  (*__kmpc_set_nest_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
void  __kmpc_set_nest_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_set_nest_lock_t __kmpc_set_nest_lock_function = NULL;    if (!__kmpc_set_nest_lock_function)        __kmpc_set_nest_lock_function = (__kmpc_set_nest_lock_t)dlsym(RTLD_NEXT,"__kmpc_set_nest_lock");

    __kmpc_set_nest_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef void  (*__kmpc_unset_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
void  __kmpc_unset_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_unset_lock_t __kmpc_unset_lock_function = NULL;    if (!__kmpc_unset_lock_function)        __kmpc_unset_lock_function = (__kmpc_unset_lock_t)dlsym(RTLD_NEXT,"__kmpc_unset_lock");

    __kmpc_unset_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef void  (*__kmpc_unset_nest_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
void  __kmpc_unset_nest_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_unset_nest_lock_t __kmpc_unset_nest_lock_function = NULL;    if (!__kmpc_unset_nest_lock_function)        __kmpc_unset_nest_lock_function = (__kmpc_unset_nest_lock_t)dlsym(RTLD_NEXT,"__kmpc_unset_nest_lock");

    __kmpc_unset_nest_lock_function(loc,gtid,user_lock);
    EXITOMP();
}

typedef int  (*__kmpc_test_lock_t)(ident_t *loc, kmp_int32 gtid, void **user_lock);
int  __kmpc_test_lock(ident_t *loc, kmp_int32 gtid, void **user_lock){
    ENTEROMP();
    static __kmpc_test_lock_t __kmpc_test_lock_function = NULL;    if (!__kmpc_test_lock_function)        __kmpc_test_lock_function = (__kmpc_test_lock_t)dlsym(RTLD_NEXT,"__kmpc_test_lock");

    int  retval = __kmpc_test_lock_function(loc,gtid,user_lock);
    EXITOMP();
    return retval;
}

typedef int  (*__kmpc_test_nest_lock_t)(ident_t *loc, kmp_int32 gtid,void **user_lock);
int  __kmpc_test_nest_lock(ident_t *loc, kmp_int32 gtid,void **user_lock){
    ENTEROMP();
    static __kmpc_test_nest_lock_t __kmpc_test_nest_lock_function = NULL;    if (!__kmpc_test_nest_lock_function)        __kmpc_test_nest_lock_function = (__kmpc_test_nest_lock_t)dlsym(RTLD_NEXT,"__kmpc_test_nest_lock");

    int  retval = __kmpc_test_nest_lock_function(loc,gtid,user_lock);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_init_lock_with_hint_t)(ident_t *loc, kmp_int32 gtid,void **user_lock, uintptr_t hint);
void  __kmpc_init_lock_with_hint(ident_t *loc, kmp_int32 gtid,void **user_lock, uintptr_t hint){
    ENTEROMP();
    static __kmpc_init_lock_with_hint_t __kmpc_init_lock_with_hint_function = NULL;    if (!__kmpc_init_lock_with_hint_function)        __kmpc_init_lock_with_hint_function = (__kmpc_init_lock_with_hint_t)dlsym(RTLD_NEXT,"__kmpc_init_lock_with_hint");

    __kmpc_init_lock_with_hint_function(loc,gtid,user_lock,hint);
    EXITOMP();
}

typedef void  (*__kmpc_init_nest_lock_with_hint_t)(ident_t *loc, kmp_int32 gtid,void **user_lock,uintptr_t hint);
void  __kmpc_init_nest_lock_with_hint(ident_t *loc, kmp_int32 gtid,void **user_lock,uintptr_t hint){
    ENTEROMP();
    static __kmpc_init_nest_lock_with_hint_t __kmpc_init_nest_lock_with_hint_function = NULL;    if (!__kmpc_init_nest_lock_with_hint_function)        __kmpc_init_nest_lock_with_hint_function = (__kmpc_init_nest_lock_with_hint_t)dlsym(RTLD_NEXT,"__kmpc_init_nest_lock_with_hint");

    __kmpc_init_nest_lock_with_hint_function(loc,gtid,user_lock,hint);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_start_record_task_t)(ident_t *loc, kmp_int32 gtid,kmp_int32 input_flags,kmp_int32 tdg_id);
kmp_int32  __kmpc_start_record_task(ident_t *loc, kmp_int32 gtid,kmp_int32 input_flags,kmp_int32 tdg_id){
    ENTEROMP();
    static __kmpc_start_record_task_t __kmpc_start_record_task_function = NULL;    if (!__kmpc_start_record_task_function)        __kmpc_start_record_task_function = (__kmpc_start_record_task_t)dlsym(RTLD_NEXT,"__kmpc_start_record_task");

    kmp_int32  retval = __kmpc_start_record_task_function(loc,gtid,input_flags,tdg_id);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_record_task_t)(ident_t *loc, kmp_int32 gtid,kmp_int32 input_flags, kmp_int32 tdg_id);
void  __kmpc_end_record_task(ident_t *loc, kmp_int32 gtid,kmp_int32 input_flags, kmp_int32 tdg_id){
    ENTEROMP();
    static __kmpc_end_record_task_t __kmpc_end_record_task_function = NULL;    if (!__kmpc_end_record_task_function)        __kmpc_end_record_task_function = (__kmpc_end_record_task_t)dlsym(RTLD_NEXT,"__kmpc_end_record_task");

    __kmpc_end_record_task_function(loc,gtid,input_flags,tdg_id);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_reduce_nowait_t)(ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t reduce_size,void *reduce_data, kmp_redfun_t reduce_func,kmp_critical_name *lck);
kmp_int32  __kmpc_reduce_nowait(ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t reduce_size,void *reduce_data, kmp_redfun_t reduce_func,kmp_critical_name *lck){
    ENTEROMP();
    static __kmpc_reduce_nowait_t __kmpc_reduce_nowait_function = NULL;    if (!__kmpc_reduce_nowait_function)        __kmpc_reduce_nowait_function = (__kmpc_reduce_nowait_t)dlsym(RTLD_NEXT,"__kmpc_reduce_nowait");

    kmp_int32  retval = __kmpc_reduce_nowait_function(loc,global_tid,num_vars,reduce_size,reduce_data,reduce_func,lck);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_reduce_nowait_t)(ident_t *loc, kmp_int32 global_tid,kmp_critical_name *lck);
void  __kmpc_end_reduce_nowait(ident_t *loc, kmp_int32 global_tid,kmp_critical_name *lck){
    ENTEROMP();
    static __kmpc_end_reduce_nowait_t __kmpc_end_reduce_nowait_function = NULL;    if (!__kmpc_end_reduce_nowait_function)        __kmpc_end_reduce_nowait_function = (__kmpc_end_reduce_nowait_t)dlsym(RTLD_NEXT,"__kmpc_end_reduce_nowait");

    __kmpc_end_reduce_nowait_function(loc,global_tid,lck);
    EXITOMP();
}

typedef kmp_int32  (*__kmpc_reduce_t)(ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t reduce_size,void *reduce_data, kmp_redfun_t reduce_func,kmp_critical_name *lck);
kmp_int32  __kmpc_reduce(ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t reduce_size,void *reduce_data, kmp_redfun_t reduce_func,kmp_critical_name *lck){
    ENTEROMP();
    static __kmpc_reduce_t __kmpc_reduce_function = NULL;    if (!__kmpc_reduce_function)        __kmpc_reduce_function = (__kmpc_reduce_t)dlsym(RTLD_NEXT,"__kmpc_reduce");

    kmp_int32  retval = __kmpc_reduce_function(loc,global_tid,num_vars,reduce_size,reduce_data,reduce_func,lck);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_end_reduce_t)(ident_t *loc, kmp_int32 global_tid,kmp_critical_name *lck);
void  __kmpc_end_reduce(ident_t *loc, kmp_int32 global_tid,kmp_critical_name *lck){
    ENTEROMP();
    static __kmpc_end_reduce_t __kmpc_end_reduce_function = NULL;    if (!__kmpc_end_reduce_function)        __kmpc_end_reduce_function = (__kmpc_end_reduce_t)dlsym(RTLD_NEXT,"__kmpc_end_reduce");

    __kmpc_end_reduce_function(loc,global_tid,lck);
    EXITOMP();
}

typedef kmp_uint64  (*__kmpc_get_taskid_t)();
kmp_uint64  __kmpc_get_taskid(){
    ENTEROMP();
    static __kmpc_get_taskid_t __kmpc_get_taskid_function = NULL;    if (!__kmpc_get_taskid_function)        __kmpc_get_taskid_function = (__kmpc_get_taskid_t)dlsym(RTLD_NEXT,"__kmpc_get_taskid");

    kmp_uint64  retval = __kmpc_get_taskid_function();
    EXITOMP();
    return retval;
}

typedef kmp_uint64  (*__kmpc_get_parent_taskid_t)();
kmp_uint64  __kmpc_get_parent_taskid(){
    ENTEROMP();
    static __kmpc_get_parent_taskid_t __kmpc_get_parent_taskid_function = NULL;    if (!__kmpc_get_parent_taskid_function)        __kmpc_get_parent_taskid_function = (__kmpc_get_parent_taskid_t)dlsym(RTLD_NEXT,"__kmpc_get_parent_taskid");

    kmp_uint64  retval = __kmpc_get_parent_taskid_function();
    EXITOMP();
    return retval;
}

typedef kmp_int32  (*__kmpc_in_parallel_t)(ident_t *loc);
kmp_int32  __kmpc_in_parallel(ident_t *loc){
    ENTEROMP();
    static __kmpc_in_parallel_t __kmpc_in_parallel_function = NULL;    if (!__kmpc_in_parallel_function)        __kmpc_in_parallel_function = (__kmpc_in_parallel_t)dlsym(RTLD_NEXT,"__kmpc_in_parallel");

    kmp_int32  retval = __kmpc_in_parallel_function(loc);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_pop_num_threads_t)(ident_t *loc, kmp_int32 global_tid);
void  __kmpc_pop_num_threads(ident_t *loc, kmp_int32 global_tid){
    ENTEROMP();
    static __kmpc_pop_num_threads_t __kmpc_pop_num_threads_function = NULL;    if (!__kmpc_pop_num_threads_function)        __kmpc_pop_num_threads_function = (__kmpc_pop_num_threads_t)dlsym(RTLD_NEXT,"__kmpc_pop_num_threads");

    __kmpc_pop_num_threads_function(loc,global_tid);
    EXITOMP();
}

typedef void  (*__kmpc_push_num_threads_t)(ident_t *loc, kmp_int32 global_tid,kmp_int32 num_threads);
void  __kmpc_push_num_threads(ident_t *loc, kmp_int32 global_tid,kmp_int32 num_threads){
    ENTEROMP();
    static __kmpc_push_num_threads_t __kmpc_push_num_threads_function = NULL;    if (!__kmpc_push_num_threads_function)        __kmpc_push_num_threads_function = (__kmpc_push_num_threads_t)dlsym(RTLD_NEXT,"__kmpc_push_num_threads");

    __kmpc_push_num_threads_function(loc,global_tid,num_threads);
    EXITOMP();
}

typedef void  (*__kmpc_push_num_threads_strict_t)(ident_t *loc,kmp_int32 global_tid,kmp_int32 num_threads,int severity,const char *message);
void  __kmpc_push_num_threads_strict(ident_t *loc,kmp_int32 global_tid,kmp_int32 num_threads,int severity,const char *message){
    ENTEROMP();
    static __kmpc_push_num_threads_strict_t __kmpc_push_num_threads_strict_function = NULL;    if (!__kmpc_push_num_threads_strict_function)        __kmpc_push_num_threads_strict_function = (__kmpc_push_num_threads_strict_t)dlsym(RTLD_NEXT,"__kmpc_push_num_threads_strict");

    __kmpc_push_num_threads_strict_function(loc,global_tid,num_threads,severity,message);
    EXITOMP();
}

typedef void  (*__kmpc_push_num_threads_list_t)(ident_t *loc, kmp_int32 global_tid,kmp_uint32 list_length,kmp_int32 *num_threads_list);
void  __kmpc_push_num_threads_list(ident_t *loc, kmp_int32 global_tid,kmp_uint32 list_length,kmp_int32 *num_threads_list){
    ENTEROMP();
    static __kmpc_push_num_threads_list_t __kmpc_push_num_threads_list_function = NULL;    if (!__kmpc_push_num_threads_list_function)        __kmpc_push_num_threads_list_function = (__kmpc_push_num_threads_list_t)dlsym(RTLD_NEXT,"__kmpc_push_num_threads_list");

    __kmpc_push_num_threads_list_function(loc,global_tid,list_length,num_threads_list);
    EXITOMP();
}

typedef void  (*__kmpc_push_num_threads_list_strict_t)(ident_t *loc, kmp_int32 global_tid, kmp_uint32 list_length,kmp_int32 *num_threads_list, int severity, const char *message);
void  __kmpc_push_num_threads_list_strict(ident_t *loc, kmp_int32 global_tid, kmp_uint32 list_length,kmp_int32 *num_threads_list, int severity, const char *message){
    ENTEROMP();
    static __kmpc_push_num_threads_list_strict_t __kmpc_push_num_threads_list_strict_function = NULL;    if (!__kmpc_push_num_threads_list_strict_function)        __kmpc_push_num_threads_list_strict_function = (__kmpc_push_num_threads_list_strict_t)dlsym(RTLD_NEXT,"__kmpc_push_num_threads_list_strict");

    __kmpc_push_num_threads_list_strict_function(loc,global_tid,list_length,num_threads_list,severity,message);
    EXITOMP();
}

typedef void  (*__kmpc_push_proc_bind_t)(ident_t *loc, kmp_int32 global_tid,int proc_bind);
void  __kmpc_push_proc_bind(ident_t *loc, kmp_int32 global_tid,int proc_bind){
    ENTEROMP();
    static __kmpc_push_proc_bind_t __kmpc_push_proc_bind_function = NULL;    if (!__kmpc_push_proc_bind_function)        __kmpc_push_proc_bind_function = (__kmpc_push_proc_bind_t)dlsym(RTLD_NEXT,"__kmpc_push_proc_bind");

    __kmpc_push_proc_bind_function(loc,global_tid,proc_bind);
    EXITOMP();
}

typedef void  (*__kmpc_push_num_teams_t)(ident_t *loc, kmp_int32 global_tid,kmp_int32 num_teams,kmp_int32 num_threads);
void  __kmpc_push_num_teams(ident_t *loc, kmp_int32 global_tid,kmp_int32 num_teams,kmp_int32 num_threads){
    ENTEROMP();
    static __kmpc_push_num_teams_t __kmpc_push_num_teams_function = NULL;    if (!__kmpc_push_num_teams_function)        __kmpc_push_num_teams_function = (__kmpc_push_num_teams_t)dlsym(RTLD_NEXT,"__kmpc_push_num_teams");

    __kmpc_push_num_teams_function(loc,global_tid,num_teams,num_threads);
    EXITOMP();
}

typedef void  (*__kmpc_set_thread_limit_t)(ident_t *loc, kmp_int32 global_tid,kmp_int32 thread_limit);
void  __kmpc_set_thread_limit(ident_t *loc, kmp_int32 global_tid,kmp_int32 thread_limit){
    ENTEROMP();
    static __kmpc_set_thread_limit_t __kmpc_set_thread_limit_function = NULL;    if (!__kmpc_set_thread_limit_function)        __kmpc_set_thread_limit_function = (__kmpc_set_thread_limit_t)dlsym(RTLD_NEXT,"__kmpc_set_thread_limit");

    __kmpc_set_thread_limit_function(loc,global_tid,thread_limit);
    EXITOMP();
}

typedef void  (*__kmpc_push_num_teams_51_t)(ident_t *loc, kmp_int32 global_tid,kmp_int32 num_teams_lb,kmp_int32 num_teams_ub,kmp_int32 num_threads);
void  __kmpc_push_num_teams_51(ident_t *loc, kmp_int32 global_tid,kmp_int32 num_teams_lb,kmp_int32 num_teams_ub,kmp_int32 num_threads){
    ENTEROMP();
    static __kmpc_push_num_teams_51_t __kmpc_push_num_teams_51_function = NULL;    if (!__kmpc_push_num_teams_51_function)        __kmpc_push_num_teams_51_function = (__kmpc_push_num_teams_51_t)dlsym(RTLD_NEXT,"__kmpc_push_num_teams_51");

    __kmpc_push_num_teams_51_function(loc,global_tid,num_teams_lb,num_teams_ub,num_threads);
    EXITOMP();
}



typedef void * (*__kmpc_threadprivate_cached_t)(ident_t *loc, kmp_int32 global_tid,void *data, size_t size,void ***cache);
void * __kmpc_threadprivate_cached(ident_t *loc, kmp_int32 global_tid,void *data, size_t size,void ***cache){
    ENTEROMP();
    static __kmpc_threadprivate_cached_t __kmpc_threadprivate_cached_function = NULL;    if (!__kmpc_threadprivate_cached_function)        __kmpc_threadprivate_cached_function = (__kmpc_threadprivate_cached_t)dlsym(RTLD_NEXT,"__kmpc_threadprivate_cached");

    void * retval = __kmpc_threadprivate_cached_function(loc,global_tid,data,size,cache);
    EXITOMP();
    return retval;
}

typedef int  (*__kmpc_get_target_offload_t)();
int  __kmpc_get_target_offload(){
    ENTEROMP();
    static __kmpc_get_target_offload_t __kmpc_get_target_offload_function = NULL;    if (!__kmpc_get_target_offload_function)        __kmpc_get_target_offload_function = (__kmpc_get_target_offload_t)dlsym(RTLD_NEXT,"__kmpc_get_target_offload");

    int  retval = __kmpc_get_target_offload_function();
    EXITOMP();
    return retval;
}

typedef int  (*__kmpc_pause_resource_t)(kmp_pause_status_t level);
int  __kmpc_pause_resource(kmp_pause_status_t level){
    ENTEROMP();
    static __kmpc_pause_resource_t __kmpc_pause_resource_function = NULL;    if (!__kmpc_pause_resource_function)        __kmpc_pause_resource_function = (__kmpc_pause_resource_t)dlsym(RTLD_NEXT,"__kmpc_pause_resource");

    int  retval = __kmpc_pause_resource_function(level);
    EXITOMP();
    return retval;
}

typedef void  (*__kmpc_error_t)(ident_t *loc, int severity, const char *message);
void  __kmpc_error(ident_t *loc, int severity, const char *message){
    ENTEROMP();
    static __kmpc_error_t __kmpc_error_function = NULL;    if (!__kmpc_error_function)        __kmpc_error_function = (__kmpc_error_t)dlsym(RTLD_NEXT,"__kmpc_error");

    __kmpc_error_function(loc,severity,message);
    EXITOMP();
}

typedef void  (*__kmpc_scope_t)(ident_t *loc, kmp_int32 gtid, void *reserved);
void  __kmpc_scope(ident_t *loc, kmp_int32 gtid, void *reserved){
    ENTEROMP();
    static __kmpc_scope_t __kmpc_scope_function = NULL;    if (!__kmpc_scope_function)        __kmpc_scope_function = (__kmpc_scope_t)dlsym(RTLD_NEXT,"__kmpc_scope");

    __kmpc_scope_function(loc,gtid,reserved);
    EXITOMP();
}
#endif

typedef void (*__kmpc_for_static_init_4_t)(ident_t *loc, kmp_int32 gtid,
                                           kmp_int32 schedtype,
                                           kmp_int32 *plastiter,
                                           kmp_int32 *plower, kmp_int32 *pupper,
                                           kmp_int32 *pstride, kmp_int32 incr,
                                           kmp_int32 chunk);
void __kmpc_for_static_init_4(ident_t *loc, kmp_int32 gtid, kmp_int32 schedtype,
                              kmp_int32 *plastiter, kmp_int32 *plower,
                              kmp_int32 *pupper, kmp_int32 *pstride,
                              kmp_int32 incr, kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_for_static_init_4_t __kmpc_for_static_init_4_function = NULL;
  if (!__kmpc_for_static_init_4_function)
    __kmpc_for_static_init_4_function = (__kmpc_for_static_init_4_t)dlsym(
        RTLD_NEXT, "__kmpc_for_static_init_4");

  __kmpc_for_static_init_4_function(loc, gtid, schedtype, plastiter, plower,
                                    pupper, pstride, incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_for_static_init_4u_t)(
    ident_t *loc, kmp_int32 gtid, kmp_int32 schedtype, kmp_int32 *plastiter,
    kmp_uint32 *plower, kmp_uint32 *pupper, kmp_int32 *pstride, kmp_int32 incr,
    kmp_int32 chunk);
void __kmpc_for_static_init_4u(ident_t *loc, kmp_int32 gtid,
                               kmp_int32 schedtype, kmp_int32 *plastiter,
                               kmp_uint32 *plower, kmp_uint32 *pupper,
                               kmp_int32 *pstride, kmp_int32 incr,
                               kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_for_static_init_4u_t __kmpc_for_static_init_4u_function = NULL;
  if (!__kmpc_for_static_init_4u_function)
    __kmpc_for_static_init_4u_function = (__kmpc_for_static_init_4u_t)dlsym(
        RTLD_NEXT, "__kmpc_for_static_init_4u");

  __kmpc_for_static_init_4u_function(loc, gtid, schedtype, plastiter, plower,
                                     pupper, pstride, incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_for_static_init_8_t)(ident_t *loc, kmp_int32 gtid,
                                           kmp_int32 schedtype,
                                           kmp_int32 *plastiter,
                                           kmp_int64 *plower, kmp_int64 *pupper,
                                           kmp_int64 *pstride, kmp_int64 incr,
                                           kmp_int64 chunk);
void __kmpc_for_static_init_8(ident_t *loc, kmp_int32 gtid, kmp_int32 schedtype,
                              kmp_int32 *plastiter, kmp_int64 *plower,
                              kmp_int64 *pupper, kmp_int64 *pstride,
                              kmp_int64 incr, kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_for_static_init_8_t __kmpc_for_static_init_8_function = NULL;
  if (!__kmpc_for_static_init_8_function)
    __kmpc_for_static_init_8_function = (__kmpc_for_static_init_8_t)dlsym(
        RTLD_NEXT, "__kmpc_for_static_init_8");

  __kmpc_for_static_init_8_function(loc, gtid, schedtype, plastiter, plower,
                                    pupper, pstride, incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_for_static_init_8u_t)(
    ident_t *loc, kmp_int32 gtid, kmp_int32 schedtype, kmp_int32 *plastiter,
    kmp_uint64 *plower, kmp_uint64 *pupper, kmp_int64 *pstride, kmp_int64 incr,
    kmp_int64 chunk);
void __kmpc_for_static_init_8u(ident_t *loc, kmp_int32 gtid,
                               kmp_int32 schedtype, kmp_int32 *plastiter,
                               kmp_uint64 *plower, kmp_uint64 *pupper,
                               kmp_int64 *pstride, kmp_int64 incr,
                               kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_for_static_init_8u_t __kmpc_for_static_init_8u_function = NULL;
  if (!__kmpc_for_static_init_8u_function)
    __kmpc_for_static_init_8u_function = (__kmpc_for_static_init_8u_t)dlsym(
        RTLD_NEXT, "__kmpc_for_static_init_8u");

  __kmpc_for_static_init_8u_function(loc, gtid, schedtype, plastiter, plower,
                                     pupper, pstride, incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_dist_for_static_init_4_t)(
    ident_t *loc, kmp_int32 gtid, kmp_int32 schedule, kmp_int32 *plastiter,
    kmp_int32 *plower, kmp_int32 *pupper, kmp_int32 *pupperD,
    kmp_int32 *pstride, kmp_int32 incr, kmp_int32 chunk);
void __kmpc_dist_for_static_init_4(ident_t *loc, kmp_int32 gtid,
                                   kmp_int32 schedule, kmp_int32 *plastiter,
                                   kmp_int32 *plower, kmp_int32 *pupper,
                                   kmp_int32 *pupperD, kmp_int32 *pstride,
                                   kmp_int32 incr, kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_dist_for_static_init_4_t
      __kmpc_dist_for_static_init_4_function = NULL;
  if (!__kmpc_dist_for_static_init_4_function)
    __kmpc_dist_for_static_init_4_function =
        (__kmpc_dist_for_static_init_4_t)dlsym(RTLD_NEXT,
                                               "__kmpc_dist_for_static_init_4");

  __kmpc_dist_for_static_init_4_function(loc, gtid, schedule, plastiter, plower,
                                         pupper, pupperD, pstride, incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_dist_for_static_init_4u_t)(
    ident_t *loc, kmp_int32 gtid, kmp_int32 schedule, kmp_int32 *plastiter,
    kmp_uint32 *plower, kmp_uint32 *pupper, kmp_uint32 *pupperD,
    kmp_int32 *pstride, kmp_int32 incr, kmp_int32 chunk);
void __kmpc_dist_for_static_init_4u(ident_t *loc, kmp_int32 gtid,
                                    kmp_int32 schedule, kmp_int32 *plastiter,
                                    kmp_uint32 *plower, kmp_uint32 *pupper,
                                    kmp_uint32 *pupperD, kmp_int32 *pstride,
                                    kmp_int32 incr, kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_dist_for_static_init_4u_t
      __kmpc_dist_for_static_init_4u_function = NULL;
  if (!__kmpc_dist_for_static_init_4u_function)
    __kmpc_dist_for_static_init_4u_function =
        (__kmpc_dist_for_static_init_4u_t)dlsym(
            RTLD_NEXT, "__kmpc_dist_for_static_init_4u");

  __kmpc_dist_for_static_init_4u_function(loc, gtid, schedule, plastiter,
                                          plower, pupper, pupperD, pstride,
                                          incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_dist_for_static_init_8_t)(
    ident_t *loc, kmp_int32 gtid, kmp_int32 schedule, kmp_int32 *plastiter,
    kmp_int64 *plower, kmp_int64 *pupper, kmp_int64 *pupperD,
    kmp_int64 *pstride, kmp_int64 incr, kmp_int64 chunk);
void __kmpc_dist_for_static_init_8(ident_t *loc, kmp_int32 gtid,
                                   kmp_int32 schedule, kmp_int32 *plastiter,
                                   kmp_int64 *plower, kmp_int64 *pupper,
                                   kmp_int64 *pupperD, kmp_int64 *pstride,
                                   kmp_int64 incr, kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_dist_for_static_init_8_t
      __kmpc_dist_for_static_init_8_function = NULL;
  if (!__kmpc_dist_for_static_init_8_function)
    __kmpc_dist_for_static_init_8_function =
        (__kmpc_dist_for_static_init_8_t)dlsym(RTLD_NEXT,
                                               "__kmpc_dist_for_static_init_8");

  __kmpc_dist_for_static_init_8_function(loc, gtid, schedule, plastiter, plower,
                                         pupper, pupperD, pstride, incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_dist_for_static_init_8u_t)(
    ident_t *loc, kmp_int32 gtid, kmp_int32 schedule, kmp_int32 *plastiter,
    kmp_uint64 *plower, kmp_uint64 *pupper, kmp_uint64 *pupperD,
    kmp_int64 *pstride, kmp_int64 incr, kmp_int64 chunk);
void __kmpc_dist_for_static_init_8u(ident_t *loc, kmp_int32 gtid,
                                    kmp_int32 schedule, kmp_int32 *plastiter,
                                    kmp_uint64 *plower, kmp_uint64 *pupper,
                                    kmp_uint64 *pupperD, kmp_int64 *pstride,
                                    kmp_int64 incr, kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_dist_for_static_init_8u_t
      __kmpc_dist_for_static_init_8u_function = NULL;
  if (!__kmpc_dist_for_static_init_8u_function)
    __kmpc_dist_for_static_init_8u_function =
        (__kmpc_dist_for_static_init_8u_t)dlsym(
            RTLD_NEXT, "__kmpc_dist_for_static_init_8u");

  __kmpc_dist_for_static_init_8u_function(loc, gtid, schedule, plastiter,
                                          plower, pupper, pupperD, pstride,
                                          incr, chunk);
  EXITOMP();
}

typedef void (*__kmpc_team_static_init_4_t)(ident_t *loc, kmp_int32 gtid,
                                            kmp_int32 *p_last, kmp_int32 *p_lb,
                                            kmp_int32 *p_ub, kmp_int32 *p_st,
                                            kmp_int32 incr, kmp_int32 chunk);
void __kmpc_team_static_init_4(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                               kmp_int32 *p_lb, kmp_int32 *p_ub,
                               kmp_int32 *p_st, kmp_int32 incr,
                               kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_team_static_init_4_t __kmpc_team_static_init_4_function = NULL;
  if (!__kmpc_team_static_init_4_function)
    __kmpc_team_static_init_4_function = (__kmpc_team_static_init_4_t)dlsym(
        RTLD_NEXT, "__kmpc_team_static_init_4");

  __kmpc_team_static_init_4_function(loc, gtid, p_last, p_lb, p_ub, p_st, incr,
                                     chunk);
  EXITOMP();
}

typedef void (*__kmpc_team_static_init_4u_t)(ident_t *loc, kmp_int32 gtid,
                                             kmp_int32 *p_last,
                                             kmp_uint32 *p_lb, kmp_uint32 *p_ub,
                                             kmp_int32 *p_st, kmp_int32 incr,
                                             kmp_int32 chunk);
void __kmpc_team_static_init_4u(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                                kmp_uint32 *p_lb, kmp_uint32 *p_ub,
                                kmp_int32 *p_st, kmp_int32 incr,
                                kmp_int32 chunk) {
  ENTEROMP();
  static __kmpc_team_static_init_4u_t __kmpc_team_static_init_4u_function =
      NULL;
  if (!__kmpc_team_static_init_4u_function)
    __kmpc_team_static_init_4u_function = (__kmpc_team_static_init_4u_t)dlsym(
        RTLD_NEXT, "__kmpc_team_static_init_4u");

  __kmpc_team_static_init_4u_function(loc, gtid, p_last, p_lb, p_ub, p_st, incr,
                                      chunk);
  EXITOMP();
}

typedef void (*__kmpc_team_static_init_8_t)(ident_t *loc, kmp_int32 gtid,
                                            kmp_int32 *p_last, kmp_int64 *p_lb,
                                            kmp_int64 *p_ub, kmp_int64 *p_st,
                                            kmp_int64 incr, kmp_int64 chunk);
void __kmpc_team_static_init_8(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                               kmp_int64 *p_lb, kmp_int64 *p_ub,
                               kmp_int64 *p_st, kmp_int64 incr,
                               kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_team_static_init_8_t __kmpc_team_static_init_8_function = NULL;
  if (!__kmpc_team_static_init_8_function)
    __kmpc_team_static_init_8_function = (__kmpc_team_static_init_8_t)dlsym(
        RTLD_NEXT, "__kmpc_team_static_init_8");

  __kmpc_team_static_init_8_function(loc, gtid, p_last, p_lb, p_ub, p_st, incr,
                                     chunk);
  EXITOMP();
}

typedef void (*__kmpc_team_static_init_8u_t)(ident_t *loc, kmp_int32 gtid,
                                             kmp_int32 *p_last,
                                             kmp_uint64 *p_lb, kmp_uint64 *p_ub,
                                             kmp_int64 *p_st, kmp_int64 incr,
                                             kmp_int64 chunk);
void __kmpc_team_static_init_8u(ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last,
                                kmp_uint64 *p_lb, kmp_uint64 *p_ub,
                                kmp_int64 *p_st, kmp_int64 incr,
                                kmp_int64 chunk) {
  ENTEROMP();
  static __kmpc_team_static_init_8u_t __kmpc_team_static_init_8u_function =
      NULL;
  if (!__kmpc_team_static_init_8u_function)
    __kmpc_team_static_init_8u_function = (__kmpc_team_static_init_8u_t)dlsym(
        RTLD_NEXT, "__kmpc_team_static_init_8u");

  __kmpc_team_static_init_8u_function(loc, gtid, p_last, p_lb, p_ub, p_st, incr,
                                      chunk);
  EXITOMP();
}

typedef void (*__kmpc_for_static_fini_t)(ident_t *loc, kmp_int32 global_tid);
void __kmpc_for_static_fini(ident_t *loc, kmp_int32 global_tid) {
  ENTEROMP();
  static __kmpc_for_static_fini_t __kmpc_for_static_fini_function = NULL;
  if (!__kmpc_for_static_fini_function)
    __kmpc_for_static_fini_function =
        (__kmpc_for_static_fini_t)dlsym(RTLD_NEXT, "__kmpc_for_static_fini");

  __kmpc_for_static_fini_function(loc, global_tid);
  EXITOMP();
}

typedef void (*__kmpc_doacross_init_t)(ident_t *loc, kmp_int32 gtid,
                                       kmp_int32 num_dims,
                                       const struct kmp_dim *dims);
void __kmpc_doacross_init(ident_t *loc, kmp_int32 gtid, kmp_int32 num_dims,
                          const struct kmp_dim *dims) {
  ENTEROMP();
  static __kmpc_doacross_init_t __kmpc_doacross_init_function = NULL;
  if (!__kmpc_doacross_init_function)
    __kmpc_doacross_init_function =
        (__kmpc_doacross_init_t)dlsym(RTLD_NEXT, "__kmpc_doacross_init");

  __kmpc_doacross_init_function(loc, gtid, num_dims, dims);
  EXITOMP();
}

typedef void (*__kmpc_doacross_wait_t)(ident_t *loc, kmp_int32 gtid,
                                       const kmp_int64 *vec);
void __kmpc_doacross_wait(ident_t *loc, kmp_int32 gtid, const kmp_int64 *vec) {
  ENTEROMP();
  static __kmpc_doacross_wait_t __kmpc_doacross_wait_function = NULL;
  if (!__kmpc_doacross_wait_function)
    __kmpc_doacross_wait_function =
        (__kmpc_doacross_wait_t)dlsym(RTLD_NEXT, "__kmpc_doacross_wait");

  __kmpc_doacross_wait_function(loc, gtid, vec);
  EXITOMP();
}

typedef void (*__kmpc_doacross_post_t)(ident_t *loc, kmp_int32 gtid,
                                       const kmp_int64 *vec);
void __kmpc_doacross_post(ident_t *loc, kmp_int32 gtid, const kmp_int64 *vec) {
  ENTEROMP();
  static __kmpc_doacross_post_t __kmpc_doacross_post_function = NULL;
  if (!__kmpc_doacross_post_function)
    __kmpc_doacross_post_function =
        (__kmpc_doacross_post_t)dlsym(RTLD_NEXT, "__kmpc_doacross_post");

  __kmpc_doacross_post_function(loc, gtid, vec);
  EXITOMP();
}

typedef void (*__kmpc_doacross_fini_t)(ident_t *loc, kmp_int32 gtid);
void __kmpc_doacross_fini(ident_t *loc, kmp_int32 gtid) {
  ENTEROMP();
  static __kmpc_doacross_fini_t __kmpc_doacross_fini_function = NULL;
  if (!__kmpc_doacross_fini_function)
    __kmpc_doacross_fini_function =
        (__kmpc_doacross_fini_t)dlsym(RTLD_NEXT, "__kmpc_doacross_fini");

  __kmpc_doacross_fini_function(loc, gtid);
  EXITOMP();
}

typedef void (*__kmpc_omp_task_complete_t)(ident_t *loc_ref, kmp_int32 gtid,
                                           kmp_task_t *task);
void __kmpc_omp_task_complete(ident_t *loc_ref, kmp_int32 gtid,
                              kmp_task_t *task) {
  ENTEROMP();
  static __kmpc_omp_task_complete_t __kmpc_omp_task_complete_function = NULL;
  if (!__kmpc_omp_task_complete_function)
    __kmpc_omp_task_complete_function = (__kmpc_omp_task_complete_t)dlsym(
        RTLD_NEXT, "__kmpc_omp_task_complete");

  __kmpc_omp_task_complete_function(loc_ref, gtid, task);
  EXITOMP();
}

typedef kmp_task_t *(*__kmpc_omp_task_alloc_t)(ident_t *loc_ref, kmp_int32 gtid,
                                               kmp_int32 flags,
                                               size_t sizeof_kmp_task_t,
                                               size_t sizeof_shareds,
                                               kmp_routine_entry_t task_entry);
kmp_task_t *__kmpc_omp_task_alloc(ident_t *loc_ref, kmp_int32 gtid,
                                  kmp_int32 flags, size_t sizeof_kmp_task_t,
                                  size_t sizeof_shareds,
                                  kmp_routine_entry_t task_entry) {
  ENTEROMP();
  static __kmpc_omp_task_alloc_t __kmpc_omp_task_alloc_function = NULL;
  if (!__kmpc_omp_task_alloc_function)
    __kmpc_omp_task_alloc_function =
        (__kmpc_omp_task_alloc_t)dlsym(RTLD_NEXT, "__kmpc_omp_task_alloc");

  kmp_task_t *retval = __kmpc_omp_task_alloc_function(
      loc_ref, gtid, flags, sizeof_kmp_task_t, sizeof_shareds, task_entry);
  EXITOMP();
  return retval;
}

typedef kmp_task_t *(*__kmpc_omp_target_task_alloc_t)(
    ident_t *loc_ref, kmp_int32 gtid, kmp_int32 flags, size_t sizeof_kmp_task_t,
    size_t sizeof_shareds, kmp_routine_entry_t task_entry, kmp_int64 device_id);
kmp_task_t *__kmpc_omp_target_task_alloc(ident_t *loc_ref, kmp_int32 gtid,
                                         kmp_int32 flags,
                                         size_t sizeof_kmp_task_t,
                                         size_t sizeof_shareds,
                                         kmp_routine_entry_t task_entry,
                                         kmp_int64 device_id) {
  ENTEROMP();
  static __kmpc_omp_target_task_alloc_t __kmpc_omp_target_task_alloc_function =
      NULL;
  if (!__kmpc_omp_target_task_alloc_function)
    __kmpc_omp_target_task_alloc_function =
        (__kmpc_omp_target_task_alloc_t)dlsym(RTLD_NEXT,
                                              "__kmpc_omp_target_task_alloc");

  kmp_task_t *retval = __kmpc_omp_target_task_alloc_function(
      loc_ref, gtid, flags, sizeof_kmp_task_t, sizeof_shareds, task_entry,
      device_id);
  EXITOMP();
  return retval;
}

typedef kmp_int32 (*__kmpc_omp_task_parts_t)(ident_t *loc_ref, kmp_int32 gtid,
                                             kmp_task_t *new_task);
kmp_int32 __kmpc_omp_task_parts(ident_t *loc_ref, kmp_int32 gtid,
                                kmp_task_t *new_task) {
  ENTEROMP();
  static __kmpc_omp_task_parts_t __kmpc_omp_task_parts_function = NULL;
  if (!__kmpc_omp_task_parts_function)
    __kmpc_omp_task_parts_function =
        (__kmpc_omp_task_parts_t)dlsym(RTLD_NEXT, "__kmpc_omp_task_parts");

  kmp_int32 retval = __kmpc_omp_task_parts_function(loc_ref, gtid, new_task);
  EXITOMP();
  return retval;
}

typedef kmp_int32 (*__kmpc_omp_task_t)(ident_t *loc_ref, kmp_int32 gtid,
                                       kmp_task_t *new_task);
kmp_int32 __kmpc_omp_task(ident_t *loc_ref, kmp_int32 gtid,
                          kmp_task_t *new_task) {
  ENTEROMP();
  static __kmpc_omp_task_t __kmpc_omp_task_function = NULL;
  if (!__kmpc_omp_task_function)
    __kmpc_omp_task_function =
        (__kmpc_omp_task_t)dlsym(RTLD_NEXT, "__kmpc_omp_task");

  kmp_int32 retval = __kmpc_omp_task_function(loc_ref, gtid, new_task);
  EXITOMP();
  return retval;
}

typedef kmp_int32 (*__kmpc_omp_taskyield_t)(ident_t *loc_ref, kmp_int32 gtid,
                                            int end_part);
kmp_int32 __kmpc_omp_taskyield(ident_t *loc_ref, kmp_int32 gtid, int end_part) {
  ENTEROMP();
  static __kmpc_omp_taskyield_t __kmpc_omp_taskyield_function = NULL;
  if (!__kmpc_omp_taskyield_function)
    __kmpc_omp_taskyield_function =
        (__kmpc_omp_taskyield_t)dlsym(RTLD_NEXT, "__kmpc_omp_taskyield");

  kmp_int32 retval = __kmpc_omp_taskyield_function(loc_ref, gtid, end_part);
  EXITOMP();
  return retval;
}

typedef void *(*__kmpc_task_reduction_get_th_data_t)(int gtid, void *tskgrp,
                                                     void *data);
void *__kmpc_task_reduction_get_th_data(int gtid, void *tskgrp, void *data) {
  ENTEROMP();
  static __kmpc_task_reduction_get_th_data_t
      __kmpc_task_reduction_get_th_data_function = NULL;
  if (!__kmpc_task_reduction_get_th_data_function)
    __kmpc_task_reduction_get_th_data_function =
        (__kmpc_task_reduction_get_th_data_t)dlsym(
            RTLD_NEXT, "__kmpc_task_reduction_get_th_data");

  void *retval = __kmpc_task_reduction_get_th_data_function(gtid, tskgrp, data);
  EXITOMP();
  return retval;
}

typedef void *(*__kmpc_task_reduction_modifier_init_t)(ident_t *loc, int gtid,
                                                       int is_ws, int num,
                                                       void *data);
void *__kmpc_task_reduction_modifier_init(ident_t *loc, int gtid, int is_ws,
                                          int num, void *data) {
  ENTEROMP();
  static __kmpc_task_reduction_modifier_init_t
      __kmpc_task_reduction_modifier_init_function = NULL;
  if (!__kmpc_task_reduction_modifier_init_function)
    __kmpc_task_reduction_modifier_init_function =
        (__kmpc_task_reduction_modifier_init_t)dlsym(
            RTLD_NEXT, "__kmpc_task_reduction_modifier_init");

  void *retval =
      __kmpc_task_reduction_modifier_init_function(loc, gtid, is_ws, num, data);
  EXITOMP();
  return retval;
}

typedef void *(*__kmpc_taskred_modifier_init_t)(ident_t *loc, int gtid,
                                                int is_ws, int num, void *data);
void *__kmpc_taskred_modifier_init(ident_t *loc, int gtid, int is_ws, int num,
                                   void *data) {
  ENTEROMP();
  static __kmpc_taskred_modifier_init_t __kmpc_taskred_modifier_init_function =
      NULL;
  if (!__kmpc_taskred_modifier_init_function)
    __kmpc_taskred_modifier_init_function =
        (__kmpc_taskred_modifier_init_t)dlsym(RTLD_NEXT,
                                              "__kmpc_taskred_modifier_init");

  void *retval =
      __kmpc_taskred_modifier_init_function(loc, gtid, is_ws, num, data);
  EXITOMP();
  return retval;
}

typedef void (*__kmpc_taskgroup_t)(ident_t *loc, int gtid);
void __kmpc_taskgroup(ident_t *loc, int gtid) {
  ENTEROMP();
  static __kmpc_taskgroup_t __kmpc_taskgroup_function = NULL;
  if (!__kmpc_taskgroup_function)
    __kmpc_taskgroup_function =
        (__kmpc_taskgroup_t)dlsym(RTLD_NEXT, "__kmpc_taskgroup");

  __kmpc_taskgroup_function(loc, gtid);
  EXITOMP();
}

typedef void (*__kmpc_end_taskgroup_t)(ident_t *loc, int gtid);
void __kmpc_end_taskgroup(ident_t *loc, int gtid) {
  ENTEROMP();
  static __kmpc_end_taskgroup_t __kmpc_end_taskgroup_function = NULL;
  if (!__kmpc_end_taskgroup_function)
    __kmpc_end_taskgroup_function =
        (__kmpc_end_taskgroup_t)dlsym(RTLD_NEXT, "__kmpc_end_taskgroup");

  __kmpc_end_taskgroup_function(loc, gtid);
  EXITOMP();
}

typedef void (*__kmpc_proxy_task_completed_t)(kmp_int32 gtid,
                                              kmp_task_t *ptask);
void __kmpc_proxy_task_completed(kmp_int32 gtid, kmp_task_t *ptask) {
  ENTEROMP();
  static __kmpc_proxy_task_completed_t __kmpc_proxy_task_completed_function =
      NULL;
  if (!__kmpc_proxy_task_completed_function)
    __kmpc_proxy_task_completed_function = (__kmpc_proxy_task_completed_t)dlsym(
        RTLD_NEXT, "__kmpc_proxy_task_completed");

  __kmpc_proxy_task_completed_function(gtid, ptask);
  EXITOMP();
}

typedef void (*__kmpc_proxy_task_completed_ooo_t)(kmp_task_t *ptask);
void __kmpc_proxy_task_completed_ooo(kmp_task_t *ptask) {
  ENTEROMP();
  static __kmpc_proxy_task_completed_ooo_t
      __kmpc_proxy_task_completed_ooo_function = NULL;
  if (!__kmpc_proxy_task_completed_ooo_function)
    __kmpc_proxy_task_completed_ooo_function =
        (__kmpc_proxy_task_completed_ooo_t)dlsym(
            RTLD_NEXT, "__kmpc_proxy_task_completed_ooo");

  __kmpc_proxy_task_completed_ooo_function(ptask);
  EXITOMP();
}

typedef kmp_event_t *(*__kmpc_task_allow_completion_event_t)(ident_t *loc_ref,
                                                             int gtid,
                                                             kmp_task_t *task);
kmp_event_t *__kmpc_task_allow_completion_event(ident_t *loc_ref, int gtid,
                                                kmp_task_t *task) {
  ENTEROMP();
  static __kmpc_task_allow_completion_event_t
      __kmpc_task_allow_completion_event_function = NULL;
  if (!__kmpc_task_allow_completion_event_function)
    __kmpc_task_allow_completion_event_function =
        (__kmpc_task_allow_completion_event_t)dlsym(
            RTLD_NEXT, "__kmpc_task_allow_completion_event");

  kmp_event_t *retval =
      __kmpc_task_allow_completion_event_function(loc_ref, gtid, task);
  EXITOMP();
  return retval;
}

typedef void (*__kmpc_taskloop_t)(ident_t *loc, int gtid, kmp_task_t *task,
                                  int if_val, kmp_uint64 *lb, kmp_uint64 *ub,
                                  kmp_int64 st, int nogroup, int sched,
                                  kmp_uint64 grainsize, void *task_dup);
void __kmpc_taskloop(ident_t *loc, int gtid, kmp_task_t *task, int if_val,
                     kmp_uint64 *lb, kmp_uint64 *ub, kmp_int64 st, int nogroup,
                     int sched, kmp_uint64 grainsize, void *task_dup) {
  ENTEROMP();
  static __kmpc_taskloop_t __kmpc_taskloop_function = NULL;
  if (!__kmpc_taskloop_function)
    __kmpc_taskloop_function =
        (__kmpc_taskloop_t)dlsym(RTLD_NEXT, "__kmpc_taskloop");

  __kmpc_taskloop_function(loc, gtid, task, if_val, lb, ub, st, nogroup, sched,
                           grainsize, task_dup);
  EXITOMP();
}

typedef void (*__kmpc_taskloop_5_t)(ident_t *loc, int gtid, kmp_task_t *task,
                                    int if_val, kmp_uint64 *lb, kmp_uint64 *ub,
                                    kmp_int64 st, int nogroup, int sched,
                                    kmp_uint64 grainsize, int modifier,
                                    void *task_dup);
void __kmpc_taskloop_5(ident_t *loc, int gtid, kmp_task_t *task, int if_val,
                       kmp_uint64 *lb, kmp_uint64 *ub, kmp_int64 st,
                       int nogroup, int sched, kmp_uint64 grainsize,
                       int modifier, void *task_dup) {
  ENTEROMP();
  static __kmpc_taskloop_5_t __kmpc_taskloop_5_function = NULL;
  if (!__kmpc_taskloop_5_function)
    __kmpc_taskloop_5_function =
        (__kmpc_taskloop_5_t)dlsym(RTLD_NEXT, "__kmpc_taskloop_5");

  __kmpc_taskloop_5_function(loc, gtid, task, if_val, lb, ub, st, nogroup,
                             sched, grainsize, modifier, task_dup);
  EXITOMP();
}

typedef void **(*__kmpc_omp_get_target_async_handle_ptr_t)(kmp_int32 gtid);
void **__kmpc_omp_get_target_async_handle_ptr(kmp_int32 gtid) {
  ENTEROMP();
  static __kmpc_omp_get_target_async_handle_ptr_t
      __kmpc_omp_get_target_async_handle_ptr_function = NULL;
  if (!__kmpc_omp_get_target_async_handle_ptr_function)
    __kmpc_omp_get_target_async_handle_ptr_function =
        (__kmpc_omp_get_target_async_handle_ptr_t)dlsym(
            RTLD_NEXT, "__kmpc_omp_get_target_async_handle_ptr");

  void **retval = __kmpc_omp_get_target_async_handle_ptr_function(gtid);
  EXITOMP();
  return retval;
}

typedef void (*__kmpc_end_record_task_t)(ident_t *loc_ref, kmp_int32 gtid,
                                         kmp_int32 input_flags,
                                         kmp_int32 tdg_id);
void __kmpc_end_record_task(ident_t *loc_ref, kmp_int32 gtid,
                            kmp_int32 input_flags, kmp_int32 tdg_id) {
  ENTEROMP();
  static __kmpc_end_record_task_t __kmpc_end_record_task_function = NULL;
  if (!__kmpc_end_record_task_function)
    __kmpc_end_record_task_function =
        (__kmpc_end_record_task_t)dlsym(RTLD_NEXT, "__kmpc_end_record_task");

  __kmpc_end_record_task_function(loc_ref, gtid, input_flags, tdg_id);
  EXITOMP();
}

typedef kmp_int32 (*__kmpc_omp_task_with_deps_t)(
    ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *new_task, kmp_int32 ndeps,
    kmp_depend_info_t *dep_list, kmp_int32 ndeps_noalias,
    kmp_depend_info_t *noalias_dep_list);
kmp_int32 __kmpc_omp_task_with_deps(ident_t *loc_ref, kmp_int32 gtid,
                                    kmp_task_t *new_task, kmp_int32 ndeps,
                                    kmp_depend_info_t *dep_list,
                                    kmp_int32 ndeps_noalias,
                                    kmp_depend_info_t *noalias_dep_list) {
  ENTEROMP();
  static __kmpc_omp_task_with_deps_t __kmpc_omp_task_with_deps_function = NULL;
  if (!__kmpc_omp_task_with_deps_function)
    __kmpc_omp_task_with_deps_function = (__kmpc_omp_task_with_deps_t)dlsym(
        RTLD_NEXT, "__kmpc_omp_task_with_deps");

  kmp_int32 retval = __kmpc_omp_task_with_deps_function(
      loc_ref, gtid, new_task, ndeps, dep_list, ndeps_noalias,
      noalias_dep_list);
  EXITOMP();
  return retval;
}
