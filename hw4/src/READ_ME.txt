Begin.java contains the main file to run the repairstation, it is depending on both
Vehicle.java and RepairStation.java. Vehicle.java is the objects started as threads and
RepairStation.java is a monitor that the objects use.
It is compiled with "make" and run with "java Begin vehicles a b c" where "vehicles a b c"
are optional. Default values are 10 vehicles, 7 a, 7 b, 7c.
It is an unfair implementation where unlucky vehicles might wait until other have finished.