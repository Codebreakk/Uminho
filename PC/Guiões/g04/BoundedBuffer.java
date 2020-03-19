public class BoundedBuffer{
  int total = 5;
  private int[] values;
  private int poswrite;

  public BoundedBuffer(int n){
    this.values = new int[n];
    this.poswrite = 0;
  }

  // bloquear enquanto o array estiver cheio
  public synchronized void put(int v){
    while(poswrite == len(this.values)){
      obj.wait();
    }
    int[poswrite] = v;
    poswrite++;
    this.notifyAll();
  }

  // bloquear enquanto o array estiver vazio
  public synchronized int get(){
    while(poswrite == 0){
      obj.wait();
    }
    int posread = --this.poswrite;
    int result = this.values[posread];
    this.notifyAll();

    return result;
  }
}
