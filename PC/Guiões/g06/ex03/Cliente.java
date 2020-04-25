import java.net.Socket;
import java.io.*;

public class Cliente{
  private String address;
  private int port;
  private Socket socket;

  public Cliente(String address, int port){
    this.address = address;
    this.port = port;
  }

  public void startCliente(){
    BufferedReader in = null;
    BufferedReader systemIn = null;
    PrintWriter out = null;
    // criar socket e ligação com o servidor
    try{
      this.socket = new Socket(this.address, this.port);
    }catch(IOException e){
      e.printStackTrace();
    }

    // abrir canais de escrita e leitura no socket
    try{
      in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      out = new PrintWriter(socket.getOutputStream());
      systemIn = new BufferedReader(new InputStreamReader(System.in));
    }catch(IOException e){
      e.printStackTrace();
    }

    // ler e escrever nos canais de acordo com o protocolo da aplicação
    try{
      String line = systemIn.readLine(); // line = userInput
      String response = null;
      while(line != null && !line.equals("quit")){
        out.println(line);
        out.flush();

        response = in.readLine();
        System.out.println("Server Response: " + response + "\n");
        line = systemIn.readLine();
      }
    }catch(IOException e){
      e.printStackTrace();
    }

    // fechar socket e respectivos canais
    try{
      socket.shutdownInput();
      socket.shutdownOutput();
      socket.close();
    }catch(IOException e){
      e.printStackTrace();
    }
  }
  public static void main(String[] args){
    int port = 12345;
    String address = "127.0.0.1";

    Cliente cliente1 = new Cliente(address, port);
    cliente1.startCliente();
  }
}
