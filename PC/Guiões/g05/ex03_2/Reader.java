import java.lang.Thread;

public class Reader implements Runnable{
  RWLock rwlock;
  private int id;

  public Reader(int id){
    this.rwlock = new RWLock();
    this.id = id;
  }

  @Override
  public void run(){
    rwlock.readlock();
    System.out.println("R  id: " + id + ", total = " + rwlock.get_total() + "\n");
    try{
      Thread.sleep(1000);
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      rwlock.readunlock();
    }
  }
}
