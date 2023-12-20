#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sum = 0;
#pragma omp parallel for ordered schedule(static, 1)
  for (int i = 0; i < 100; i++) {
    // usleep(20000);
#pragma omp ordered
    {
      sum += i;
      usleep(10000);
    }
    // usleep(20000);
  }

  printf("sum = %i\n", sum);
}
