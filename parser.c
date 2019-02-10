#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

extern const char* keymap[];

int main(int argc, char *argv[])
{
	FILE * fp;
    int * buffer;
	unsigned int * deltas;
	struct timespec * times;	
	int s;

    if (argc == 1)
    {
        printf("Usage: %s log.bin\n", argv[0]); 
        exit(EXIT_SUCCESS);
    }

    if((fp=fopen(argv[1], "rb")) == 0)
    {
        fprintf(stderr,"fopen failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

	fread(&s, sizeof(int), 1, fp);
	printf("Total characters saved: %i \n\n", s);

	buffer = malloc(s*sizeof(int));
	deltas = malloc(s*sizeof(unsigned int));
	times = malloc(s*sizeof(struct timespec));

    fread(buffer, sizeof(int), s, fp);
	fread(times, sizeof(struct timespec), s, fp);
	fread(deltas, sizeof(unsigned int), s, fp);

	for(int i = 0; i < s; i++){
		int key = buffer[i];
		unsigned int d = deltas[i];
		struct timespec t = times[i];
	
		printf("Keypress %i (%s) at %lu:%lu with delta %u\n", key, keymap[key], t.tv_sec % 60, t.tv_nsec / 1000, d);
		

	}


	free(buffer);
	free(times);
	free(deltas);

    fclose(fp);
	return 0;
}
