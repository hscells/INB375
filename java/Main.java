import java.util.Random;

public class Main {

  public static int N = 1000;
  public static int NUM_THREADS = 10;

  public static int[][] A = new int[N][N];
  public static int[][] B = new int[N][N];
  public static int[][] C = new int[N][N];

  public static void main(String[] args) {

    Random rand = new Random(42);

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i][j] = rand.nextInt();
        B[i][j] = rand.nextInt();
      }
    }

  }

}
