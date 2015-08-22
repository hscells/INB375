#include <stdlib.h>
#include <stdio.h>

//  MAC OSX specific 
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <pthread.h>

#define N 2000
#define NUM_THREADS 10

#define min(X, Y) (((X) < (Y)) ? (X) : (Y));

int* A;
int* B;
int* C;

double get_time() {
  static double timeConvert = 0.0;
  if ( timeConvert == 0.0 ) {
    mach_timebase_info_data_t timeBase;
    (void)mach_timebase_info(&timeBase);
    timeConvert = (double)timeBase.numer / (double)timeBase.denom / 1000000000.0;
  }
  return (double)mach_absolute_time() * timeConvert;
}

void* matrix_multiply(void* arg) {
  int i, j ,k;
  long id = (long)arg;
  long chunksize = (N+NUM_THREADS-1)/NUM_THREADS;
  long lowerbound = id*chunksize;
  long upperbound = min(lowerbound + chunksize, N);
  printf("id: %lu\n",id);
  for(i = lowerbound; i < upperbound; i++) {
    for(j = 0; j < N; j++) {
      C[i, j] = 0;
      for(k = 0; k < N; k++) {
        C[i, j] += A[i, k] * B[k, j];
      }
    }
  }
  return NULL;
}

int main(int argc, char const *argv[]) {

  srand(42);

  long i, j;

  double starttime;
  double endtime;

  A = (int*) malloc(N * N * sizeof(int));
  B = (int*) malloc(N * N * sizeof(int));
  C = (int*) malloc(N * N * sizeof(int));

  pthread_t threads[NUM_THREADS];

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[N*i+j] = rand();
      B[N*i+j] = rand();
    }
  }

  starttime = get_time();
  for (i = 0; i < NUM_THREADS; i++) {
      pthread_create(&threads[i], NULL, matrix_multiply, (void*) i);
  }
  for (i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
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
