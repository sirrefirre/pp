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
    public void run(){
        try {
            //loop for number of repairs on this car
            for (int i = Begin.REPAIRS_PER_CAR; i > 0; i--) {
                //arrive at station for repair
                rs.arrive(type, id);
                //simulate repair time
                sleep((int) Math.ceil(Math.random() * 500) + 1000);
                //leave station
                rs.depart(type, id);
                //drive around until next repair is needed
                sleep((int) Math.floor(Math.random() * 2000) + 2000);
            }
        } catch(InterruptedException e){}
    }
}
