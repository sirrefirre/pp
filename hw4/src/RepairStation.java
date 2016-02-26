/**
 * monitor used by vehicle object
 */

public class RepairStation {

    private int total = 0;
    private int typeA = 0;
    private int typeB = 0;
    private int typeC = 0;
    //vehicle arrives at station
    public synchronized void arrive(char type, int id) throws InterruptedException {
        //check specific type first
        //if type is A
        if(type == 'a'){
            //if all slots for type A is occupied, wait
            while(typeA == (int)Math.ceil(Begin.A /2) || total == Begin.VEHICLES){
                System.out.println(id + " is waiting for vacant " + type);
                wait();
            }
            //take slot for type A
            typeA++;

        //if type is B
        }else if(type == 'b'){
            //if all slots for type B is occupied, wait
            while(typeB == (int)Math.ceil(Begin.B /2) || total == Begin.VEHICLES){
                System.out.println(id + " is waiting for vacant " + type);
                wait();
            }
            //take slot for type B
            typeB++;

        //if type is C
        } else{
            //if all slots for type C is occupied, wait
            while(typeC == (int)Math.ceil(Begin.C /2) || total == Begin.VEHICLES){
                System.out.println(id + " is waiting for vacant " + type);
                wait();
            }
            //take slot for type C
            typeC++;
        }
        //vehicle has taken a spot
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
        //notify(wake up) all waiting vehicle
        notifyAll();
    }
}
