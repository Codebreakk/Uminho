import java.io.*;
import java.util.concurrent.locks.ReentrantLock;

public class Usuario{
  private String password;
  private int casos;
  private BufferedWriter bufferedwriter;
  private Boolean logged_in;
  public ReentrantLock locker;



  public Usuario(BufferedWriter bufferedwriter, String password){
    this.password = password;
    this.casos = 0;
    this.bufferedwriter = bufferedwriter;
    this.logged_in = true;
    this.locker = new ReentrantLock();
  }

  public BufferedWriter get_bufferedwriter(){
    return this.bufferedwriter;
  }

  public void set_bufferedwriter(BufferedWriter out){
    this.bufferedwriter = out;
  }

  public String get_password(){
    return this.password;
  }

  public Boolean get_logged_in(){
    return this.logged_in;
  }

  public void set_logged_in(Boolean status){
    this.logged_in = status;
  }

  public void set_casos(int casos){
    this.casos = casos;
  }
  public int get_casos(){
    return this.casos;
  }

  public void lock() {
      this.locker.lock();
  }

  public void unlock() {
      this.locker.unlock();
  }

}
