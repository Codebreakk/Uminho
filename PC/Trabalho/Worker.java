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

  /** Step 1 - Login and Signup
  */
  private String login_and_signup() throws IOException{
    String username = null; // username indicado pelo cliente
    Boolean step_completed = false; // flag usada para verificar se o processo de login/signup foi completado
    String message_on_success = null; // String que guarda a mensagem a ser escrita quando o processo é completado com sucesso
    Boolean go_back = false; // flag usada para indicar que o usuário quer voltar ao início do processo
    int tries = 0; // contador do número de tentativas de login/signup

    // O usuário não pode avançar enquanto não se registar, logo deverá ficar
    // preso neste ciclo até que complete o registo.
    while(!step_completed){
      go_back = false; // flag reset no início de cada ciclo.
      write_message(out, "> Please enter your username:");
      username = this.in.readLine();
      if(username.trim().equals("") || username.trim().equals(null)){
        write_message(this.out, "> Invalid username, please try again.");
      }else{
        Boolean user_exists = registos.user_exists(username);
        Boolean user_is_logged = registos.check_if_user_is_logged_in(username);
        if(user_is_logged){ // USUÁRIO JÁ ESTÁ LOGGADO
          write_message(this.out, "> User already logged in. Please try again.");
          System.out.println("> User " + username + " is already logged in.\n");
        }else if(user_exists){ // LOGIN
          write_message(this.out, "> Username already registered, insert password or leave empty to go back:");
          tries = 0;
          while(!step_completed && !go_back && tries < 3){
            String password = this.in.readLine();
            if(password.trim().equals("") || password.trim().equals(null)){
              write_message(this.out, "> Returning to login screen...");
              go_back = true;
            }else{
              step_completed = registos.login_user(username, password);
              tries++;
              if(!step_completed && tries < 3){
                write_message(this.out, "> Login failed, please try again.");
              }else if(!step_completed && tries >= 3){
                write_message(this.out, "> Login failed after 3 tries.");
              }else if(step_completed){
                registos.set_user_to_logged_in(username);
                message_on_success = "\n> You are now logged in!\n> Welcome back " + username + ".\n";
              }
            }
          }
        }else if(user_exists == false){ // NOVO REGISTO
          write_message(this.out, "> Username not found, insert password to register or leave empty to go back:");
          tries = 0;
          while(!step_completed && tries < 3){
            write_message(this.out, "> Insert password:");
            String password = this.in.readLine();
            if(password.trim().equals("") || password.trim().equals(null)){
              write_message(this.out, "> Returning to login screen...");
              go_back = true;
            }else{
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
          }
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
      //TODO: melhorar o tratamento de excepções.
      System.out.println("> IOException when trying to login/signup:\n");
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
    try{
      write_message(this.out, "> Please insert the number of infected people you know:");
      String line = null;
      while((line = this.in.readLine()) != null && !line.equals("quit")){
        Boolean success = false;
        int n_infected = -1;
        try{ // este try é para evitar situações em que recebemos Strings q não são números.
          n_infected = Integer.parseInt(line);
        }catch(NumberFormatException e){
          //TODO: melhorar o tratamento de excepções.
          write_message(this.out, "> The number is in the wrong format, please try again:");
          System.out.println("> NumberFormatException when trying to parse the String " + line + " to an Integer.\n");
          e.printStackTrace();
          continue; // salta o resto do loop de forma a recomeçar.
        }
        success = registos.set_casos(username, n_infected);
        if(success){
          write_message(this.out, "> The number of infected has been successfully registered. Thank you!");
          // agora precisamos de calcular a proporção média e enviar para todos os clientes.
          registos.write_estimate_to_all_users();
        }else{
          write_message(this.out, "> Failed to register the number of infected, please try again:");
        }
      }
    }catch(IOException e){
      //TODO: melhorar o tratamento de excepções.
      System.out.println("> IOException when trying to register the number of infected people the user knows: \n");
      e.printStackTrace();
    }

    /** FINAL STEP - client disconnects */
    // fechar socket e respectivos canais.
    try{
      System.out.println("> Client disconnected. Connection is closed.\n");
      registos.set_user_to_logged_out(username);
      socket.shutdownOutput();
      socket.shutdownInput();
      socket.close();
    }catch(IOException e){
      //TODO: melhorar o tratamento de excepções.
      System.out.println("> IOException when trying to close the socket:\n");
      e.printStackTrace();
    }
  }
}
