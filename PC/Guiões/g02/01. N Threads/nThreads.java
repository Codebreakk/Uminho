// start, join, new Thread
import java.lang.Thread;

public class nThreads implements Runnable{
  int n; // Thread number
  int incVal;
  int counter;

  public nThreads(int n, int incVal, int counter){
    this.n = n;
    this.incVal = incVal;
    this.counter = counter;
  }

  public synchronized void inc(){
    this.counter++;
  }

  @Override
  public void run(){
    for(int i = 1; i <= this.incVal; i++){
      this.inc();
      System.out.println(this.counter);
    }
  }
}
