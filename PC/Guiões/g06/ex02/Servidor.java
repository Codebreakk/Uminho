import java.net.Socket;
import java.net.ServerSocket;
import java.io.*;

public class Servidor{
	private int port;
	private ServerSocket sSock;

	public Servidor(int port){
		this.port = port;
	}

	public void startServidor(){
		try {
			// aceitar conexões indefinidamente.
			this.sSock = new ServerSocket(this.port);
			System.out.println("Server socket created.");
		} catch (IOException e) {
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
			} catch (IOException e) {
				System.out.println("Connection failed!");
				e.printStackTrace();
			}

			// abrir canais de escrita e leitura no socket.
			BufferedReader in = null;
			PrintWriter out = null;
			try {
				in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				out = new PrintWriter(socket.getOutputStream());
			} catch (IOException e) {
				System.out.println("Failed creating the stream readers.");
				e.printStackTrace();
			}

			// ler e escrever nos canais de acordo com o protocolo da aplicação.
			String line = null;
			try {
				while((line = in.readLine()) != null && !line.equals("quit")){
					System.out.println("\n> Received message from client: " + line);
					out.println(line);
					out.flush();
					System.out.println("> Replied with: " + line);
				}
			} catch (IOException e) {
				System.out.println("Failed when reading the client message.");
				e.printStackTrace();
			}

			System.out.println("> Client disconnected. Connection is closed.\n");

			// fechar socket e respectivos canais.
			try {
				socket.shutdownOutput();
				socket.shutdownInput();
				socket.close();
			} catch (IOException e) {
				System.out.println("Failed when closing the socket and channels.");
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args){
		int port = 12345;
		String address = "127.0.0.1";

		Servidor servidor = new Servidor(port);
		servidor.startServidor();
	}
}
