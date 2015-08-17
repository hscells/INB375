#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

//  MAC OSX specific 
#include <mach/mach.h>
#include <mach/mach_time.h>

#define N 1000
#define NUM_THREADS 1

int* C;
pthread_mutex_t lock;

typedef struct {
  int* A;
  int* B;
  int size;
  int index;
} matrix_t;

double get_time() {
  static double timeConvert = 0.0;
  if ( timeConvert == 0.0 ) {
    mach_timebase_info_data_t timeBase;
    (void)mach_timebase_info(&timeBase);
    timeConvert = (double)timeBase.numer / (double)timeBase.denom / 1000000000.0;
  }
  return (double)mach_absolute_time() * timeConvert;
}

void* matrixMultiply(void* arg) {
  int i, j;

  matrix_t* M= (matrix_t*) arg;

  for (i = 0; i < M->size; i++) {
    for (j = 0; j < M->size; j++) {
      pthread_mutex_lock(&lock);
      C[(M->size*M->index)*i+j] += M->A[i] * M->B[j];
      pthread_mutex_unlock(&lock);
    }
  }

  free(M->A);
  free(M->B);
  free(M);
  return NULL;
}

matrix_t* chunk(int* A, int* B, int index, int num_chunks) {
  int i, j;
  matrix_t* M = (matrix_t*) malloc(sizeof(matrix_t));

  // this is bad atm because it will split uneavenly
  int chunk_size = (N*index+N)/num_chunks - (N*index)/num_chunks;

  M->size = chunk_size;
  M->index = index;
  printf("%d\n", chunk_size);
  M->A = (int*) malloc(chunk_size * sizeof(int));
  M->B = (int*) malloc(chunk_size * sizeof(int));
  for(i = 0; i < chunk_size; i++) {
    M->A[i] = A[N*index+i];
    M->B[i] = B[N*index+i];
  }
  return M;
}

int main(int argc, char const *argv[]) {

  srand(42);

  int i, j;

  double starttime;
  double endtime;

  int* A = (int*) malloc(N * N * sizeof(int));
  int* B = (int*) malloc(N * N * sizeof(int));
  C = (int*) malloc(N * N * sizeof(int));

  pthread_t threads[NUM_THREADS];

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[N*i+j] = rand();
      B[N*i+j] = rand();
      C[N*i+j] = 0;
    }
  }

  starttime = get_time();
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads[i], NULL, matrixMultiply, (void*)chunk(A, B, i, NUM_THREADS));
  }
  for (i = 0; i < NUM_THREADS; ++i) {
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
