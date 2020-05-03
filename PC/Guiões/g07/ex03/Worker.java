import java.net.Socket;
import java.net.ServerSocket;
import java.io.*;
import java.lang.Thread;

public class Worker implements Runnable{
  private Socket socket;
  private Chat chat;
  private BufferedReader in;
  private BufferedWriter out;

  public Worker(Chat chat, Socket socket){
    this.chat = chat;
    this.socket = socket;
    try{
      this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      this.out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
    }catch(IOException e){
      e.printStackTrace();
    }
    System.out.println("\n> Worker successfully created!\n");
  }

  @Override
  public void run(){
    // abrir canais de escrita e leitura no socket.
    try{
      // Registar novo usuário.
      String nickname = null;
      Boolean registered = false;
      while(registered == false){
        nickname = this.in.readLine();
        System.out.println("> Registering user with nickname: " + nickname + "\n");
        registered = this.chat.register_new_user(nickname, out);
      }
      out.write("OK");
      out.newLine();
      out.flush();
      System.out.println("> New user registered: " + nickname + "\n");

      // ler e escrever nos canais de acordo com o protocolo da aplicação.
      String line = null;
      while((line = this.in.readLine()) != null && !line.equals("quit")){
        // Mensagem escrita para o terminal do servidor.
        System.out.println("\n> Received message from client: " + line);
        // Mensagem escrita para o terminal do cliente.
        this.chat.write_message(nickname + " :: " + line);
        System.out.println("> Replied with: " + line);
      }

      // fechar socket e respectivos canais.
      System.out.println("> Client disconnected. Connection is closed.\n");
      this.chat.remove_disconnected_user(nickname);
      socket.shutdownOutput();
      socket.shutdownInput();
      socket.close();
    }catch(IOException e){
      System.out.println("Something failed...\n");
      e.printStackTrace();
    }
  }
}
