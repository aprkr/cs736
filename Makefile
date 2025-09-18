all: latency.c
	gcc latency.c -o latency.out

clean:
	rm -rf *.out