import java.lang.Thread;

public class cliente2 implements Runnable{

  private Banco banco;

  public cliente2(Banco b){
    this.banco = b;
  }

  @Override
  public void run(){
    this.banco.transferencia(0, 1, 1000);
  }
}
