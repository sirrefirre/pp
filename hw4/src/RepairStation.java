/**
 * monitor used by vehicle object
 */

public class RepairStation {

    private int total = 0;
    private int typeA = 0;
    private int typeB = 0;
    private int typeC = 0;
    //vehicle arrives at station
    public synchronized void arrive(char type, int id){
        //check specific type first
        //if type is A
        if(type == 'a'){
            //if all slots for type A is occupied, wait
            while(typeA == (int)Math.ceil(Begin.A /2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                 } catch(InterruptedException e){}
            }
            //take slot for type A
            typeA++;

        //if type is B
        }else if(type == 'b'){
            //if all slots for type B is occupied, wait
            while(typeB == (int)Math.ceil(Begin.B /2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                } catch(InterruptedException e){}
            }
            //take slot for type B
            typeB++;
        //if type is C
        } else{
            //if all slots for type C is occupied, wait
            while(typeC == (int)Math.ceil(Begin.C /2)){
                try{
                    System.out.println(id + " is waiting for vacant " + type);
                    wait();
                } catch(InterruptedException e){}
            }
            //take slot for type C
            typeC++;
        }
        //if all spots are occupied, wait
        while(total == Begin.VEHICLES){
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
            typeA--;
        }else if(type == 'b'){
            typeB--;
        }else{
            typeC--;
        }
        //decrement counter for total vehicles at station
        total--;
        System.out.println(id + " type " + type + " is leaving");
        //notify(wake up) one waiting vehicle
        notify();
    }
}
