#include <assert.h>
#include <mpi.h>
#include <stdio.h>

#include "tracking.h"
#include "mpi-critical.h"

#define mpiSendPBMacro(c)   mpiSendPB pb{dest, tag, c};
#define mpiIsendPBMacro(c,r)  mpiIsendPB pb{dest, tag, c, r};
#define mpiSendInitPBMacro(c,r)  mpiSendInitPB pb{dest, tag, c, r};
#define mpiRecvPBMacro(c,s)  mpiRecvPB pb{source, tag, c, &s};
#define mpiIrecvPBMacro(c,r)  mpiIrecvPB pb{source, tag, c, r};
#define mpiMprobePBMacro(c,s,m)  mpiMprobePB pb{source, tag, c, &s, m};
#define mpiImprobePBMacro(c,s,m)  mpiMprobePB pb{source, tag, flag, c, &s, m};
#define mpiMrecvPBMacro(m,s)  mpiRecvPB pb{m, &s};
#define mpiImrecvPBMacro(m,r)  mpiIrecvPB pb{m, r};
#define mpiRecvInitPBMacro(c,r)  mpiRecvInitPB pb{source, tag, c, r};

#ifdef HAVE_ISRR
#define mpiSendrecvPBMacro(c,s)  mpiSendrecvPB pb{dest, source, sendtag, recvtag, c, &s};
#define mpiIsendrecvPBMacro(c,r)  mpiSendrecvPB pb{dest, source, sendtag, recvtag, c, r};
#else
#define mpiSendrecvPBMacro(c,s)
#define mpiIsendrecvPBMacro(c,r)
#endif

{{fn fn_name MPI_Bsend MPI_Rsend MPI_Ssend MPI_Send MPI_Bsend_c MPI_Rsend_c MPI_Ssend_c MPI_Send_c }}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->send++;

  {{apply_to_type MPI_Comm 'mpiSendPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Ibsend MPI_Irsend MPI_Issend MPI_Isend MPI_Ibsend_c MPI_Irsend_c MPI_Issend_c MPI_Isend_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->isend++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiIsendPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Bsend_init MPI_Rsend_init MPI_Ssend_init MPI_Send_init MPI_Bsend_init_c MPI_Rsend_init_c MPI_Ssend_init_c MPI_Send_init_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->isend++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiSendInitPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Recv MPI_Recv_c }}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->recv++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Status*'}} 'mpiRecvPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Irecv MPI_Irecv_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->irecv++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiIrecvPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Sendrecv MPI_Sendrecv_c MPI_Sendrecv_replace MPI_Sendrecv_replace_c }}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->recv++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Status*'}} 'mpiSendrecvPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Isendrecv MPI_Isendrecv_c MPI_Isendrecv_replace MPI_Isendrecv_replace_c }}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->irecv++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiIsendrecvPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Mprobe MPI_Mprobe_c }}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->probe++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Status*' 'MPI_Message*'}} 'mpiMprobePBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Improbe MPI_Improbe_c }}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->probe++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Status*' 'MPI_Message*'}} 'mpiImprobePBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Recv_init MPI_Recv_init_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->irecv++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiRecvInitPBMacro'}}
  {{apply_to_type MPI_Comm preComm}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Mrecv MPI_Mrecv_c }}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->recv++;

  {{apply_to_type {{list 'MPI_Message*' 'MPI_Status*'}} 'mpiMrecvPBMacro'}}
  {{apply_to_type MPI_Message preMessage}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

{{fn fn_name MPI_Imrecv MPI_Imrecv_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->irecv++;

  {{apply_to_type {{list 'MPI_Message*' 'MPI_Request*'}} 'mpiImrecvPBMacro'}}
  {{apply_to_type MPI_Message preMessage}}
   
  {{ret_val}} = P{{fn_name}}({{args}});
{{endfn}}

#define mpiCollAllreduceMacro(c) mpiCollAllreducePB pb{c}
#define mpiIcollAllreduceMacro(c,r) mpiIcollAllreducePB pb{c,r}
#define mpiCollAllreduceInitMacro(c,r) mpiCollAllreduceInitPB pb{c,r}
#define mpiCollReduceMacro(c) mpiCollReducePB pb{c,root}
#define mpiIcollReduceMacro(c,r) mpiIcollReducePB pb{c,r,root}
#define mpiCollReduceInitMacro(c,r) mpiCollReduceInitPB pb{c,r,root}
#define mpiCollBcastMacro(c) mpiCollBcastPB pb{c,root}
#define mpiIcollBcastMacro(c,r) mpiIcollBcastPB pb{c,r,root}
#define mpiCollBcastInitMacro(c,r) mpiCollBcastInitPB pb{c,r,root}

{{fn fn_name MPI_Barrier MPI_Allgather MPI_Allgather_c MPI_Allgatherv MPI_Allgatherv_c MPI_Alltoall MPI_Alltoall_c MPI_Alltoallv MPI_Alltoallv_c MPI_Allreduce MPI_Allreduce_c MPI_Reduce_scatter_block MPI_Reduce_scatter_block_c MPI_Reduce_scatter MPI_Reduce_scatter_c MPI_Scan MPI_Scan_c MPI_Exscan MPI_Exscan_c MPI_Neighbor_allgather MPI_Neighbor_allgather_c MPI_Neighbor_allgatherv MPI_Neighbor_allgatherv_c MPI_Neighbor_alltoall MPI_Neighbor_alltoall_c MPI_Neighbor_alltoallv MPI_Neighbor_alltoallv_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->coll++;

  {{apply_to_type MPI_Comm 'mpiCollAllreduceMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Ibarrier MPI_Iallgather  MPI_Iallgather_c MPI_Iallgatherv MPI_Iallgatherv_c MPI_Ialltoall MPI_Ialltoall_c MPI_Ialltoallv MPI_Ialltoallv_c MPI_Iallreduce MPI_Iallreduce_c MPI_Ireduce_scatter_block MPI_Ireduce_scatter_block_c MPI_Ireduce_scatter MPI_Ireduce_scatter_c MPI_Iscan MPI_Iscan_c MPI_Iexscan MPI_Iexscan_c MPI_Ineighbor_allgather MPI_Ineighbor_allgather_c MPI_Ineighbor_allgatherv MPI_Ineighbor_allgatherv_c MPI_Ineighbor_alltoall MPI_Ineighbor_alltoall_c MPI_Ineighbor_alltoallv MPI_Ineighbor_alltoallv_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiIcollAllreduceMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}


{{fn fn_name MPI_Barrier_init MPI_Allgather_init MPI_Allgather_init_c MPI_Allgatherv_init MPI_Allgatherv_init_c MPI_Alltoall_init MPI_Alltoall_init_c MPI_Alltoallv_init MPI_Alltoallv_init_c MPI_Allreduce_init MPI_Allreduce_init_c MPI_Reduce_scatter_block_init MPI_Reduce_scatter_block_init_c MPI_Reduce_scatter_init MPI_Reduce_scatter_init_c MPI_Scan_init MPI_Scan_init_c MPI_Exscan_init MPI_Exscan_init_c MPI_Neighbor_allgather_init MPI_Neighbor_allgatherv_init MPI_Neighbor_alltoall_init MPI_Neighbor_alltoallv_init}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiCollAllreduceInitMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}


{{fn fn_name MPI_Gather MPI_Gather_c MPI_Gatherv MPI_Gatherv_c MPI_Reduce MPI_Reduce_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->coll++;

  {{apply_to_type MPI_Comm 'mpiCollReduceMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Igather MPI_Igather_c MPI_Igatherv MPI_Igatherv_c MPI_Ireduce MPI_Ireduce_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiIcollReduceMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Gather_init MPI_Gather_init_c MPI_Gatherv_init MPI_Gatherv_init_c MPI_Reduce_init MPI_Reduce_init_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiCollReduceInitMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}


{{fn fn_name MPI_Bcast MPI_Bcast_c MPI_Scatter MPI_Scatter_c MPI_Scatterv MPI_Scatterv_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->coll++;

  {{apply_to_type MPI_Comm 'mpiCollBcastMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Ibcast MPI_Ibcast_c MPI_Iscatter MPI_Iscatter_c MPI_Iscatterv MPI_Iscatterv_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiIcollBcastMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Bcast_init MPI_Bcast_init_c MPI_Scatter_init MPI_Scatter_init_c MPI_Scatterv_init MPI_Scatterv_init_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiCollBcastInitMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Alltoallw MPI_Alltoallw_c MPI_Neighbor_alltoallw MPI_Neighbor_alltoallw_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->coll++;

  {{apply_to_type MPI_Comm 'mpiCollAllreduceMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Ialltoallw MPI_Ialltoallw_c MPI_Ineighbor_alltoallw MPI_Ineighbor_alltoallw_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiIcollAllreduceMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}

{{fn fn_name MPI_Alltoallw_init MPI_Alltoallw_init_c MPI_Neighbor_alltoallw_init MPI_Neighbor_alltoallw_init_c}}
  mpiTimer mt{false, __func__};
  if (analysis_flags->running)
    thread_local_clock->icoll++;

  {{apply_to_type {{list 'MPI_Comm' 'MPI_Request*'}} 'mpiCollAllreduceInitMacro'}}
   {{apply_to_type MPI_Comm preComm}}
   {{ret_val}} = P{{fn_name}}({{args}});
   {{apply_to_type MPI_Comm* postComm}}
{{endfn}}
