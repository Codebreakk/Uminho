import java.lang.Thread;

public class Conta{
  private double saldo;

  public Conta(){
    this.saldo = 0;
  }

  public double consultar(){
    synchronized(this){
      return this.saldo;
    }
  }

  public void depositar(double valor){
    synchronized(this){
      this.saldo += valor;
    }
  }

  public void levantar(double valor){
    synchronized(this){
      this.saldo -= valor;
    }
  }
}
