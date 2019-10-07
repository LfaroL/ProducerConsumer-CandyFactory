#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "bbuff.h"
#include "stats.h"

#define TRUE 1
#define FALSE 0

int factories;				   // int variable for factories
_Bool stop_thread = FALSE; 	   // bool for stop_thread

// candy structure
typedef struct {
	int source_thread;
	double time_stamp_in_ms;
} candy_t;

// function to get time in millisecond for when function is called
double current_time_in_ms(void){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

// function called for all factory threads
void *factoryThread(void *ptr){
	// obtain factory number from *ptr
	int source = *(int*)ptr;

	// seed for random time
	srand(time(NULL));
	
	// while stop_thread is false
	while (!stop_thread){
		// random number between 0 to 3
		int sleep_time = rand() % 4;

		// print which factory made a candy
		printf("\t Factory %d ship candy and wait %d s\n", source, sleep_time);

		// initialize new candy_t variable
		candy_t *candy = malloc(sizeof(candy_t));
		// set source thread to factory number
		candy->source_thread = source;
		// set to time in milliseconds
		candy->time_stamp_in_ms = current_time_in_ms();
		
		// insert candy
		bbuff_blocking_insert(candy);

		// record candy made
		stats_record_produced(source);

		// sleep for random amount of seconds
		sleep(sleep_time);

		// free all candy_t variables dynamically created
		free(candy);

		// print that factory is done
		printf("\t Candy-factory %d done\n", source);
	}

	// exit thread: return NULL
	pthread_exit(NULL);
}

// function called for all kid threads
void *kidThread(void *ptr){
	int sleep_time;

	// remove candy
	while (TRUE) {
		sleep_time = rand() % 2;

		// get candy from buffer
		candy_t* candy = bbuff_blocking_extract();

		// print which factory candy is being taken from
		printf("Candy from factory %d has been taken.\n", candy->source_thread);

		// record candy consumed and delay
		stats_record_consumed(candy->source_thread, current_time_in_ms() - candy->time_stamp_in_ms);
	
		sleep(sleep_time);
	}
	// exit thread
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){

	// if only the program is called with no arguments passed, throw error
	if (argc == 1){
		printf("Error, no arguments passed.\n");
		return 1;
	}

	// if less than or more arguments are passed than 4, throw error
	if (argc != 4){
		printf("Error, only three argument are accepted.\n");
		return 1;
	}

	// convert second argument to int for factories
	factories = atoi(argv[1]);
	// convert third argument to int for kids
	int kids = atoi(argv[2]);
	// convert fourth argument to int for seconds
	int seconds = atoi(argv[3]);

	// if at least one of the inputs is negative or 0, throw error
	if (factories <= 0 || kids <= 0 || seconds <= 0){
		printf("Error, all inputs must be a positive number.\n");
		return 1;
	}

	// initailize semaphores and all dynamically created variables
	bbuff_init();
	stats_init(factories);

	// factory threads array
	pthread_t tFact[factories];

	// factory ID array
	int tID[factories];
	for (int x = 0; x < factories; x++) {
		tID[x] = x;
	}

	// loop for same amount of factories
	for (int x = 0; x < factories; x++){
		// create thread with ID passed as the last argument
		pthread_create(&tFact[x], NULL, &factoryThread, &tID[x]);
	}

	// kid threads array
	pthread_t tKids[kids];

	// loop for same amount of kids
	for (int x = 0; x < kids; x++){
		// create kid thread
		pthread_create(&tKids[x], NULL, &kidThread, NULL);
	}

	// wait one second for the amount specified
	for (int x = 0; x < seconds; x++){
		printf("Time %ds: \n", x);
		sleep(1);
	}

	// stop all factory threads when done waiting
	stop_thread = TRUE;

	// print that all factories are stopping
	printf("Stopping candy factories.\n");

	// loop for same amount of factories
	for (int x = 0; x < factories; x++){
		// join factory thread
		pthread_join(tFact[x], NULL);
	}

	// while there is still candy in factory
	while(bbuff_is_data_available()){

		printf("Waiting for candies to be consumed.\n");

		// sleep one second
		sleep(1);
	}

	// print that all kids are stopping
	printf("Stopping kids.\n\n");

	// loop for same amount of kids
	for (int x = 0; x < kids; x++){
		// cancel kid thread
		pthread_cancel(tKids[x]);
		// join kid thread
		pthread_join(tKids[x], NULL);
	}

	// display candy factories information
	stats_display();

	// free all memory
	stats_cleanup();

	// end program
	return 0;
}