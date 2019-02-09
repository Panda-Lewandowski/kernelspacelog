#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

extern keycode codes[];

void print_char(int code) {
    int i;

    if((code < FIRST_CD || code > LAST_CD) && code != KEY_SPACE)
    {
        for(i = 0; i < sizeof(codes)/sizeof(keycode); i++)   
            if (codes[i].code == code)
                printf("%s", codes[i].string);
    }
    else
        printf("%c",ch_table[(code-FIRST_CD)]);
}

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
	
		printf("Keypress %i at %lu:%lu with delta %u\n", key, t.tv_sec % 60, t.tv_nsec / 1000, d);

	}

	free(buffer);
	free(times);
	free(deltas);

    fclose(fp);
	return 0;
}
