public class Main{
  public static void main(String[] args){
    Thread[] t1 = new Thread[1];
    Thread[] t2 = new Thread[1];
    Banco b = new Banco(2);

    t1[0] = new Thread(new cliente1(b));
    t1[0].setName(String.valueOf(0));

    t2[0] = new Thread(new cliente2(b));
    t2[0].setName(String.valueOf(0));

    t1[0].start();
    t2[0].start();

    try{
      t1[0].join();
      t2[0].join();
    }catch(InterruptedException e){
      System.err.println("Erro no join.\n");
      e.printStackTrace();
    }
    System.out.println(b.consultar(0));
  }
}
