public class Incrementor implements Runnable{
 private int incvalue;
 private Counter counter;

 public Incrementor(Counter c, int incvalue){
 this.incvalue = incvalue;
 this.counter = counter;
 }

 @Override
 public void run(){
   for(int i = 1; i <= this.incvalue; i++){
     counter.count++;
   }
 }
}
