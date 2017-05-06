CC = mpicc
CFLAGS = -c -Wall -Iinclude
OBJ = types.o operations.o simulation.o

simulation: $(OBJ) 
	$(CC) $(OBJ) -o $@

types.o: src/types.c include/types.h
	$(CC) $(CFLAGS) $< -o $@

operations.o: src/operations.c include/operations.h
	$(CC) $(CFLAGS) $< -o $@

simulation.o: src/simulation.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o simulation
