
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BufferSize 20
#define STATIC_BUFFER_SIZE 16384

void process_data(char* buffer, int bufferSizeInBytes) {

}
int get_external_data(char* buffer, int bufferSizeInBytes) {
	return 4;
}
//TODO Define global data structures to be used
typedef struct DataHolder
{
	char* buffer;
	int bufferSizeInBytes;
} DataHolder;

// Critical variables
DataHolder buffers[BufferSize];
int read_index = 0;
int write_index = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty;
sem_t full;

/**
 * This thread is responsible for pulling data off of the shared data 
 * area and processing it using the process_data() API.
 */
void *reader_thread(void *arg) {
	//TODO: Define set-up required
	// while(1) {
	for (int i = 0; i < 21; i ++){
		//TODO: Define data extraction (queue) and processing 

		int val;
		// CS
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		DataHolder cur_buffer = buffers[read_index];
		read_index = (read_index + 1) % BufferSize;
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		// Passing by value, no mutex anymore
		process_data(cur_buffer.buffer, cur_buffer.bufferSizeInBytes);
		free(cur_buffer.buffer);
	}
	
	return NULL;
}


/**
 * This thread is responsible for pulling data from a device using
 * the get_external_data() API and placing it into a shared area
 * for later processing by one of the reader threads.
 */
void *writer_thread(void *arg) {
	//TODO: Define set-up required
	char* temp_buffer = (char *) malloc(STATIC_BUFFER_SIZE);
	// while(1) {
	for(int i = 0; i < 3; i++){
		//TODO: Define data extraction (device) and storage 
		int ret = get_external_data(temp_buffer, STATIC_BUFFER_SIZE);
		if (ret < 0) {
			//Error happens when getting external data, we should raise error
			return NULL;
		}
		char* storage_ptr = (char *) malloc(ret);
		memcpy(storage_ptr, temp_buffer, ret);
		DataHolder buffer = {storage_ptr, ret};

		// CS
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		buffers[write_index] = buffer;
		write_index = (write_index + 1) % BufferSize;
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
	
	free(temp_buffer);
	return NULL;
}


#define M 1
#define N 2
int main(int argc, char **argv) {
	int i;
	pthread_t readers[N], writers[M];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BufferSize);
	sem_init(&full, 0, 0);
	for(i = 0; i < N; i++) { 
		pthread_create(&readers[i], NULL, reader_thread, NULL);
	}

	for(i = 0; i < M; i++) { 
		pthread_create(&writers[i], NULL, writer_thread, NULL);
	}

	for(i = 0; i < M; i++) {
		pthread_join(writers[i], NULL);
	}

	for(i = 0; i < N; i++) {
		pthread_join(readers[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);
	return 0;	
}