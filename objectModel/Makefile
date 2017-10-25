CC = g++
CFLAGS = -c -Wall -lGL -lGLU -lglut -Iinclude
OBJ = types.o simulation.o visualizer.o

simulation: $(OBJ) 
	$(CC) $(OBJ) -o $@ -lGL -lGLU -lglut
	rm *.o

types.o: src/types.cpp include/types.h
	$(CC) $(CFLAGS) $< -o $@

visualizer.o: src/visualizer.cpp include/visualizer.h
	$(CC) $(CFLAGS) $< -o $@ 	

simulation.o: src/simulation.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm simulation
