import java.io.*;
import java.util.HashMap;
import java.util.HashSet;
import java.util.concurrent.locks.ReentrantLock;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class Registos{
  private static final int N_CONTACTOS = 150;
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

  public Boolean set_user_to_logged_in(String username){
    Boolean result = false;

    this.lock.lock();
    try{
      Usuario usuario = user_list.get(username);
      usuario.set_logged_in(true);
      result = usuario.get_logged_in();
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public Boolean set_user_to_logged_out(String username){
    Boolean result = false;

    this.lock.lock();
    try{
      Usuario usuario = user_list.get(username);
      usuario.set_logged_in(false);
      result = !usuario.get_logged_in();
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public Boolean check_if_user_is_logged_in(String username){
    Boolean result = false;

    this.lock.lock();
    try{
      Usuario usuario = user_list.get(username);
      result = usuario.get_logged_in();
    }finally{
      this.lock.unlock();
      return result;
    }
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

  public Boolean set_casos(String username, int casos){
    Boolean result = false;

    this.lock.lock();
    try{
      Usuario usuario = get_user(username);
      usuario.set_casos(casos);
      result = (casos == usuario.get_casos());
    }finally{
      this.lock.unlock();
      return result;
    }
  }

  public double get_estimate(Usuario usuario){
    return ((double) usuario.get_casos()/N_CONTACTOS);
  }

  private void write_message(BufferedWriter out, String message) throws IOException{
    out.write(message);
    out.newLine();
    out.flush();
  }

  public void write_estimate_to_all_users(){
    double media = 0.0d;
    int n_users = this.user_list.size();

    this.lock.lock();
    try{
      /** São necessários 2 ciclos para enviar a estimativa a todos os usuários:
      *     1 ciclo para obter o total de casos e calcular a média;
      *     1 ciclo para enviar o resultado a todos os clientes;
      *   Para evitar que a estimativa fique desactualizada entre o calculo e o
      * envio da mensagem, devemos fazer tudo dentro de 1 só lock, de forma a
      * que nenhum usuário possa actualizar o seu número de casos sem primeiro
      * informar todos os usuários da nova média.
      */

      //TODO: será possível fazer tudo em 1 só ciclo usando variáveis de condi-
      //ção? (wait/notify/notifyAll)
      //calculo da media
      for(Usuario usuario : this.user_list.values()){
        media += get_estimate(usuario);
      }
      media = media/n_users;
      // Usamos BigDecimal para arredondar (a 2 casas) porque é o mais eficaz nestes cálculos.
      BigDecimal media_rounded = new BigDecimal(Double.toString(media));
      media_rounded = media_rounded.setScale(2, RoundingMode.HALF_UP);
      // Envio de mensagem com a nova estimativa.
      for(Usuario usuario : this.user_list.values()){
        BufferedWriter out = usuario.get_bufferedwriter();
        if(out != null){ // só para proteger de NullPointerException...
          write_message(out, "> The estimate has been updated.\n> The new estimate of the average proportion of people infected is " + media_rounded + ".");
        }
      }
    }catch(IOException e){
      e.printStackTrace();
    }finally{
      this.lock.unlock();
    }
  }
}
