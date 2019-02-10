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
	float speed;
	float av_delta;
	float av_pause;
	float buff_time;
	float frequencies[KEYS_NUM] = {0};

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
	

	buffer = malloc(s * sizeof(int));
	deltas = malloc(s * sizeof(unsigned int));
	times = malloc(s * sizeof(struct timespec));

    fread(buffer, sizeof(int), s, fp);
	fread(times, sizeof(struct timespec), s, fp);
	fread(deltas, sizeof(unsigned int), s, fp);

	for(int i = 0; i < s; i++){
		int key = buffer[i];
		unsigned int d = deltas[i];
		struct timespec t = times[i];
		if(key > 126){
			printf("UNKNOWN CODE: %i", key);
		}else
			frequencies[key] += 1;
	
		printf("Keypress %i (%s) at %lu:%lu with delta %u\n", key, keymap[key], t.tv_sec % 60, t.tv_nsec / 1000, d);
		
	}

	printf(" _______________________________\n");
	printf("| The frequency of occurrence   |\n");
	printf("|      of each character        |\n");
	printf("|_______________________________|\n");
	printf("|  num |      key     |  freq   |\n");
	printf("|______|______________|_________|\n");

	for(int j = 0; j < KEYS_NUM; j++){
		frequencies[j] = frequencies[j]*100/s;
		if(frequencies[j] != 0){
			printf("| %4i | %12s | %7.3f |\n", j, keymap[j], frequencies[j]);
		}
	}

	printf("|______|______________|_________|\n");
	printf("\n\n");
	printf("Total characters saved: %i \n", s);

	speed = times[s-1].tv_sec - times[0].tv_sec;
	speed += (60000 - (times[s-1].tv_nsec + times[0].tv_nsec)/1000000)/1000;
	printf("Print speed: %.3f symbols/sec.\n", speed/s);

	av_delta = 0;
	for(int k = 0; k < s; k++){
		av_delta += deltas[k];
	}
	printf("Average time pressed: %0.3f seconds.\n", av_delta/s/1000);

	av_pause = 0;
	for(int l = 1; l < s; l++){
		buff_time = (60000 - (times[l-1].tv_nsec + times[l].tv_nsec)/1000000) + deltas[l-1];
		av_pause += times[l-1].tv_sec - times[l].tv_sec + buff_time/1000;
		
	}
	printf("The average pause between keypresses: %0.3f seconds.\n", av_pause/s);

	free(buffer);
	free(times);
	free(deltas);

    fclose(fp);
	return 0;
}
