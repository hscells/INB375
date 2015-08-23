public class MatrixMultiplyRunnable implements Runnable {

  public void run() {
    System.out.println("Hello from a running thread!");
  }

  public static void main(String[] args) {
    (new Thread(new MatrixMultiplyRunnable())).start();
  }

}
