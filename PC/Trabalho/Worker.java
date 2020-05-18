import java.net.Socket;
import java.net.ServerSocket;
import java.io.*;
import java.lang.Thread;

public class Worker implements Runnable{
  private Socket socket;
  private Registos registos;
  private BufferedReader in;
  private BufferedWriter out;

  public Worker(Registos registos, Socket socket){
    this.registos = registos;
    this.socket = socket;
    try{
      this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      this.out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
    }catch(IOException e){
      e.printStackTrace();
    }
    System.out.println("\n> Worker successfully created!\n");
  }

  private void write_message(BufferedWriter out, String message) throws IOException{
      out.write(message);
      out.newLine();
      out.flush();
  }

  private String login_and_signup() throws IOException{
    String username = null;
    String client_response = null;
    Boolean step_completed = false;
    String message_on_success = null;
    int tries = 0;

    // O usuário não pode avançar enquanto não se registar, logo deverá ficar
    // preso neste ciclo até que complete o registo.
    while(!step_completed){
      //TODO: para reduzir o tamanho do código e ao mesmo tempo não permitir
      //passwords inválidas, pq não considerar null => go back?
      write_message(out, "> Please enter your username: ");
      username = this.in.readLine();
      Boolean user_exists = registos.user_exists(username);
      if(user_exists == true){
        write_message(this.out, "> Username already registered, do you wish to login [y/n]?:");

        client_response = this.in.readLine();
        if(client_response.equals("y")){
          tries = 0;
          while(!step_completed && tries < 3){
            write_message(this.out, "> Insert password: ");
            //TODO: verificar que a password não é nula e talvez q tem mais de 8 chars;
            String password = this.in.readLine();
            step_completed = registos.login_user(username, password);
            tries++;
            if(!step_completed && tries < 3){
              write_message(this.out, "> Login failed, please try again.");
            }else if(!step_completed && tries >= 3){
              write_message(this.out, "> Login failed after 3 tries.");
            }else if(step_completed){
              message_on_success = "\n> You are now logged in!\n> Welcome back " + username + ".\n";
            }
          }
        }else if(client_response.equals("n")){
          write_message(this.out, "> Restarting login... ");
        }
      }else if(user_exists == false){
        write_message(this.out, "> Username not found, register new user [y/n]?:");
        client_response = this.in.readLine();
        if(client_response.equals("y")){
          tries = 0;
          while(!step_completed && tries < 3){
            write_message(this.out, "> Insert password: ");
            //TODO: verificar que a password não é nula e talvez q tem mais de 8 chars;
            String password = this.in.readLine();
            step_completed = registos.register_new_user(username, password, this.out);
            tries++;
            if(!step_completed && tries < 3){
              write_message(this.out, "> Invalid password.");
            }else if(!step_completed && tries >= 3){
              write_message(this.out, "> Signup failed after 3 tries.");
            }else if(step_completed){
              message_on_success = "> You have completed the signup!\n> Welcome " + username + ".\n";
            }
          }
        }else if(client_response.equals("n")){
          write_message(this.out, "> Restarting login... ");
        }
      }
    }
    write_message(this.out, message_on_success);
    return username;
  }

  @Override
  public void run(){
    String client_response = null;
    Boolean step_completed = false;
    /** Start of Step 1 - Cada cliente deve registar-se (passo efetuado uma úni-
    ca vez por cliente) e autenticar-se para se ligar ao servidor. */
    String username = null;
    try{
      username = login_and_signup();
    }catch(IOException e){
      System.out.println("> IOException when trying to login/signup: \n");
      e.printStackTrace();
    }

    /** End of Step 1. */
    if(username != null){
      System.out.println("> Step 1 completed for user: " + username + ".\n");
    }else{
      System.out.println("> Step 1 failed for user: " + username + ".\n");
    }


    /** Start of Step 2 - De seguida, o servidor aguarda que o cliente indique
    quantos casos de doença este conhece nos seus contactos (valor inteiro >= 0)
    */
  }
}
