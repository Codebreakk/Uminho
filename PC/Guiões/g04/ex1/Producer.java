import java.lang.Thread;

public class Producer implements Runnable{
  private BoundedBuffer boundedBuffer;

  public Producer(BoundedBuffer b){
    this.boundedBuffer = b;
  }

  @Override
  public void run(){
    for(int i = 0; i < 20; i++){
      {
        try{
          this.boundedBuffer.put(i);
        }catch(InterruptedException e){
          e.printStackTrace();
        }
      }
    }
  }
}
