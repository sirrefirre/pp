/**
 * Created by Simon on 2016-02-24.
 */

public class begin {
    //max values
    private static int MAX_V = 10;
    private static int MAX_A = 7;
    private static int MAX_B = 7;
    private static int MAX_C = 7;

    //input values
    public static int max_vehicles = MAX_V;
    public static int max_a = MAX_A;
    public static int max_b = MAX_B;
    public static int max_c = MAX_C;
    public static int repairs_per_car = 5;

    //main
    public static void main(String args[]){
        int len = args.length;
        //set values if given
        if(len>0) max_vehicles = Integer.parseInt(args[0]);
        if(len>1) max_a = Integer.parseInt(args[1]);
        if(len>2) max_b = Integer.parseInt(args[2]);
        if(len>3) max_c = Integer.parseInt(args[3]);

        //limit to max values
        if(max_vehicles > MAX_V) max_vehicles = MAX_V;
        if(max_a > MAX_A) max_a = MAX_A;
        if(max_b > MAX_B) max_b = MAX_B;
        if(max_c > MAX_C) max_c = MAX_C;

        repairstation rs = new repairstation();
        int id = 0;
        for(int i =  max_a; i > 0; i--){
            new vehicle(rs, 'a', id++).start();
        }
        for(int i =  max_b; i > 0; i--){
            new vehicle(rs, 'b', id++).start();
        }
        for(int i =  max_c; i > 0; i--){
            new vehicle(rs, 'c', id++).start();
        }
    }
}
