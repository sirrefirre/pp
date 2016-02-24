/**
 * Created by Simon on 2016-02-24.
 */
class vehicle extends Thread{
    private repairstation rs;
    private int type;
    public vehicle(repairstation rs, int type){
        this.rs = rs;
        this.type = type;
    }


}
