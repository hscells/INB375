#include <stdlib.h>
#include <stdio.h>

//  MAC OSX specific 
#include <mach/mach.h>
#include <mach/mach_time.h>

#define N 1000

double get_time() {
  static double timeConvert = 0.0;
  if ( timeConvert == 0.0 ) {
    mach_timebase_info_data_t timeBase;
    (void)mach_timebase_info(&timeBase);
    timeConvert = (double)timeBase.numer / (double)timeBase.denom / 1000000000.0;
  }
  return (double)mach_absolute_time() * timeConvert;
}

int main(int argc, char const *argv[]) {

  srand(42);

  int i, j, k;

  double starttime;
  double endtime;

  int* A = (int*) malloc(N * N * sizeof(int));
  int* B = (int*) malloc(N * N * sizeof(int));
  int* C = (int*) malloc(N * N * sizeof(int));

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[N*i+j] = rand();
      B[N*i+j] = rand();
    }
  }

  starttime = get_time();
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      C[N*i+j] = 0;
      for (k = 0; k < N; k++) {
        C[N*i+j] += A[N*i+k] * B[N*k+j];
      }
    }
  }

  endtime = get_time();
  printf("%f\n",endtime-starttime);
  printf("%d\n",C[0]);
  printf("%d\n",C[N*5+5]);
  printf("%d\n",C[N*0+500]);
  printf("%d\n",C[N*500+0]);
  printf("%d\n",C[N*500+500]);

  free(A);
  free(B);
  free(C);
  return 0;
}
