/**
 * vehicle object started by begin/main
 */
class Vehicle extends Thread {
    private RepairStation rs;
    private char type;
    private int id;

    //set local variables to input
    public Vehicle(RepairStation rs, char type, int id) {
        this.rs = rs;
        this.type = type;
        this.id = id;
    }
    //when object is started
    public void run() {
        //loop for number of repairs on this car
        for(int i = Begin.REPAIRS_PER_CAR; i > 0; i--){
            //arrive at station for repair
            rs.arrive(type, id);
            //simulate repair time
            try {
                sleep((int)Math.ceil(Math.random()*500) + 1000);
            } catch(InterruptedException e) {}
            //leave station
            rs.depart(type, id);
            //drive around until next repair is needed
            try {
                sleep((int)Math.floor(Math.random()*2000) + 2000);
            } catch(InterruptedException e) {}
        }
    }
}