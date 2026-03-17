#ifndef IPC_DATA_H
#define IPC_DATA_H 1

#include <cstdint>
#define NUM_UC_VALUES 4
#define NUM_UC_DOUBLE 1
#define NUM_UC_INT64 0

struct depMetric {
  double fvalues[NUM_UC_DOUBLE];
  int64_t ivalues[NUM_UC_INT64];
};

#if NUM_UC_INT64 > 0
using ipcMetric = depMetric;
#else
using ipcMetric = double;
#endif

#endif