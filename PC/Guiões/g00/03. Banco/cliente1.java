import java.lang.Thread;

public class cliente1 implements Runnable{

  private Banco banco;

  public cliente1(Banco b){
    this.banco = b;
  }

  @Override
  public void run(){
    for(int i = 0; i < 1000; i++){
      this.banco.depositar(0, 5);
    }
  }
}
