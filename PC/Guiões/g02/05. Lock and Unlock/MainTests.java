import junit.framework.TestCase;

/**javac -cp /usr/share/java/junit4-4.12.jar:hamcrest-core-1.3.jar Main.java Conta.java
Banco.java MainTests.java cliente1.java cliente2.java
java -cp /usr/share/java/junit4-4.12.jar MainTests
*/
public class MainTests extends TestCase{
  public void testTransferClientes(){
    for(int i = 0; i < 100; i++){

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

      assertEquals(b.consultar(0), 0.0);
      assertEquals(b.consultar(1), 0.0);
    }
  }
}
