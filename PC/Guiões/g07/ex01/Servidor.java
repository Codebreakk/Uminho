import java.net.Socket;
import java.net.ServerSocket;
import java.io.*;

public class Servidor{
	private int port;
	private ServerSocket sSock;
	private Banco banco;

	public Servidor(int port){
		this.port = port;
		this.banco = new Banco();
	}

	public void startServidor(){
		int worker_id = 0;
		try {
			// aceitar conexões indefinidamente.
			this.sSock = new ServerSocket(this.port);
			worker_id = 1;
			System.out.println("Server socket created. Id: " + worker_id);
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
			Worker worker = new Worker(this.Banco, socket, worker_id++);
			new Thread(worker).start();
		}
	}

	public static void main(String[] args){
    int port = 12345;

    Servidor servidor = new Servidor(port);
    servidor.startServidor();
  }
}
