CC = gcc
CFLAGS = -lm -O2

solution: Prim.c Benchmark.c Tests.c Solution.c
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm solution
