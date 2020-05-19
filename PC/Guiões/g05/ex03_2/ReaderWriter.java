import java.lang.Thread;

public class ReaderWriter implements Runnable{
  RWLock rwlock;
  private int id;

  public ReaderWriter(int id){
    this.rwlock = new RWLock();
    this.id = id;
  }

  @Override
  public void run(){
    this.rwlock.readwritelock();
    int total = rwlock.get_total();
    if(total >= 0){
      rwlock.decrement_total();
      total--;
    }else{
      rwlock.increment_total();
      total++;
    }
    System.out.println("RW id: " + id + ", total = " + total + "\n");
    try{
      Thread.sleep(1000);
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      this.rwlock.readwriteunlock();
    }
  }
}
