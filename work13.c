#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

struct pop_entry {int year; int population; char boro[15];};

int main(int argc, char *argv[]) {
	if (argc == 2 && strcmp(argv[1], "-read_csv") == 0) {
		struct pop_entry data;
		struct stat info;
		stat("./population.csv", &info);
		int csv = open("population.csv", O_RDONLY);
		char line[info.st_size];
		int i = 0;
		int int_data[info.st_size];
		int buffer = 0;
		int x = 0;
		for (i = 0; i < info.st_size; i++) {
			read(csv, &(line[i - buffer]), 1);
			if (line[i] == '\n') {
				buffer = i;
				sscanf(line, "%d,%d,%d,%d,%d,%d", int_data[x], int_data[x+1], int_data[x+2], int_data[x+3], int_data[x+4], int_data[x+5]);
				x += 6;
				printf("%d,%d,%d,%d,%d,%d\n", int_data[x], int_data[x+1], int_data[x+2], int_data[x+3], int_data[x+4], int_data[x+5]);
			}
		}
	}
	return 0;
}
