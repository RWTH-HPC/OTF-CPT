#include <assert.h>
#include <mpi.h>
#include <stdio.h>

#ifndef NOTOOL
#include "tracking.h"

int MPI_Group_size(MPI_Group group, int *size) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Group_size(myGrp, size);
  return ret;
}
int MPI_Group_rank(MPI_Group group, int *rank) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Group_rank(myGrp, rank);
  return ret;
}
int MPI_Group_translate_ranks(MPI_Group group1, int n, const int ranks1[],
                              MPI_Group group2, int ranks2[]) {
  MPI_Group myGrp1 = gf.getHandlePool()->getHandle(group1);
  MPI_Group myGrp2 = gf.getHandlePool()->getHandle(group2);
  int ret = PMPI_Group_translate_ranks(myGrp1, n, ranks1, myGrp2, ranks2);
  return ret;
}
int MPI_Group_compare(MPI_Group group1, MPI_Group group2, int *result) {
  MPI_Group myGrp1 = gf.getHandlePool()->getHandle(group1);
  MPI_Group myGrp2 = gf.getHandlePool()->getHandle(group2);
  int ret = PMPI_Group_compare(myGrp1, myGrp2, result);
  return ret;
}
int MPI_Comm_create(MPI_Comm comm, MPI_Group group, MPI_Comm *newcomm) {
  MPI_Comm myComm = cf.getHandlePool()->getHandle(comm);
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Comm_create(myComm, myGrp, newcomm);
  *newcomm = cf.getHandlePool()->newHandle(*newcomm);
  return ret;
}
int MPI_Win_post(MPI_Group group, int assert, MPI_Win win) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Win_post(myGrp, assert, win);
  return ret;
}
int MPI_Win_start(MPI_Group group, int assert, MPI_Win win) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Win_start(myGrp, assert, win);
  return ret;
}
int MPI_Comm_create_group(MPI_Comm comm, MPI_Group group, int tag,
                          MPI_Comm *newcomm) {
  MPI_Comm myComm = cf.getHandlePool()->getHandle(comm);
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Comm_create_group(myComm, myGrp, tag, newcomm);
  *newcomm = cf.getHandlePool()->newHandle(*newcomm);
  return ret;
}

int MPI_Comm_group(MPI_Comm comm, MPI_Group *group) {
  MPI_Comm myComm = cf.getHandlePool()->getHandle(comm);
  int ret = PMPI_Comm_group(myComm, group);
  *group = gf.getHandlePool()->newHandle(*group);
  return ret;
}
int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) {
  MPI_Group myGrp1 = gf.getHandlePool()->getHandle(group1);
  MPI_Group myGrp2 = gf.getHandlePool()->getHandle(group2);
  int ret = PMPI_Group_union(myGrp1, myGrp2, newgroup);
  *newgroup = gf.getHandlePool()->newHandle(*newgroup);
  return ret;
}
int MPI_Group_intersection(MPI_Group group1, MPI_Group group2,
                           MPI_Group *newgroup) {
  MPI_Group myGrp1 = gf.getHandlePool()->getHandle(group1);
  MPI_Group myGrp2 = gf.getHandlePool()->getHandle(group2);
  int ret = PMPI_Group_intersection(myGrp1, myGrp2, newgroup);
  *newgroup = gf.getHandlePool()->newHandle(*newgroup);
  return ret;
}
int MPI_Group_difference(MPI_Group group1, MPI_Group group2,
                         MPI_Group *newgroup) {
  MPI_Group myGrp1 = gf.getHandlePool()->getHandle(group1);
  MPI_Group myGrp2 = gf.getHandlePool()->getHandle(group2);
  int ret = PMPI_Group_difference(myGrp1, myGrp2, newgroup);
  *newgroup = gf.getHandlePool()->newHandle(*newgroup);
  return ret;
}
int MPI_Group_incl(MPI_Group group, int n, const int ranks[],
                   MPI_Group *newgroup) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Group_incl(group, n, ranks, newgroup);
  *newgroup = gf.getHandlePool()->newHandle(*newgroup);
  return ret;
}
int MPI_Group_excl(MPI_Group group, int n, const int ranks[],
                   MPI_Group *newgroup) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Group_excl(group, n, ranks, newgroup);
  *newgroup = gf.getHandlePool()->newHandle(*newgroup);
  return ret;
}
int MPI_Group_range_incl(MPI_Group group, int n, int ranges[][3],
                         MPI_Group *newgroup) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Group_range_incl(group, n, ranges, newgroup);
  *newgroup = gf.getHandlePool()->newHandle(*newgroup);
  return ret;
}
int MPI_Group_range_excl(MPI_Group group, int n, int ranges[][3],
                         MPI_Group *newgroup) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(group);
  int ret = PMPI_Group_range_excl(group, n, ranges, newgroup);
  *newgroup = gf.getHandlePool()->newHandle(*newgroup);
  return ret;
}
int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group) {
  MPI_Comm myComm = cf.getHandlePool()->getHandle(comm);
  int ret = PMPI_Comm_remote_group(myComm, group);
  *group = gf.getHandlePool()->newHandle(*group);
  return ret;
}
int MPI_Win_get_group(MPI_Win win, MPI_Group *group) {
  int ret = PMPI_Win_get_group(win, group);
  *group = gf.getHandlePool()->newHandle(*group);
  return ret;
}

int MPI_Group_free(MPI_Group *group) {
  MPI_Group myGrp = gf.getHandlePool()->getHandle(*group);
  *group = gf.getHandlePool()->freeHandle(*group);
  int ret = PMPI_Group_free(&myGrp);
  return ret;
}

#endif