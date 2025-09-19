all: latency.c throughput.c timing.c
	gcc latency.c -o latency.out
	gcc throughput.c -o throughput.out
	gcc timing.c -o timing.out

clean:
	rm -rf *.out