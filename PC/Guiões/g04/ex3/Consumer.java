import java.lang.Thread;

public class Consumer implements Runnable{
  private Barreira barreira;

  public Consumer(Barreira b){
    this.barreira = b;
  }

  @Override
  public void run(){
    for(int i = 0; i < 2; i++){
      try{
        System.out.println("Consumer numero: " + i + ".\n");
        this.barreira.esperar();
      }catch(InterruptedException e){
        e.printStackTrace();
      }
    }
  }
}
