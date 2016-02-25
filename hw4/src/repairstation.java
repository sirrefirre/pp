/**
 * monitor used by vehicle object
 */

public class repairstation {

    private int total = 0;
    private int type_a = 0;
    private int type_b = 0;
    private int type_c = 0;
    //vehicle arrives at station
    public synchronized void arrive(char type, int id){
        //check specific type first
        //if type is a
        if(type == 'a'){
            //if all slots for type a is occupied, wait
            while(type_a == (int)Math.ceil(begin.max_a/2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                 } catch(InterruptedException e){}
            }
            //take slot for type a
            type_a++;

        //if type is b
        }else if(type == 'b'){
            //if all slots for type b is occupied, wait
            while(type_b == (int)Math.ceil(begin.max_b/2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                } catch(InterruptedException e){}
            }
            //take slot for type b
            type_b++;
        //if type is c
        } else{
            //if all slots for type c is occupied, wait
            while(type_c == (int)Math.ceil(begin.max_c/2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                } catch(InterruptedException e){}
            }
            //take slot for type c
            type_c++;
        }
        //if all spots are occupied, wait
        while(total == begin.max_vehicles){
            try{
                System.out.println(id + " is waiting for vacant spot");
                wait();
            } catch(InterruptedException e){}
        }
        //increment total cars being repaired
        total++;
        System.out.println(id + " type " + type + " is repairing");
    }

    //departing vehicles leaving their spots
    public synchronized void depart(char type, int id) {
        //decrement counter for departing type
        if(type == 'a') {
            type_a--;
        }else if(type == 'b'){
            type_b--;
        }else{
            type_c--;
        }
        //decrement counter for total vehicles at station
        total--;
        System.out.println(id + " type " + type + " is leaving");
        //notify(wake up) one waiting vehicle
        notify();
    }
}
