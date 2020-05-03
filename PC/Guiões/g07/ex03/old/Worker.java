import java.net.Socket;
import java.net.ServerSocket;
import java.io.*;
import java.lang.Thread;

public class Worker implements Runnable{
  private Socket socket;
  private int worker_id;
  private String client_nickname;

  public Worker(Socket socket, int worker_id){
    this.socket = socket;
    this.worker_id = worker_id;
    this.client_nickname = null;
  }

  private void setNickname(String client_nickname){
    this.client_nickname = client_nickname;
  }
  private String getNickname(){
    return this.client_nickname;
  }

  @Override
  public void run(){
    // abrir canais de escrita e leitura no socket.
    try{
      BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

      // ler e escrever nos canais de acordo com o protocolo da aplicação.
      String line = null;
      while((line = in.readLine()) != null && !line.equals("quit")){
        if(line.contains("set_nickname ")){
          String nickname = line.replace("set_nickname ", "");
          setNickname(nickname);
          // Mensagem escrita para o terminal do servidor
          System.out.println("Nickname set: " + this.getNickname());
          // Mensagem escrita para o terminal do cliente
          out.write("Client " + client_nickname + ": " + line);
          out.newLine();
          out.flush();
        }else{
          System.out.println("\n> Received message from client: " + line);
          out.write("Client " + client_nickname + ": " + line);
          out.newLine();
          out.flush();
          System.out.println("> Replied with: " + line);
        }
      }

      // fechar socket e respectivos canais.
      System.out.println("> Client disconnected. Connection is closed.\n");
      socket.shutdownOutput();
      socket.shutdownInput();
      socket.close();
    }catch(IOException e){
      e.printStackTrace();
    }
  }
}
