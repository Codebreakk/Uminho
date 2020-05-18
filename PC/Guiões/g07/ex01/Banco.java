import java.io.*;
import java.util.HashMap;
import java.util.concurrent.locks.ReentrantLock;

public class Banco{
  private HashMap<Integer, Double> contas;
  private ReentrantLock lock;

  public Banco(){
    contas = new HashMap<Integer, Double>();
    this.lock = new ReentrantLock();
  }

  private Boolean get_conta(int id){
    Double new_saldo = this.contas.get(id);
    return (!contas.containsKey(id));
  }

  private Boolean add_conta(int id, Double saldo){
    Double new_saldo = this.contas.put(id, saldo);
    return (!contas.containsKey(id));
  }

  private Boolean remove_conta(int id){
    Double new_saldo = this.contas.remove(id);
    return (!contas.containsKey(id));
  }

  public Boolean criarConta(int id, double saldo){
    Boolean result = false;

    this.lock.lock();

    try {
      if(!get_conta(id)){
        result = add_conta(id, saldo);
      }
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public Boolean fecharConta(int id){
    Boolean result = false;

    this.lock.lock();

    try {
      if(get_conta(id)){
        result = remove_conta(id);
      }
    }finally{
      this.lock.unlock();
      return result;
    }
  }
}
