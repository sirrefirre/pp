/**
 * Created by Simon on 2016-02-24.
 */
class vehicle extends Thread {
    private repairstation rs;
    private char type;

    public vehicle(repairstation rs, char type) {
        this.rs = rs;
        this.type = type;
    }

    public void run() {
        for(int i = begin.repairs_per_car; i > 0; i--){
            rs.arrive(type);
            rs.depart(type);
            try {
                sleep((int)Math.floor(Math.random()*3) + 5);
            } catch(InterruptedException e) {}
        }
    }
}