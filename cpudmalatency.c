#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
	int32_t v;
	int fd;

	if (argc != 2) {		
		fprintf(stderr, "Usage: %s <latency [us]>\n", argv[0]);
		fprintf(stderr, "\n");
		fprintf(stderr, "	latency: the maximum tolerable CPU DMA you\n");
		fprintf(stderr, "	         are willing to put up with [in microseconds]\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "This program will block until you hit Ctrl-C, at which point\n");
		fprintf(stderr, "the file descriptor is closed and the latency requirement is\n");
		fprintf(stderr, "unregistered again.\n");
		fprintf(stderr, "Hint: if you have an platform with a buggy BIOS that has\n");
		fprintf(stderr, "issues with excessive C-state transition latencies try value 55\n");
		return 2;
	}

	v = atoi(argv[1]);
	printf("setting latency to %d.%.6d seconds\n", v/1000000, v % 1000000);
	fd = open("/dev/cpu_dma_latency", O_WRONLY);
	if (fd < 0) {
		perror("open /dev/cpu_dma_latency");		return 1;
	}	
	if (write(fd, &v, sizeof(v)) != sizeof(v)) {
		perror("write to /dev/cpu_dma_latency");
		return 1;
	}
	while (1)
		 sleep(10);
	return 0;
}
