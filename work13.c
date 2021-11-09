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
		struct stat info;
		int csv = open("population.csv", O_RDWR);
		lseek(csv, -1, SEEK_END);
		char end;
		read(csv, &end, 1);
		if (end != '\n') {
			end = '\n';
			write(csv, &end, 1);
		}
		lseek(csv, 0, SEEK_SET);
		stat("./population.csv", &info);
		struct pop_entry data[info.st_size];
		char line[info.st_size];
		int i = 0;
		int buffer = 0;
		int x = 0;
		int file = open("bpop.data", O_WRONLY | O_CREAT | O_TRUNC, 0744);
		int size = 0;
		for (i = 0; i < info.st_size; i++) {
			read(csv, &(line[i - buffer]), 1);
			if (line[i - buffer] == '\n') {
				line[i - buffer + 1] = '\0';
				//printf("%s", line);
				if (buffer > 0) {
					sscanf(line, "%d,%d,%d,%d,%d,%d", &(data[x].year), &(data[x].population), &(data[x+1].population), &(data[x+2].population), &(data[x+3].population), &(data[x+4].population));
					data[x+1].year = data[x].year;
					data[x+2].year = data[x].year;
					data[x+3].year = data[x].year;
					data[x+4].year = data[x].year;
					strcpy(data[x].boro, "Manhattan");
					strcpy(data[x+1].boro, "Brooklyn");
					strcpy(data[x+2].boro, "Queens");
					strcpy(data[x+3].boro, "Bronx");
					strcpy(data[x+4].boro, "Staten Island");
					int len = 9;
					size += write(file, &len, 4);
					size += write(file, &(data[x].year), 4);
					size += write(file, &(data[x].boro), 9);
					size += write(file, &(data[x].population), 4);
					len = 8;
					size += write(file, &len, 4);
					size += write(file, &(data[x+1].year), 4);
					size += write(file, &(data[x+1].boro), 8);
					size += write(file, &(data[x+1].population), 4);
					len = 6;
					size += write(file, &len, 4);
					size += write(file, &(data[x+2].year), 4);
					size += write(file, &(data[x+2].boro), 6);
					size += write(file, &(data[x+2].population), 4);
					len = 5;
					size += write(file, &len, 4);
					size += write(file, &(data[x+3].year), 4);
					size += write(file, &(data[x+3].boro), 5);
					size += write(file, &(data[x+3].population), 4);
					len = 14;
					size += write(file, &len, 4);
					size += write(file, &(data[x+4].year), 4);
					size += write(file, &(data[x+4].boro), 14);
					size += write(file, &(data[x+4].population), 4);
					x += 5;
				}
				buffer = i + 1;
			}
		}
		close(csv);
		close(file);
		printf("wrote %d bytes to bpop.data\n", size);
	}
	else if (argc == 2 && strcmp(argv[1], "-read_data") == 0) {
		int file = open("bpop.data", O_RDONLY);
		struct stat info;
		stat("./bpop.data", &info);
		struct pop_entry *data = malloc(info.st_size * sizeof(struct pop_entry));
		int i = 0;
		int len;
		int reallocsize = 0;
		while (i < info.st_size){
			read(file, &len, 4);
			read(file, &(data[reallocsize].year), 4);
			read(file, &(data[reallocsize].boro), len);
			read(file, &(data[reallocsize].population), 4);
			i += 4 + len + 4 + 4;
			reallocsize += 1;
		}
		data = realloc(data, reallocsize * sizeof(struct pop_entry));
		int row = 0;
		for (i = 0; i < reallocsize; i++) {
			printf("%d) Year: %d\tBorough: %s\tpop: %d\n", i, data[i].year, data[i].boro, data[i].population);
		}
		free(data);
	}
	else if (argc == 2 && strcmp(argv[1], "-add_data") == 0) {
		char *readin = malloc(500);
		struct pop_entry data;
		int file = open("bpop.data", O_APPEND | O_WRONLY);
		printf("Enter new value year borough population: ");
		fgets(readin, 500, stdin);
		sscanf(readin, "%d %s %d", &(data.year), data.boro, &(data.population));
		int len = strlen(data.boro);
		write(file, &len, 4);
		write(file, &(data.year), 4);
		write(file, &(data.boro), len);
		write(file, &(data.population), 4);
		free(readin);
		printf("Appended data to file: year: %d\tboro: %s\tpop: %d\n", data.year, data.boro, data.population);
	}
	else if (argc == 2 && strcmp(argv[1], "-update_data") == 0) {
		int file = open("bpop.data", O_RDWR);
		struct stat info;
		stat("./bpop.data", &info);
		struct pop_entry *data = malloc(info.st_size * sizeof(struct pop_entry));
		int i = 0;
		int len;
		int reallocsize = 0;
		while (i < info.st_size){
			read(file, &len, 4);
			read(file, &(data[reallocsize].year), 4);
			read(file, &(data[reallocsize].boro), len);
			read(file, &(data[reallocsize].population), 4);
			i += 4 + len + 4 + 4;
			reallocsize += 1;
		}
		data = realloc(data, reallocsize * sizeof(struct pop_entry));
		close(file);
		open("bpop.data", O_RDWR | O_TRUNC);
		printf("entry to update: ");
		char *readin = malloc(500);
		fgets(readin, 500, stdin);
		int entry;
		sscanf(readin, "%d", &entry);
		printf("Enter new value year borough population: ");
		fgets(readin, 500, stdin);
		printf("%s%d\n", readin, entry);
		sscanf(readin, "%d %s %d", &(data[entry].year), data[entry].boro, &(data[entry].population));
		for (i = 0; i < reallocsize; i++) {
			len = strlen(data[i].boro);
			write(file, &len, 4);
			write(file, &(data[i].year), 4);
			write(file, &(data[i].boro), len);
			write(file, &(data[i].population), 4);
		}
		close(file);
		free(data);
		printf("File updated\n");
	}
	return 0;
}
