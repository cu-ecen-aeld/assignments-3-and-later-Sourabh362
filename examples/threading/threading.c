#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)


void* threadfunc(void* thread_param)
{
    // Cast the parameter to thread_data structure
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    // Wait before attempting to obtain the mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);  // Convert milliseconds to microseconds

    // Attempt to obtain the mutex
    if (pthread_mutex_lock(thread_func_args->mutex) == 0) {
        DEBUG_LOG("Mutex obtained by thread.");

        // Wait for a specified time while holding the mutex
        usleep(thread_func_args->wait_to_release_ms * 1000);  // Convert milliseconds to microseconds

        // Release the mutex
        pthread_mutex_unlock(thread_func_args->mutex);
        DEBUG_LOG("Mutex released by thread.");

        // Set success flag
        thread_func_args->thread_complete_success = true;
    } else {
        ERROR_LOG("Failed to obtain mutex.");
        thread_func_args->thread_complete_success = false;
    }

    return (void*)thread_func_args;  // Return thread data structure for cleanup
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
    // Allocate memory for thread_data structure
    struct thread_data* data = malloc(sizeof(struct thread_data));
    if (data == NULL) {
        ERROR_LOG("Failed to allocate memory for thread_data.");
        return false;
    }

    // Setup mutex and wait arguments in the allocated structure
    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    data->thread_complete_success = false;  // Initialize success flag

    // Create the thread and pass the thread_data as an argument
    int result = pthread_create(thread, NULL, threadfunc, (void*)data);
    
    if (result != 0) {
        ERROR_LOG("Failed to create thread: %d", result);
        free(data);  // Clean up allocated memory on failure
        return false;
    }

    return true;  // Return true if successful
}