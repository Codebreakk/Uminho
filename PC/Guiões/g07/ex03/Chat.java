import java.io.*;
import java.util.HashMap;
import java.util.concurrent.locks.ReentrantLock;

public class Chat{
  private HashMap<String, BufferedWriter> user_list;
  private ReentrantLock lock;

  public Chat(){
    this.user_list = new HashMap<String, BufferedWriter>();
    this.lock = new ReentrantLock();
  }

  private Boolean add_user(String nickname, BufferedWriter bufferedwriter){
    BufferedWriter new_buffwriter = this.user_list.put(nickname, bufferedwriter);
    return get_user(nickname);
  }

  private Boolean get_user(String nickname){
    BufferedWriter bufferedwriter = this.user_list.get(nickname);
    return (bufferedwriter != null);
  }

  private Boolean remove_user(String nickname){
    BufferedWriter bufferedwriter = this.user_list.remove(nickname);
    return (bufferedwriter != null);
  }

  public Boolean register_new_user(String nickname, BufferedWriter bufferedwriter){
    Boolean result = false;
    BufferedWriter checking_bufferedwriter = null;

    this.lock.lock();
    try{
      if(!get_user(nickname)){ // true -> nickname não encontrado
        // nickname ainda não foi registado.
        System.out.println("> Nickname available, adding user...\n");
        result = add_user(nickname, bufferedwriter); // true -> registado com sucesso.

      }
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public void write_message(String line){
    this.lock.lock();

    try{
      for(BufferedWriter out : this.user_list.values()){
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

  public Boolean remove_disconnected_user(String nickname){
    Boolean result = false;

    this.lock.lock();
    try{
      if(get_user(nickname)){ // true -> nickname existe.
        result = remove_user(nickname); // true -> nickname removido.
      }
    }finally{
      this.lock.unlock();
      return result;
    }
  }
}
