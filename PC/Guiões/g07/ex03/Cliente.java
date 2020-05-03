import java.net.Socket;
import java.io.*;

public class Cliente{
  private String address;
  private int port;
  public Socket socket;
  public BufferedReader in; // leitura do Socket do servidor
  public PrintWriter out; // escrita para o Socket do servidor
  public String nickname;

  public Cliente(String address, int port){
    this.address = address;
    this.port = port;
  }

  public void startCliente(){
    BufferedReader systemIn = null; // Input da consola do cliente

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

    // registo do cliente
    String response = null;
    try{
      Boolean registered = false;
      while(registered == false){
        System.out.println("Please, choose a nickname:");
        String nickname = systemIn.readLine();

        // enviar nickname para o servidor;
        out.println(nickname);
        out.flush();

        response = in.readLine();
        registered = response.equals("OK");
        if(!registered){
          System.out.println("Nickname already chosen, try another one...");
        }else if (registered){
          System.out.println("You have been registered successfully, welcome!");
        }
      }

      Thread listener = new Thread(new ClientListener());
      listener.start();

    }catch(IOException e){
      e.printStackTrace();
    }

    // ler e escrever nos canais de acordo com o protocolo da aplicação
    response = null;
    try{
      String line = systemIn.readLine(); // line = userInput

      while(line != null && !line.equals("quit")){
        out.println(line);
        out.flush();

        response = in.readLine();
        System.out.println(response + "\n");
        line = systemIn.readLine();
      }
    }catch(IOException e){
      e.printStackTrace();
    }

    // fechar socket e respectivos canais
    try{
      systemIn.close();
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

    Cliente cliente = new Cliente(address, port);
    cliente.startCliente();
  }

  public class ClientListener implements Runnable{
  public ClientListener(){}

    @Override
    public void run(){
      String message;
      try{
        while((message = in.readLine()) != null){
          System.out.println(message);
        }
      }catch(IOException e){
        e.printStackTrace();
      }
    }
  }
}
