/**
 * Initiates vehicles based on initial parameters and
 * the monitor repairstation.
 *
 * run with "java Begin vehicles a b c"
 * compiled with "make"
 * Unfair implementation
 */

public class Begin {
    //max values
    private static int MAX_VEHICLES = 10;
    private static int MAX_A = 7;
    private static int MAX_B = 7;
    private static int MAX_C = 7;

    //input values
    public static int VEHICLES = MAX_VEHICLES;
    public static int A = MAX_A;
    public static int B = MAX_B;
    public static int C = MAX_C;
    public static int REPAIRS_PER_CAR = 5;

    //main
    public static void main(String args[]){
        int len = args.length;
        //set values if given
        if(len>0) VEHICLES = Integer.parseInt(args[0]);
        if(len>1) A = Integer.parseInt(args[1]);
        if(len>2) B = Integer.parseInt(args[2]);
        if(len>3) C = Integer.parseInt(args[3]);

        //limit input to max values
        if(VEHICLES > MAX_VEHICLES) VEHICLES = MAX_VEHICLES;
        if(A > MAX_A) A = MAX_A;
        if(B > MAX_B) B = MAX_B;
        if(C > MAX_C) C = MAX_C;
        //create monitor
        RepairStation rs = new RepairStation();
        //id to keep track of all vehicles
        int id = 0;
        //start vehicles type A
        for(int i = A; i > 0; i--){
            new Vehicle(rs, 'a', id++).start();
        }
        //start vehicles type B
        for(int i = B; i > 0; i--){
            new Vehicle(rs, 'b', id++).start();
        }
        //start vehicles type C
        for(int i = C; i > 0; i--){
            new Vehicle(rs, 'c', id++).start();
        }
    }
}
