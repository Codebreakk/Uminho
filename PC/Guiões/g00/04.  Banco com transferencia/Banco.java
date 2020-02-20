import java.lang.Thread;

public class Banco{
  private double[] contas;

  public Banco(int n){
    this.contas = new double[n];
  }

  public synchronized void depositar(int conta, double valor){
    this.contas[conta] += valor;
  }

  public synchronized void levantar(int conta, double valor){
    this.contas[conta] -= valor;
  }

  public synchronized double consultar(int conta){
    return(contas[conta]);
  }

  public synchronized void transferencia(int contaIn, int contaOut, double valor){
    levantar(0, valor);
    depositar(1, valor);
  }
}
