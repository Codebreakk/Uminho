import java.lang.Thread;

public class Banco{
  private double[] contas;

  public Banco(int n){
    this.contas = new double[n];
  }

  public synchronized void depositar(int conta, int valor){
    this.contas[conta] += valor;
  }

  public synchronized void levantar(int conta, int valor){
    this.contas[conta] -= valor;
  }

  public synchronized double consultar(int conta){
    return(contas[conta]);
  }
}
