public class Barreira{
  private int n_threads;

  public Barreira(int n_threads){
    this.n_threads = n_threads;
  }

  public synchronized void esperar() throws InterruptedException{
    if(this.n_threads >= 0){
      System.out.println("Waiting..." + n_threads + "\n");
      this.n_threads--;
      this.wait();
    }else{
      System.out.println("GO!\n");
      this.notifyAll();
    }
  }
}
