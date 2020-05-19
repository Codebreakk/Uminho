import java.io.*;

public class Usuario{
  private String password;
  private int casos;
  private BufferedWriter bufferedwriter;
  private Boolean logged_in;


  public Usuario(BufferedWriter bufferedwriter, String password){
    this.password = password;
    this.casos = 0;
    this.bufferedwriter = bufferedwriter;
    this.logged_in = true;
  }

  public BufferedWriter get_bufferedwriter(){
    return this.bufferedwriter;
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
}
