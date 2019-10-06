#include <stdlib.h>
#include <stdio.h>
#include "stats.h"

int *made, *eaten, *delaySize; // int pointers for candies made, eaten and delay duration
double **delay;				   // pointer to double pointer to simulate 2d array
int factories = 0;

// set sizes of int pointers
void stats_init(int num_producers){
	// all allocated memory sizes of factory amount
	made = malloc(num_producers*sizeof(int));
	eaten = malloc(num_producers*sizeof(int));
	delay = (double**)malloc(num_producers*sizeof(double));
	delaySize = malloc(num_producers*sizeof(int));
	factories = num_producers;
}

void stats_cleanup(void){
	// free all allocated memory
	free(made);
	free(eaten);
	free(delay);
	free(delaySize);
}

void stats_record_produced(int factory_number){
	// add one candy made to factory number
	made[factory_number]++;
}

void stats_record_consumed(int producer_number, double delay_in_ms){
	// add one candy eaten from factory number
	eaten[producer_number]++;

	// reallocate one more double memory size for every candy created
	delay[producer_number] = realloc(delay[producer_number], (delaySize[producer_number]+1) * sizeof(double));

	// record delay duration to factory
	delay[producer_number][delaySize[producer_number]] = delay_in_ms;

	// increase size of delay amounts in factory
	delaySize[producer_number]++;
}

void stats_display(void){
	// print statistics table
	printf("Statistics: \n");
	printf("   %8s %8s %8s  %13s %13s %13s\n", "#Factory", "#Made", "#Eaten", "Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");

	// double array for min, max and average delay duration
	double min[factories], max[factories], average[factories];

	// calculate min, max and average for each factory
	for (int x = 0; x < factories; x++){

		// loop through all delay amount for each factory
		for (int y = 0; y < delaySize[x]; y++){

			// if first delay in array, assign as min
			if (y == 0){
				average[x] = 0;
				min[x] = delay[x][y];
				max[x] = delay[x][y];
			}

			// add current delay to total
			average[x] += delay[x][y];
			
			// if at last delay, then divide by the total amount of delays
			if (y == delaySize[x]-1){
				average[x] = average[x]/delaySize[x];
			}



			// if current delay is lower, set as min
			if (min[x] > delay[x][y]){
				min[x] = delay[x][y];
			}

			// if current delay is higher, set as max
			if (max[x] < delay[x][y]){
				max[x] = delay[x][y];
			}
		}
	}

	// print all data
	for (int x = 0; x < factories; x++){
		printf("   %8d %8d %8d  %13.5f %13.5f %13.5f\n", x, made[x], eaten[x], min[x], average[x], max[x]);
	}

}