import java.lang.Thread;
import java.util.concurrent.locks.ReentrantLock;
import java.util.*;

public class Banco{
  private HashMap<Integer, Conta> contas;
  private ReentrantLock lockBanco;

  public Banco(int n){
    this.contas = new HashMap<Integer, Conta>();
    this.lockBanco = new ReentrantLock();
  }

  public void lock(){
    this.lockBanco.lock();
  }

  public void unlock(){
    this.lockBanco.unlock();
  }

  public void criarConta(int id){
    Conta conta = new Conta();

    this.lockBanco.lock();
    this.contas.put(id, conta);
    this.lockBanco.unlock();
  }

  public double fecharConta(int id) throws ContaInvalidaId{
    this.lockBanco.lock();

    if(this.contas.containsKey(id)){
      Conta c = this.contas.get(id);
      c.lock();
      double saldo = c.consultar();
      this.contas.remove(id);
      this.lockBanco.unlock();
      return saldo;
    }else{
      this.lockBanco.unlock();
      throw new ContaInvalidaId();
    }
  }

  public void depositar(int conta, double valor) throws ContaInvalidaId{
    this.lockBanco.lock();
    if(this.contas.containsKey(conta)){
      Conta c = this.contas.get(conta);
      this.lockBanco.unlock();
      c.lock();
      c.depositar(valor);
      c.unlock();
    }else{
      this.lockBanco.unlock();
      throw new ContaInvalidaId();
    }
  }

  public void levantar(int conta, double valor) throws ContaInvalidaId{
    this.lockBanco.lock();
    if(this.contas.containsKey(conta)){
      Conta c = this.contas.get(conta);
      c.lock();
      this.lockBanco.unlock();
      c.levantar(valor);
      c.unlock();
    }else{
      this.lockBanco.unlock();
      throw new ContaInvalidaId();
    }
  }

  public double consultar(int conta) throws ContaInvalidaId{
    Double saldo;
    this.lockBanco.lock();
    if(this.contas.containsKey(conta)){
      Conta c = this.contas.get(conta);
      c.lock();
      this.lockBanco.unlock();
      saldo = c.consultar();
      c.unlock();
    }else{
      this.lockBanco.unlock();
      throw new ContaInvalidaId();
    }

    return saldo;
  }

  public void transferencia(int contaIn, int contaOut, double valor) throws ContaInvalidaId{
    Conta cIn;
    Conta cOut;

    // Lock the bank to manipulate the HashMap
    this.lockBanco.lock();
    // If contaIn exists, acquire the lock for the account
    if(this.contas.containsKey(contaIn)){
      cIn = this.contas.get(contaIn);
      cIn.lock();
    }else{ // else, release the lock for the bank and throw exception
      this.lockBanco.unlock();
      throw new ContaInvalidaId();
    }

    // If contaOut exists, acquire the lock for the account
    if(this.contas.containsKey(contaOut)){
      cOut = this.contas.get(contaOut);
      cOut.lock();
    }else{ // else, release the lock for the bank and contaIn and throw exception
      cIn.unlock();
      this.lockBanco.unlock();
      throw new ContaInvalidaId();
    }
    // We don't need the hashmap anymore, so we can release the bank lock.
    this.lockBanco.unlock();

    // withdraw the money from contaIn and release the lock afterwards.
    cOut.levantar(valor);
    cOut.unlock();

    // deposit the money in contaOut and release the lock afterwards.
    cIn.depositar(valor);
    cIn.unlock();
  }
}

// Unfinished
class ContaInvalidaId extends Exception{
  public void ContaInvalidaId(){
    System.out.println("A conta é invalida.");
  }
}
