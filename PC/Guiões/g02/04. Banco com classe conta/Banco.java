import java.lang.Thread;

public class Banco{
  private Conta[] contas;

  public Banco(int n){
    this.contas = new Conta[n];
    for(int i = 0; i < n; i++){
      contas[i] = new Conta();
    }
  }

  public void depositar(int conta, double valor){
    (this.contas[conta]).depositar(valor);
  }

  public void levantar(int conta, double valor){
    (this.contas[conta]).levantar(valor);
  }

  public double consultar(int conta){
    return (contas[conta].consultar());
  }

  public void transferencia(int contaIn, int contaOut, double valor){
    levantar(contaOut, valor);
    depositar(contaIn, valor);
  }
}
