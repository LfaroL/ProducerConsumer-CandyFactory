#include <pthread.h>
#include <semaphore.h>
#include "bbuff.h"

#define TRUE 1
#define FALSE 0

void* buffer[QUEUE_SIZE];   // candy array set to 10

int counter = 0;	// counter start at 0

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex variable
sem_t empty, full; 	// sempahore variables

// initialize semaphores
void bbuff_init(void){
	// empty semaphore set to queue size
	sem_init(&empty, 0, QUEUE_SIZE);

	// full semaphore set to 0
	sem_init(&full, 0, 0);
}

// add candy
void bbuff_blocking_insert(void* item){
	// decrement one to empty, if 0 then semaphore is blocked
	sem_wait(&empty);

	// thread locks mutex before adding
	pthread_mutex_lock(&mutex);

	// add candy to buffer
	buffer[counter] = item;
	// increment counter
	counter++;

	// thread unlocks mutex when finished
	pthread_mutex_unlock(&mutex);
	// increment one to semaphore full
	sem_post(&full);
}

// remove candy
void* bbuff_blocking_extract(void){
	void* candy;

	// decrement one to full, if 0 then semaphore is blocked
	sem_wait(&full);

	// thread locks mutex before extracting
	pthread_mutex_lock(&mutex);

	// decrement one
	counter--;
	candy = buffer[counter];

	// thread unlocks mutex after extracting
	pthread_mutex_unlock(&mutex);

	// increment one to semaphore empty
	sem_post(&empty);

	return candy;
}

// check if there is still candy in factories
_Bool bbuff_is_data_available(void){
	if (counter > 0){
		return TRUE;
	}
	return FALSE;
}