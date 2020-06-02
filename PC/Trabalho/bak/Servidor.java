import java.net.Socket;
import java.net.ServerSocket;
import java.io.*;

public class Servidor{
  private int port;
  private Registos registos;
  private ServerSocket sSock;

  public Servidor(int port){
    this.port = port;
    this.registos = new Registos();
  }

  public void startServidor(){
		try {
			// aceitar conexões indefinidamente.
			this.sSock = new ServerSocket(this.port);
			System.out.println("Server socket created.");
		} catch(IOException e) {
			System.out.println("Failed creating server socket.");
			e.printStackTrace();
		}

		while(true){
			Socket socket = null;
			try {
				// bloquear até que uma conexão seja estabelecida.
				System.out.println("> Server is running and waiting for a new connection...");
				socket = sSock.accept();
				System.out.println("> Connection received!");
			} catch(IOException e) {
				System.out.println("Connection failed!");
				e.printStackTrace();
			}

			// Criar o worker
			Worker worker = new Worker(this.registos, socket);
			new Thread(worker).start();
		}
	}

  public static void main(String[] args){
    int port = 12345;

    Servidor servidor = new Servidor(port);
    servidor.startServidor();
  }
}
