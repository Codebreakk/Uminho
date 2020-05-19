import java.util.LinkedList;

public class Main{
  public static void main(String[] args){
    RWLock rwlock = new RWLock();
    int ARRAY_SIZE = 60;
    Thread[] threads = new Thread[ARRAY_SIZE];

    LinkedList<Reader> readers = new LinkedList<Reader>();
    LinkedList<Writer> writers = new LinkedList<Writer>();
    LinkedList<ReaderWriter> readerwriters = new LinkedList<ReaderWriter>();

    for(int i = 0; i < ARRAY_SIZE/4; i++){
      Reader r = new Reader(i);
      readers.add(r);
      threads[i] = new Thread(r);
      threads[i].start();

      int i2 = i + ARRAY_SIZE/4;
      Writer w = new Writer(i2);
      writers.add(w);
      threads[i2] = new Thread(w);
      threads[i2].start();
    }

    for(int i = ARRAY_SIZE/2; i < ARRAY_SIZE; i++){
      ReaderWriter rw = new ReaderWriter(i);
      readerwriters.add(rw);
      threads[i] = new Thread(rw);
      threads[i].start();
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
