// start, join, new Thread
import java.lang.Thread;

public class nThreads implements Runnable{
  int n; // Thread number

  public nThreads(int n){
    this.n = n;
  }

  @Override
  public void run(){
    int i = 0;
    while(i <= this.n){
      i++;
      System.out.println(i);
    }
  }
}
