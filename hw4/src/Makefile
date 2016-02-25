JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Vehicle.java \
    Begin.java \
    RepairStation.java

repair: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class