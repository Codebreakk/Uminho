import java.io.*;
import java.util.HashMap;
import java.util.HashSet;
import java.util.ArrayList;
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
  private int writer; // 1 se existir algum escritor a usar a secção critica
  private int readers; // >0 se existem leitores a usar a secção crítica
  private int writers_waiting;

  /** Contructor */
  public Registos(){
    this.user_list = new HashMap<String, Usuario>();
    this.lock = new ReentrantLock();
    this.reader_wait = this.lock.newCondition();
    this.writer_wait = this.lock.newCondition();
    this.writer = 0;
    this.readers = 0;
    this.writers_waiting = 0;
  }

  /** Read and Write Locks */
  private void writelock(){
    this.lock.lock();

    try{
      writers_waiting++;
      // Enquanto existir 1 writer, reader ou reader/writer activo ou reader/writers à espera:
      while(writer != 0 || readers > 0){
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
    writer_wait.signal();
    this.lock.unlock();
    System.out.println("> Write Lock Released.\n");
  }

  private void readlock(){
    this.lock.lock();
    try{
      // Enquanto existirem writers à espera ou 1 writer activo:
      while(writer != 0 || writers_waiting > 0){
        System.out.println("> Acquiring Read Lock...\n");
        reader_wait.await();
        readers++;
        System.out.println("> Read Lock Acquired.\n");
      }
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
      writer_wait.signal();
    }
    this.lock.unlock();
    System.out.println("> Read Lock Released.\n");
  }

  /** Métodos comuns entre os vários métodos públicos */
  private Usuario get_user(String username){
    Usuario usuario = this.user_list.get(username);
    return usuario;
  }

  private double get_estimate(Usuario usuario){
    return ((double) usuario.get_casos()/N_CONTACTOS);
  }

  private void write_message(BufferedWriter out, String message) throws IOException{
    try{
      out.write(message);
      out.newLine();
      out.flush();
    }catch(Exception e){
      e.printStackTrace();
    }
  }

  /** Métodos públicos a ser usados pelo Worker */

  /** TODO: REVER/ADICIONAR LOCKS como raio faço os locks disto?
  * readlock() na hashmap e writelock() no usuário (acho).
  */
  // READ(registos)/WRITE(usuario)
  public Boolean set_user_to_logged_in(String username,BufferedWriter out){
    Boolean result = false;

    this.readlock();
    try{
      Usuario usuario = user_list.get(username);
      usuario.lock();
      this.readunlock();
      usuario.set_logged_in(true);
      // O usuário acaba de se ligar novamente ao servidor, logo o socket é diferente
      usuario.set_bufferedwriter(out);
      usuario.unlock();
      result = usuario.get_logged_in();
    }finally{
      return result;
    }
  }

  /** TODO: REVER/ADICIONAR LOCKS como raio faço os locks disto?
  * readlock() na hashmap e writelock() no usuário (acho).
  */
  // READ(registos)/WRITE(usuario)
  public Boolean set_user_to_logged_out(String username){
    Boolean result = false;

    this.readlock();
    try{
      Usuario usuario = user_list.get(username);
      usuario.lock();
      this.readunlock();
      usuario.set_logged_in(false);
      result = !usuario.get_logged_in();
      usuario.unlock();
    }finally{
      return result;
    }
  }

  // READ ONLY
  public Boolean check_if_user_is_logged_in(String username){
    Boolean result = false;

    this.readlock();
    try{
      Usuario usuario = user_list.get(username);
      usuario.lock();
      this.readunlock();
      result = usuario.get_logged_in();
      usuario.unlock();
    }finally{
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
      this.readunlock();
    }finally{
      return result;
    }
  }

  /** TODO: REVER LOCKS e testar */
  // WRITE(usuario)
  public Boolean register_new_user(String username, String password, BufferedWriter bufferedwriter){
    Boolean result = false;

    System.out.println("> Registering new user...\n");
    this.writelock();
    try{
      Usuario usuario = new Usuario(bufferedwriter, password);
      Usuario verifying_usuario = this.user_list.put(username, usuario);
      // se o usuario não existia, então o put devolve null. Caso contrário,
      // devolve o usuário que ocupa esse espaço.
      result = (verifying_usuario == null);
    }finally{
      this.writeunlock();
      return result;
    }
  }

  // READ ONLY
  public Boolean login_user(String username, String password){
    Boolean result = false;

    this.readlock();
    try{
      Usuario usuario = get_user(username);
      usuario.lock();
      result = (password.equals(usuario.get_password()));
      usuario.unlock();
    }finally{
      this.readunlock();
      return result;
    }
  }

  /** TODO: REVER LOCKS como raio faço os locks disto?...
  * readlock() na hashmap para obter o usuario, writelock() no usuario para
  * escrever o número de casos. Mas depois como faço para, num só passo, fazer
  * writeunlock() do usuário e readlock() de todos os usuários?
  * VER AULA 3, PÁGINA 17: talvez se em vez de implementar RWlocks para os usu-
  * ários implementar apenas "Hand-over-hand locking" (Aula teórica 2, Locks,
  * p.19). Apenas podemos ter 1 thread a usar 1 usuário de cada vez, mas dificil-
  * mente teremos 2 threads a precisar do mesmo usuário. O único método que acede
  * a vários usuários ao mesmo tempo é este.
  */
  // READ(registos)/WRITE(usuario) AND READ(usuario)
  public Boolean set_casos_and_update_all_users(String username, int casos){
    Boolean result = false;
    double media = 0.0d;

    this.readlock();
    try{
      BufferedWriter out_usuario_actual = null;
      int n_users = this.user_list.size();
      ArrayList<BufferedWriter> array_out = new ArrayList<BufferedWriter>(n_users);

      for(Usuario usuario : this.user_list.values()){
        usuario.lock();
        if(get_user(username) == usuario){
          //
          usuario.set_casos(casos);
        }
        if(casos == usuario.get_casos()){
          out_usuario_actual = usuario.get_bufferedwriter();
          array_out.add(usuario.get_bufferedwriter());
          if(out_usuario_actual != null){ // só para proteger de NullPointerException...
            write_message(out_usuario_actual, "> The number of infected has been successfully registered. Thank you!");
            result = true;
          }else{
            write_message(out_usuario_actual, "Failed to update the number of infected you know. Please try again.");
          }
        }
        double casos_usuario = (double) get_estimate(usuario);
        if(casos_usuario < 0){
          n_users--;
        }else{
          media += casos_usuario;
        }
        usuario.unlock();
      }

      media = media/n_users;

      // Usamos BigDecimal para arredondar (a 2 casas) porque é o mais eficaz nestes cálculos.
      BigDecimal media_rounded = new BigDecimal(Double.toString(media));
      media_rounded = media_rounded.setScale(2, RoundingMode.HALF_UP);

      // 2º - enviamos a nova estimativa a todos os usuários.
      for(BufferedWriter out : array_out){
          if(out != null){ // só para proteger de NullPointerException...
            write_message(out, "\n> The estimate has been updated.\n> The new estimate of the average proportion of people infected is " + media_rounded + ".");
            result = true;
          }

      }
    }catch(IOException e){
      //TODO: melhorar o tratamento de excepções.
      e.printStackTrace();
    }finally{
      this.readunlock();
      return result;
    }
  }
}
