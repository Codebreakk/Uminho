import java.lang.Thread;

public class cliente1 implements Runnable{

  private Banco banco;

  public cliente1(Banco b){
    this.banco = b;
  }

  @Override
  public void run(){
    this.banco.transferencia(1, 0, 1000);
  }
}
