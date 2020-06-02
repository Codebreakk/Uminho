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
      String e_cause = e.getCause().toString();
      String e_message = e.getMessage();
      System.out.println("> IOException when trying to initialize the worker.\n> Cause: " + e_cause + ".\n> Message: " + e_message + ".\n");
    }
    System.out.println("\n> Worker successfully created!\n");
  }

  private void write_message(String message) throws IOException{
    try{
      this.out.write(message);
      this.out.newLine();
      this.out.flush();
    }catch(NullPointerException e){
      e.printStackTrace();
    }
  }

  private Boolean is_number(String line){
    try{ // este try é para evitar situações em que recebemos Strings q não são números.
      int number = Integer.parseInt(line);
      return true;
    }catch(NumberFormatException e){
      return false;
    }
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
      write_message("> Please enter your username:");
      username = this.in.readLine();
      if(username.trim().equals("") || username.trim().equals(null)){
        write_message("> Invalid username, please try again.");
      }else{
        Boolean user_exists = this.registos.user_exists(username);
        Boolean user_is_logged = this.registos.check_if_user_is_logged_in(username);
        if(user_is_logged){ // USUÁRIO JÁ ESTÁ LOGGADO
          write_message("> User already logged in. Please try again.");
          System.out.println("> User " + username + " is already logged in.\n");
        }else if(user_exists){ // LOGIN
          write_message("> Username already registered, insert password or leave empty to go back:");
          tries = 0;
          while(!step_completed && !go_back && tries < 3){
            String password = this.in.readLine();
            if(password.trim().equals("") || password.trim().equals(null)){
              write_message("> Returning to login screen...");
              go_back = true;
            }else{
              step_completed = this.registos.login_user(username, password);
              tries++;
              if(!step_completed && tries < 3){
                write_message("> Login failed, please try again.");
              }else if(!step_completed && tries >= 3){
                write_message("> Login failed after 3 tries.");
              }else if(step_completed){
                this.registos.set_user_to_logged_in(username,out);
                message_on_success = "\n> You are now logged in!\n> Welcome back " + username + ".\n";
              }
            }
          }
        }else if(user_exists == false){ // NOVO REGISTO
          write_message("> Username not found, insert password to register or leave empty to go back:");
          tries = 0;
          while(!step_completed && tries < 3){
            write_message("> Insert password:");
            String password = this.in.readLine();
            if(password.trim().equals("") || password.trim().equals(null)){
              write_message("> Returning to login screen...");
              go_back = true;
            }else{
              step_completed = this.registos.register_new_user(username, password, this.out);
              tries++;
              if(!step_completed && tries < 3){
                write_message("> Invalid password.");
              }else if(!step_completed && tries >= 3){
                write_message("> Signup failed after 3 tries.");
              }else if(step_completed){
                message_on_success = "> You have completed the signup!\n> Welcome " + username + ".\n";
              }
            }
          }
        }
      }
    }
    write_message(message_on_success);
    return username;
  }

  private void get_n_infected_and_update_users(String username) throws IOException{
    String line = null;

    while((line = this.in.readLine()) != null && !line.equals("quit")){
      try{
        Boolean success = false;
        int n_infected = -1;
        if(is_number(line)){ // este try é para evitar situações em que recebemos Strings q não são números.
          n_infected = Integer.parseInt(line);
        }else{
          write_message("> The number is in the wrong format, please try again:");
          continue; // salta o resto do loop de forma a recomeçar.
        }
        if(n_infected > 150){
          write_message("> The number of infected people you know cannot be greater than 150. Try again:");
          continue;
        }
        success = this.registos.set_casos_and_update_all_users(username, n_infected);
        if(success){
          write_message("> You can now update the number of infected people you know or quit by typing \"quit\":");
          System.out.println("> Step 2 successfully completed for user \"" + username + "\"\n");
        }else{
          write_message("> Failed to register the number of infected, please try again:");
          System.out.println("> Step 2 failed for user \"" + username + "\"\n");
        }
      }catch(IOException e){
        write_message("> Failed to register the number of infected, please try again:");
        e.printStackTrace();
        String e_cause = e.getCause().toString();
        String e_message = e.getMessage();
        System.out.println("> IOException when trying to register the number of infected people the user knows: \n> Cause: " + e_cause + ".\n> Message: " + e_message + ".\n");
      }
    }
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
      e.printStackTrace();
      String e_cause = e.getCause().toString();
      String e_message = e.getMessage();
      System.out.println("> IOException when trying to login/signup:\n> Cause: " + e_cause + ".\n> Message: " + e_message + ".\n");
    }

    /** End of Step 1. */
    if(username != null){
      System.out.println("> Step 1 completed for user \"" + username + "\".\n");
    }else{
      System.out.println("> Step 1 failed for user \"" + username + "\".\n");
    }

    /** Start of Step 2 - De seguida, o servidor aguarda que o cliente indique
    quantos casos de doença este conhece nos seus contactos (valor inteiro >= 0)
    */
    try{
      write_message("> Please insert the number of infected people you know:");
      get_n_infected_and_update_users(username);
    }catch(IOException e){ // apanha a excepção do "write_message" acima e do "readLine" no método acima.
      e.printStackTrace();
      String e_cause = e.getCause().toString();
      String e_message = e.getMessage();
      System.out.println("> IOException when trying to register the number of infected people the user knows: \n> Cause: " + e_cause + ".\n> Message: " + e_message + ".\n");
    }

    /** FINAL STEP - client disconnects */
    // fechar socket e respectivos canais.
    try{
      this.registos.set_user_to_logged_out(username);
      System.out.println("> Client \"" + username + "\" has disconnected. Connection is closed.\n");
      this.socket.shutdownOutput();
      this.socket.shutdownInput();
      this.socket.close();
    }catch(IOException e){
      e.printStackTrace();
      String e_cause = e.getCause().toString();
      String e_message = e.getMessage();
      System.out.println("> IOException when trying to close the socket:\n> Cause: " + e_cause + ".\n> Message: " + e_message + ".\n");
    }
  }
}
