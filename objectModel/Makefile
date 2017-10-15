CC = g++
CFLAGS = -c -Wall -Iinclude
OBJ = types.o simulation.o

simulation: $(OBJ) 
	$(CC) $(OBJ) -o $@
	rm *.o

types.o: src/types.cpp include/types.h
	$(CC) $(CFLAGS) $< -o $@

# operations.o: src/operations.c include/operations.h
# 	$(CC) $(CFLAGS) $< -o $@

simulation.o: src/simulation.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm simulation
