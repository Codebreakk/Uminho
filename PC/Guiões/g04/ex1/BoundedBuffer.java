import java.lang.String;

public class BoundedBuffer{
  int total = 5;
  private int[] values;
  private int poswrite;

  public BoundedBuffer(int n){
    this.values = new int[n];
    this.poswrite = 0;
  }

  // bloquear enquanto o array estiver cheio
  public synchronized void put(int v) throws InterruptedException{
    while(poswrite == (this.values.length)){
      this.wait();
    }
    values[poswrite] = v;
    poswrite++;
    this.notifyAll();
  }

  // bloquear enquanto o array estiver vazio
  public synchronized int get() throws InterruptedException{
    while(poswrite == 0){
      this.wait();
    }
    int posread = --this.poswrite;
    int result = this.values[posread];
    this.notifyAll();

    return result;
  }
}
