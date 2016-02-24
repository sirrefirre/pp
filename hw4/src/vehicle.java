/**
 * Created by Simon on 2016-02-24.
 */
class vehicle extends Thread {
    private repairstation rs;
    private char type;
    private int id;

    public vehicle(repairstation rs, char type, int id) {
        this.rs = rs;
        this.type = type;
        this.id = id;
    }

    public void run() {
        for(int i = begin.repairs_per_car; i > 0; i--){
            rs.arrive(type, id);
            try {
                sleep((int)Math.ceil(Math.random()*500) + 1000);
            } catch(InterruptedException e) {}
            rs.depart(type, id);
            try {
                sleep((int)Math.floor(Math.random()*2000) + 2000);
            } catch(InterruptedException e) {}
        }
    }
}