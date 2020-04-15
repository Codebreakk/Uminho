import java.util.*;

public class Warehouse{
  private HashMap<String,Item> stock;

  public Warehouse(){
    this.stock = new HashMap<String, Item>();
  }

  public void supply(String item, int quantity){
    if(this.stock.containsKey(item)){
      Item wh_item = this.stock.get(item);
      wh_item.supply(quantity);
    }
  }

  public void consume(String[] items){
    for(int i = 0; i < items.length; i++){
      if(this.stock.containsKey(items[i])){
        Item wh_item = this.stock.get(item);
        wh_item.consume();
      }
    }
  }
}
