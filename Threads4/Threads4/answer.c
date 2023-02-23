
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define BufferSize 20
#define STATIC_BUFFER_SIZE 16384
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
pthread_mutex_t mutex; 

/**
 * This thread is responsible for pulling data off of the shared data 
 * area and processing it using the process_data() API.
 */
void *reader_thread(void *arg) {
	//TODO: Define set-up required
	while(1) {
		//TODO: Define data extraction (queue) and processing 
		DataHolder cur_buffer = buffers[read_index];
		read_index ++;
		read_index = (read_index + 1) % BufferSize;
		// Passing by value, no mutex anymore
		process_data(cur_buffer.buffer, cur_buffer.bufferSizeInBytes);
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
	static char* temp_buffer = (char *) malloc(STATIC_BUFFER_SIZE);
	while(1) {
		//TODO: Define data extraction (device) and storage 
		int ret = get_external_data(temp_buffer, STATIC_BUFFER_SIZE)
		if (ret < 0) {
			//Error happens when getting external data, we should raise error
			return NULL
		}
		char* storage_ptr = (char *) malloc(ret);
		DataHolder buffer = {storage_ptr, ret};
		buffers[write_index] = buffer;
		write_index ++;
		write_index = (write_index + 1) % BufferSize;
	}
	
	free(temp_buffer);
	return NULL;
}


#define M 10
#define N 20
int main(int argc, char **argv) {
	int i;
	
	for(i = 0; i < N; i++) { 
		pthread_create(NULL, NULL, reader_thread, NULL);
	}

	for(i = 0; i < M; i++) { 
		pthread_create(NULL, NULL, writer_thread, NULL);
	}
	

	return 0;	
}