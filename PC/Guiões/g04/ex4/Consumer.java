import java.lang.Thread;

public class Consumer implements Runnable{
  private BoundedBuffer boundedBuffer;

  public Consumer(BoundedBuffer b){
    this.boundedBuffer = b;
  }

  @Override
  public void run(){
    for(int i = 0; i < 20; i++){
      try{
        this.boundedBuffer.get();
      }catch(InterruptedException e){
        e.printStackTrace();
      }
    }
  }
}
