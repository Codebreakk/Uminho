import java.util.LinkedList;

public class Main{
  public static void main(String[] args){
    RWLock rwlock = new RWLock();
    int ARRAY_SIZE = 30;
    Thread[] threads = new Thread[ARRAY_SIZE];

    LinkedList<Reader> readers = new LinkedList<Reader>();
    LinkedList<Writer> writers = new LinkedList<Writer>();

    for(int i = 0; i < 15; i++){
      Reader r = new Reader(i);
      readers.add(r);
      threads[i] = new Thread(r);
      threads[i].start();

      Writer w = new Writer(i+15);
      writers.add(w);
      threads[i+15] = new Thread(w);
      threads[i+15].start();
    }

    for(Thread t: threads){
      try{
        t.join();
      } catch(InterruptedException e){
        e.printStackTrace();
      }
    }
  }
}
