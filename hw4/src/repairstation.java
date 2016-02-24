import java.util.concurrent.TimeUnit;

public class repairstation {

    private int total = 0;
    private int type_a = 0;
    private int type_b = 0;
    private int type_c = 0;

    public synchronized void arrive(char type){
        if(type == 'a'){
            while(type_a == begin.max_a){
                try{
                    wait();
                 } catch(InterruptedException e){}
            }
            type_a++;


        }else if(type == 'b'){
            while(type_b == begin.max_b){
                try{
                    wait();
                } catch(InterruptedException e){}
            }
            type_b++;

        } else{
            while(type_c == begin.max_c){
                try{
                    wait();
                } catch(InterruptedException e){}
            }
            type_c++;
        }

        while(total == begin.max_vehicles){
            try{
                wait();
            } catch(InterruptedException e){}
        }
        total++;
    }


    public synchronized void depart(char type) {
        try {
            //repairs under way
            TimeUnit.SECONDS.sleep((int)Math.ceil(Math.random()*3));


        }  catch(InterruptedException e){}
        if(type == 'a') {
            type_a--;
        }else if(type == 'b'){
            type_b--;
        }else{
            type_c--;
        }
        total--;
        notifyAll();
    }
}
