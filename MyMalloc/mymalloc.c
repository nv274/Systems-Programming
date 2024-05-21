#include <stdio.h>
#include <stdlib.h>
#define MEMLENGTH 512
#include "mymalloc.h"
static double memory[MEMLENGTH];

struct block {
    int free;
    int size;
//Address of next block is determined by size of current block + metadata
};

void initialize(struct block *target) {
    struct block p = {1, 4096 - sizeof(struct block)};
    *target = p;
//Creates the first block
}

int align(int numBytes) {
    int alignedBytes = ((numBytes + 7) / 8) * 8; // Align to 8 bytes
    return alignedBytes;
//8 byte wide alignment
}

void split(struct block *target, int reqBytes) {
    struct block *nextBlock = (struct block*)((char*)target + reqBytes+sizeof(struct block));
    nextBlock->free = 1;
    nextBlock->size = target->size - reqBytes - sizeof(struct block);
    target->size = reqBytes;
//When allocating a new block, will split a free block if it has enough space for the requested memory.
}

void* mymalloc(size_t numBytes, char* file, int line) {
    int alignBytes = align(numBytes);
    if (alignBytes > 4096 - sizeof(struct block)) {
        printf("File %s, Line %d: Too much memory requested.\n",file, line);
        return NULL;
    }
    struct block* curr = (struct block*)memory;
    if (!curr->size) {
	printf("Initialization confirmed.\n");
        initialize(curr);
        printf("Minimum chunk size: %ld bytes\n",(sizeof(struct block)+8));
    }
//moves to address of possible next block
    struct block *prev = curr;
    curr = (struct block*) ((char*)curr + curr->size + sizeof(struct block));
    void* payload = NULL;
    while ((char*)prev < (char*)memory+4088) {
        if (prev->free) {
            if (prev->size >= alignBytes) {
                // Split block if necessary
                if (prev->size > alignBytes + sizeof(struct block) ){
                    split(prev, alignBytes);
                }
                prev->free = 0;
                payload = (void*)((char*)prev + sizeof(struct block));
                return payload;
     	    }
	    else {
		prev->size += (curr->size+sizeof(struct block)) * curr->free;
		curr = (struct block*)((char*) prev);
	   }
	}
	prev = curr;
	curr = (struct block *)((char*)curr + curr->size+sizeof(struct block));
    }
    printf("File %s, Line %d: Sorry. There is no more space to allocate.\n", file, line);
    return NULL;
}

void myfree(void* address, char* file, int line) {
    if(address == NULL){
	printf("File %s, Line %d: NULL pointer provided for freeing.\n", file, line);
	return;
    }

    if ((char*)address < (char*)memory + sizeof(struct block) || (char*)address > (char*)memory + 4088) {
        printf("File %s, Line %d: %p is not a valid address because it is out of bounds.\n", file, line, address);
        return;
    }
    if((int)(char*)address%8 != 0){
	printf("File %s, Line %d: Invalid Address provided.", file, line);
	return;
    }
    struct block *target = (struct block*)memory;
	//Iterates through blocks to see if the address requested to be freed has been returned by mymalloc.
    while ((void*)target <= address) {
        if ((void*)target + sizeof(struct block) == address) {
            if (!target->free) {
                target->free = 1;
                return;
            } else {
                printf("File %s: Line %d: %p is already free.\n", file, line, address);
                return;
            }
        }
        target = (struct block*) ((char*)target+target->size+sizeof(struct block));
    }
    printf("File %s, Line %d: %p is not a valid allocated address.\n", file, line, address);
}
