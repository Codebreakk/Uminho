public class Main{
  public static void main(String[] args){
    Barreira b = new Barreira(2);
    int ARRAY_SIZE = 2;
    Thread[] producers = new Thread[ARRAY_SIZE];
    Thread[] consumers = new Thread[ARRAY_SIZE];

    for(int i = 0; i < ARRAY_SIZE; i++){
      producers[i] = new Thread(new Producer(b));
      consumers[i] = new Thread(new Consumer(b));
    }

    for(int i = 0; i < ARRAY_SIZE; i++){
      producers[i].start();
      consumers[i].start();
    }

    try{
      for(int i = 0; i < ARRAY_SIZE; i++){
        producers[i].join();
        consumers[i].join();
      }
    }catch(InterruptedException e){
      e.printStackTrace();
    }
  }
}
