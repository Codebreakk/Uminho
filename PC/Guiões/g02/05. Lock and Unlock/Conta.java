import java.lang.Thread;
import java.util.concurrent.locks.ReentrantLock;

public class Conta{
  private double saldo;
  private ReentrantLock lockConta;

  public void lock(){
    lockConta.lock();
  }

  public void unlock(){
    lockConta.unlock();
  }

  public Conta(){
    this.saldo = 0;
    this.lockConta = new ReentrantLock();
  }

  public double consultar(){
    //this.lockConta.lock();
    double saldo = this.saldo;
    //this.lockConta.unlock();
    return saldo;
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
