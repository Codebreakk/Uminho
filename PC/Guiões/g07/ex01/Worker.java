import java.net.Socket;
import java.net.ServerSocket;
import java.io.*;
import java.lang.Thread;
import java.lang.Double;

public class Worker implements Runnable{
  private Socket socket;
  private Banco banco;
  private int worker_id;
  private BufferedReader in;
  private BufferedWriter out;

  public Worker(Banco banco, Socket socket, int worker_id){
    this.banco = banco;
    this.socket = socket;
    this.worker_id = worker_id;
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

      // ler e escrever nos canais de acordo com o protocolo da aplicação.
      String line = null;
      while((line = this.in.readLine()) != null && !line.equals("quit")){
        // Mensagem escrita para o terminal do servidor.
        System.out.println("\n> Received message from client: " + line);
        // Mensagem escrita para o terminal do cliente.
        Boolean result = false;
        if(line.contains("criar_conta")){
          Double saldo = Double.parseDouble(line.replace("criar_conta ", ""));
          result = this.banco.criarConta(worker_id, saldo);
        }else if(line.contains("fechar_conta")){
          int id = Integer.parseInt(line.replace("fechar_conta ", ""));
          result = this.banco.fecharConta(id);
        }

        if(result == true){
          // a operação ocorreu com sucesso, informar cliente
        }else{
          // a operação falhou, informar cliente do erro
        }

        System.out.println("> Replied with: " + line);
      }

      // fechar socket e respectivos canais.
      System.out.println("> Client disconnected. Connection is closed.\n");
      //this.chat.remove_disconnected_user(worker_id);
      socket.shutdownOutput();
      socket.shutdownInput();
      socket.close();
    }catch(IOException e){
      System.out.println("Something failed...\n");
      e.printStackTrace();
    }
  }
}
