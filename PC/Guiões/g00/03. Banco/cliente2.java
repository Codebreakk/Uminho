import java.lang.Thread;

public class cliente2 implements Runnable{

  private Banco banco;

  public cliente2(Banco b){
    this.banco = b;
  }

  @Override
  public void run(){
    for(int i = 0; i < 1000; i++){
      this.banco.levantar(0, 5);
    }
  }
}
