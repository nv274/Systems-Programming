#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define NUM_TASKS 5
#define NUM_ITERATIONS 50
#define NUM_ALLOCS 120

void task1() {
    struct timeval start, end;
    long total_time = 0;
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        gettimeofday(&start, NULL);
        
        for (int j = 0; j < 120; j++) {
            char* ptr = (char*)malloc(1);
            free(ptr);
        }
        
        gettimeofday(&end, NULL);
        total_time += ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    }
    
    printf("Task 1 average time: %ld microseconds\n", total_time / NUM_ITERATIONS);
}

void task2() {
    struct timeval start, end;
    long total_time = 0;
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        gettimeofday(&start, NULL);
        
        char* ptrs[NUM_ALLOCS];
        for (int j = 0; j < NUM_ALLOCS; j++) {
            ptrs[j] = (char*)malloc(1);
        }
        
        for (int j = 0; j < NUM_ALLOCS; j++) {
            free(ptrs[j]);
        }
        
        gettimeofday(&end, NULL);
        total_time += ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    }
    
    printf("Task 2 average time: %ld microseconds\n", total_time / NUM_ITERATIONS);
}

void task3() {
    struct timeval start, end;
    long total_time = 0;
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        gettimeofday(&start, NULL);
        
        char* ptrs[NUM_ALLOCS];
        int num_allocs = 0;
        for (int j = 0; j < NUM_ALLOCS; j++) {
            int choice = rand() % 2;
            if (choice == 0 && num_allocs < NUM_ALLOCS) {
                ptrs[num_allocs] = (char*)malloc(1);
                num_allocs++;
            } else if (num_allocs > 0) {
                int index = rand() % num_allocs;
                free(ptrs[index]);
                ptrs[index] = ptrs[num_allocs - 1];
                num_allocs--;
            }
        }
        
        for (int j = 0; j < num_allocs; j++) {
            free(ptrs[j]);
        }
        
        gettimeofday(&end, NULL);
        total_time += ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    }
    
    printf("Task 3 average time: %ld microseconds\n", total_time / NUM_ITERATIONS);
}

void stressTest1() {
    struct timeval start, end;
    long total_time = 0;
    const int CHUNK_SIZE = 504;
    const int NUM_CHUNKS = 8;
    char* ptrs[8];
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        gettimeofday(&start, NULL);        
        for (int j = 0; j < NUM_CHUNKS; j++) {
            ptrs[j] = (char*)malloc(CHUNK_SIZE);
        }
	for (int k = 0; k < NUM_CHUNKS; k++) {
	    free(ptrs[k]);
	}
        
        gettimeofday(&end, NULL);
        total_time += ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    }
    
    printf("Stress Test 1 average time: %ld microseconds\n", total_time / NUM_ITERATIONS);
}

void stressTest2() {
    struct timeval start, end;
    long total_time = 0;
    const int MAX_SIZE = 24;
    char* ptrs[128];
    int num_allocs = 0;
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        gettimeofday(&start, NULL);
        
        while (num_allocs < 128) {
            int size = rand() % MAX_SIZE + 1;
            ptrs[num_allocs] = (char*)malloc(size);
            if (ptrs[num_allocs] != NULL) {
                num_allocs++;
            } else {
                break;
            }
        }
        
        while (num_allocs > 0) {
            int index = rand() % num_allocs;
            free(ptrs[index]);
            ptrs[index] = ptrs[num_allocs - 1];
            num_allocs--;
        }
        
        gettimeofday(&end, NULL);
        total_time += ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    }
    
    printf("Stress Test 2 average time: %ld microseconds\n", total_time / NUM_ITERATIONS);
}

int main() {
    srand(time(NULL)); // Seed the random number generator
    
    task1();
    task2();
    task3();
    stressTest1();
    stressTest2();
    
    return 0;
}
