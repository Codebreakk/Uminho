import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class echo{
  public static void main(String[] args) throws IOException{
    BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

    String line = null;
    while((line = reader.readLine()) != null){
      System.out.println(line);
    }
  }
}
