public class Main2{
  public static void main(String args[]){
    //int total = Integer.parseInt(args[0]), i = 0;
    int total = 3;
    int incVal = 5;
    int counter = 5;
    Thread[] ThreadArray = new Thread[total];

    for(int i = 0; i < total; i++){
      ThreadArray[i] = new Thread(new nThreads(i, incVal, counter));
      ThreadArray[i].setName(String.valueOf(i));
    }

    for(int i = 0; i < total; i++){
      ThreadArray[i].start();
    }
    try{
      for(int i = 0; i < total; i++){
        ThreadArray[i].join();
      }
    }catch(InterruptedException e){
      System.err.println("Erro no join.\n");
      e.printStackTrace();
    }
  }
}
