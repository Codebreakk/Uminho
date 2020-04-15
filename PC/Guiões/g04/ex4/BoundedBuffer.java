import java.lang.String;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class BoundedBuffer{
  int total = 2;
  private int[] values;
  private int poswrite;
  private ReentrantLock lock;
  private Condition isEmpty;
  private Condition isFull;

  public BoundedBuffer(int n){
    this.values = new int[n];
    this.poswrite = 0;
    this.lock = new ReentrantLock();
    this.isEmpty = this.lock.newCondition();
    this.isFull = this.lock.newCondition();
  }

  // bloquear enquanto o array estiver cheio
  public synchronized void put(int v) throws InterruptedException{
    this.lock.lock();
    while(this.poswrite == (this.values.length)){
      this.isFull.await();
    }
    this.values[poswrite] = v;
    poswrite++;
    this.isEmpty.signal();
    this.lock.unlock();
  }

  // bloquear enquanto o array estiver vazio
  public synchronized int get() throws InterruptedException{
    this.lock.lock();
    while(this.poswrite == 0){
      this.isEmpty.await();
    }
    int posread = --this.poswrite;
    int result = this.values[posread];
    this.isFull.signal();
    this.lock.unlock();

    return result;
  }
}
