import java.io.*;
import java.util.HashMap;
import java.util.concurrent.locks.ReentrantLock;

public class Registos{
  private HashMap<String, Usuario> user_list;
  private ReentrantLock lock;

  // Contructor
  public Registos(){
    this.user_list = new HashMap<String, Usuario>();
    this.lock = new ReentrantLock();
  }

  private Usuario get_user(String username){
    Usuario usuario = this.user_list.get(username);
    return usuario;
  }

  private Boolean add_user(String username, String password, BufferedWriter bufferedwriter){
    Usuario usuario = new Usuario(bufferedwriter, password);
    Usuario verifying_usuario = this.user_list.put(username, usuario);
    return (get_user(username) != null);
  }

  public Boolean user_exists(String username){
    Boolean result = false;

    this.lock.lock();
    try{
      if(get_user(username) != null){
        System.out.println("> Username already exists, waiting for client...");
        result = true;
      }else{
        System.out.println("> Username does not exist, waiting for client...");
      }
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public Boolean register_new_user(String username, String password, BufferedWriter bufferedwriter){
    Boolean result = false;

    this.lock.lock();
    try{
      result = add_user(username, password, bufferedwriter);
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public Boolean login_user(String username, String password){
    Boolean result = false;

    this.lock.lock();
    try{
      Usuario usuario = get_user(username);
      result = (password.equals(usuario.get_password()));
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public void write_message_to_all_users(String line){
    this.lock.lock();

    try{
      for(Usuario usuario : this.user_list.values()){
        BufferedWriter out = usuario.get_bufferedwriter();
        out.write(line);
        out.newLine();
        out.flush();
      }
    }catch(IOException e){
      e.printStackTrace();
    }finally{
      this.lock.unlock();
    }
  }
}
