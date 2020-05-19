import java.io.*;
import java.util.HashMap;
import java.util.HashSet;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class Registos{
  private static final int N_CONTACTOS = 150;
  private HashMap<String, Usuario> user_list;
  private ReentrantLock lock;
  private Condition reader_wait;
  private Condition writer_wait;
  private Condition reader_writer_wait;
  private int writer; // 1 se existir algum escritor a usar a secção critica
  private int readers; // >0 se existem leitores a usar a secção crítica
  private int reader_writer; // 1 se existe algum leitor e escritor a usar a secção crítica
  private int reader_writers_waiting;
  private int writers_waiting;

  /** Contructor */
  public Registos(){
    this.user_list = new HashMap<String, Usuario>();
    this.lock = new ReentrantLock();
    this.reader_wait = this.lock.newCondition();
    this.writer_wait = this.lock.newCondition();
    this.reader_writer_wait = this.lock.newCondition();
    this.writer = 0;
    this.readers = 0;
    this.reader_writer = 0;
    this.reader_writers_waiting = 0;
    this.writers_waiting = 0;
  }

  /** Read and Write Locks */
  private void writelock(){
    this.lock.lock();

    try{
      writers_waiting++;
      // Enquanto existir 1 writer ou reader/writer activo ou readers activos:
      while(writer != 0 || readers > 0 || reader_writers_waiting != 0){
        System.out.println("> Acquiring Write Lock...\n");
        writer_wait.await();
        System.out.println("> Write Lock Acquired.\n");
      }
      writers_waiting--;
      writer = 1;
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      this.lock.unlock();
    }
  }

  private void writeunlock(){
    this.lock.lock();
    System.out.println("> Releasing Write Lock...\n");
    writer = 0;
    reader_wait.signalAll();
    reader_writer_wait.signal();
    writer_wait.signal();
    this.lock.unlock();
    System.out.println("> Write Lock Released.\n");
  }

  private void readlock(){
    this.lock.lock();
    try{
      // Enquanto existirem writers à espera ou 1 writer activo:
      while(writer != 0 || writers_waiting > 0 || reader_writer != 0 || reader_writers_waiting > 0){
        System.out.println("> Acquiring Read Lock...\n");
        reader_wait.await();
        System.out.println("> Read Lock Acquired.\n");
      }
      readers++;
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      this.lock.unlock();
    }
  }

  private void readunlock(){
    this.lock.lock();
    System.out.println("> Releasing Read Lock...\n");
    readers--;
    if(readers == 0){
      reader_writer_wait.signal();
      writer_wait.signal();
    }
    this.lock.unlock();
    System.out.println("> Read Lock Released.\n");
  }

  private void readwritelock(){
    this.lock.lock();
    try{
      reader_writers_waiting++;
      while(writer != 0 || readers > 0 || writers_waiting != 0){
        System.out.println("> Acquiring Read/Write Lock...\n");
        reader_writer_wait.await();
        System.out.println("> Read/Write Lock Acquired.\n");
      }
      reader_writers_waiting--;
      reader_writer = 1;
    }catch(InterruptedException e){
      e.printStackTrace();
    }finally{
      this.lock.unlock();
    }
  }

  private void readwriteunlock(){
    this.lock.lock();
    System.out.println("> Releasing Read/Write Lock...\n");
    reader_writer = 0;
    reader_wait.signalAll();
    reader_writer_wait.signal();
    writer_wait.signal();
    this.lock.unlock();
    System.out.println("> Read/Write Lock Released.\n");
  }

  /** Métodos comuns entre os vários métodos públicos */
  private Usuario get_user(String username){
    Usuario usuario = this.user_list.get(username);
    return usuario;
  }

  private Boolean add_user(String username, String password, BufferedWriter bufferedwriter){
    Usuario usuario = new Usuario(bufferedwriter, password);
    Usuario verifying_usuario = this.user_list.put(username, usuario);
    return (get_user(username) != null);
  }

  private double get_estimate(Usuario usuario){
    return ((double) usuario.get_casos()/N_CONTACTOS);
  }

  private void write_message(BufferedWriter out, String message) throws IOException{
    out.write(message);
    out.newLine();
    out.flush();
  }

  /** Métodos públicos a ser usados pelo Worker */
  // READ/WRITE
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

  // READ/WRITE
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

  // READ ONLY
  public Boolean check_if_user_is_logged_in(String username){
    Boolean result = false;

    try{
      this.readlock();
      Usuario usuario = user_list.get(username);
      result = usuario.get_logged_in();
    }finally{
      this.readunlock();
      return result;
    }
  }

  // READ ONLY
  public Boolean user_exists(String username){
    Boolean result = false;

    try{
      this.readlock();
      if(get_user(username) != null){
        System.out.println("> Username already exists, waiting for client...");
        result = true;
      }else{
        System.out.println("> Username does not exist, waiting for client...");
      }
    }finally{
      this.readunlock();
      return result;
    }
  }

  // READ/WRITE
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

  // READ ONLY
  public Boolean login_user(String username, String password){
    Boolean result = false;

    try{
      this.readlock();
      Usuario usuario = get_user(username);
      result = (password.equals(usuario.get_password()));
    }finally{
      this.readunlock();
      return result;
    }
  }

  // READ/WRITE
  public Boolean set_casos_and_update_all_users(String username, int casos){
    Boolean result = false;
    double media = 0.0d;

    try{
      this.readwritelock();
      // actualiza o número de casos que este usuário conhece.
      Usuario usuario_actual = get_user(username);
      usuario_actual.set_casos(casos);

      // se a actualização correu com sucesso:
      if(casos == usuario_actual.get_casos()){
        BufferedWriter out_usuario_actual = usuario_actual.get_bufferedwriter();
        if(out_usuario_actual != null){ // só para proteger de NullPointerException...
          write_message(out_usuario_actual, "> The number of infected has been successfully registered. Thank you!");
        }
        // 1º - calculamos a nova estimativa da proporção média.
        int n_users = this.user_list.size();
        for(Usuario usuario : this.user_list.values()){
          double casos_usuario = (double) get_estimate(usuario);
          if(casos_usuario <= 0){
            n_users--;
          }else{
            media += casos_usuario;
          }
        }
        media = media/n_users;
        // Usamos BigDecimal para arredondar (a 2 casas) porque é o mais eficaz nestes cálculos.
        BigDecimal media_rounded = new BigDecimal(Double.toString(media));
        media_rounded = media_rounded.setScale(2, RoundingMode.HALF_UP);

        // 2º - enviamos a nova estimativa a todos os usuários.
        for(Usuario usuario : this.user_list.values()){
          BufferedWriter out = usuario.get_bufferedwriter();
          if(out != null){ // só para proteger de NullPointerException...
            write_message(out, "\n> The estimate has been updated.\n> The new estimate of the average proportion of people infected is " + media_rounded + ".");
            result = true;
          }
        }
      }
    }catch(IOException e){
      //TODO: melhorar o tratamento de excepções.
      e.printStackTrace();
    }finally{
      this.readwriteunlock();
      return result;
    }
  }
}
