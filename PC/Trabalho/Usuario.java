import java.io.*;

public class Usuario{
  private String password;
  private int casos;
  private BufferedWriter bufferedwriter;


  public Usuario(BufferedWriter bufferedwriter, String password){
    this.password = password;
    this.casos = 0;
    this.bufferedwriter = bufferedwriter;
  }

  public BufferedWriter get_bufferedwriter(){
    return this.bufferedwriter;
  }

  public String get_password(){
    return this.password;
  }
}
