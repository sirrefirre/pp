hungrybirds.c is a process where one producer fills a common resource and multiple consumers access it one by one.
It is compiled with "make bird" and run with "./bird babybirds worms" where default and max is 15 birds, 10 worms.
It is an unfair implementation where consumers might starve if they're unlucky.

bear.c is a process where multiple producers fill a common resource one by one and a single consumer empties it 
when it is full. It is compiled with "make bear" and run with "./bear bees potsize" where default and max is 15 bees,
10 potsize. It is an unfair implementation where producers might not be deliver if they're unlucky.