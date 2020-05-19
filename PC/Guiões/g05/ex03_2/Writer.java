import java.lang.Thread;

public class Writer implements Runnable{
  RWLock rwlock;
  private int id;

  public Writer(int id){
    this.rwlock = new RWLock();
    this.id = id;
  }

  @Override
  public void run(){
    this.rwlock.writelock();
    rwlock.increment_total();
    rwlock.decrement_total();
    System.out.println("W  id: " + id + "\n");
    try{
      Thread.sleep(1000);
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      this.rwlock.writeunlock();
    }
  }
}
