import java.util.concurrent.TimeUnit;

public class repairstation {

    private int total = 0;
    private int type_a = 0;
    private int type_b = 0;
    private int type_c = 0;

    public synchronized void arrive(char type, int id){
        if(type == 'a'){
            while(type_a == (int)Math.ceil(begin.max_a/2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                 } catch(InterruptedException e){}
            }
            type_a++;


        }else if(type == 'b'){
            while(type_b == (int)Math.ceil(begin.max_b/2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                } catch(InterruptedException e){}
            }
            type_b++;

        } else{
            while(type_c == (int)Math.ceil(begin.max_c/2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                } catch(InterruptedException e){}
            }
            type_c++;
        }

        while(total == begin.max_vehicles){
            try{
                System.out.println(id + " is waiting for vacant spot");
                wait();
            } catch(InterruptedException e){}
        }
        total++;
        System.out.println(id + " type " + type + " is repairing");
    }


    public synchronized void depart(char type, int id) {
        /*
        try {
            //repairs under way
            TimeUnit.SECONDS.sleep((int)Math.ceil(Math.random()*3));


        }  catch(InterruptedException e){}
        */
        if(type == 'a') {
            type_a--;
        }else if(type == 'b'){
            type_b--;
        }else{
            type_c--;
        }
        total--;
        System.out.println(id + " type " + type + " is leaving");
        notify();
    }
}
