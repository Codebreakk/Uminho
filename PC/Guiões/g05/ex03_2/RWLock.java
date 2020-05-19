import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class RWLock{
  private ReentrantLock lock;
  private Condition reader_wait;
  private Condition writer_wait;
  private Condition reader_writer_wait;
  private int writer; //1 se escritor está a usar a secção critica
  private int reader_writer;
  private int readers;
  private int writers_waiting;
  private int reader_writer_waiting;
  private static int total = 0;

  public RWLock(){
    this.lock = new ReentrantLock();
    this.reader_wait = this.lock.newCondition();
    this.writer_wait = this.lock.newCondition();
    this.reader_writer_wait = this.lock.newCondition();
    this.writer = 0;
    this.reader_writer = 0;
    this.readers = 0;
    this.writers_waiting = 0;
    this.reader_writer_waiting = 0;
  }

  public void increment_total(){
    this.total++;
  }

  public void decrement_total(){
    this.total--;
  }

  public int get_total(){
    return this.total;
  }

  public void readlock(){
    lock.lock();
    try{
      while(writer != 0 || writers_waiting > 0){
        reader_wait.await();
        readers++;
      }
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      lock.unlock();
    }
  }

  public void readunlock(){
    lock.lock();
    readers--;

    if(readers == 0){
      reader_writer_wait.signal();
      writer_wait.signal();
    }

    lock.unlock();
  }

  public void writelock(){
    lock.lock();
    try{
      this.writers_waiting++;
      while(readers > 0 || writer != 0 || reader_writer != 0 || reader_writer_waiting > 0){
        writer_wait.await();
      }
      this.writers_waiting--;
      this.writer = 1;
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      lock.unlock();
    }
  }

  public void writeunlock(){
    lock.lock();
    this.writer = 0;

    reader_wait.signalAll();
    reader_writer_wait.signal();
    writer_wait.signal();

    lock.unlock();
  }

  public void readwritelock(){
    lock.lock();

    try{
      this.reader_writer_waiting++;
      while(readers > 0 || writer != 0 || reader_writer != 0){
        reader_writer_wait.await();
      }
      this.reader_writer_waiting--;
      this.reader_writer = 1;
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      lock.unlock();
    }
  }

  public void readwriteunlock(){
    lock.lock();
    this.reader_writer = 0;

    reader_wait.signalAll();
    reader_writer_wait.signal();
    writer_wait.signal();

    lock.unlock();
  }
}
