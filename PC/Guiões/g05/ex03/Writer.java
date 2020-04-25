import java.lang.Thread;

public class Writer implements Runnable{
  RWLock rwlock;
  private int id;

  public Writer(int id){
    this.id = id;
  }

  @Override
  public void run(){
    this.rwlock.writelock();
    try{
      Thread.sleep(1000);
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      this.rwlock.writeunlock();
    }
  }
}
