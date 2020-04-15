import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;


public class Item{
  private ReentrantLock lock;
  private Condition condition;
  private int quantity;

  public Item(){
    this.quantity = 0;
    this.lock = new ReentrantLock();
    this.condition = this.lock.newCondition();
  }

  public void supply(int quantity){
    this.lock.lock();
    this.quantity += quantity;
    this.condition.signalAll();
    this.lock.unlock();
  }

  public void consume(){
    this.lock.lock();
    while(quantity == 0){
      this.condition.await();
    }
    this.quantity--;
    this.lock.unlock();
  }
}
