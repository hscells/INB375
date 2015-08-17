using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace MatrixMultiply
{
    class Program
    {

        const int NUM_THREADS = 10;
        const int N = 1000;

        static int[,] A = new int[N, N];
        static int[,] B = new int[N, N];
        static int[,] C = new int[N, N];

        static void Main(string[] args)
        {
            
            Thread[] threads;
            threads = new Thread[NUM_THREADS];

            Random random = new Random(42);

            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    A[i, j] = random.Next(0, 100);
                    B[i, j] = random.Next(0, 100);
                }

            }

            for (int i = 0; i < NUM_THREADS; i++)
            {
                threads[i] = new Thread(matrixMultiply);
                threads[i].Start(i);
            }

            for (int i = 0; i < NUM_THREADS; i++)
            {
                threads[i].Join();
            }

            Console.WriteLine(C[0,0]);
        }

        static void matrixMultiply(object id)
        {
            int threadId = (int) id;
            int chunksize = (N + NUM_THREADS - 1) / NUM_THREADS;
            int lower = threadId * chunksize;
            int upper = Math.Min(lower + chunksize, N);
            System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
            watch.Start();
            for (int i = lower; i < upper; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    C[i, j] = 0;
                    for (int k = 0; k < N; k++)
                    {
                        C[i, j] = A[i, k] * B[k, j];
                    }
                }
            }
            watch.Stop();
            Console.WriteLine(watch.ElapsedMilliseconds);
        }
    }
}
