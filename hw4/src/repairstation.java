import java.util.concurrent.TimeUnit;

public class repairstation {
    private int total;
    private int type_a = 0;
    private int type_b = 0;
    private int type_c = 0;

    public synchronized int arrive(char type){
        if(type == 'a'){
            if(type_a < begin.max_a){
                type_a++;
                try {
                    TimeUnit.SECONDS.sleep(2);
                }  catch(InterruptedException e){}
            }
        }else if(type == 'b'){
            if(type_b < begin.max_b){
                type_b++;
                try {
                    TimeUnit.SECONDS.sleep(2);
                }  catch(InterruptedException e){}
            }
        } else{
            if(type_c < begin.max_c){
                type_c++;
                try {
                    TimeUnit.SECONDS.sleep(2);
                }  catch(InterruptedException e){}
            }
        }
        if(total < begin.max_vehicles){
            try{
                wait();
            } catch(InterruptedException e){}
        }

        return 0;
    }
    public synchronized  int depart() {

    return 0;
    }
}
