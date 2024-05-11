#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main() {
    // Allocate memory blocks
    int *intPtr = (int *)malloc(sizeof(int) * 10);
    double *doublePtr = (double *)malloc(sizeof(double) * 5);
    char *charPtr = (char *)malloc(sizeof(char) * 20);


    // Use allocated memory
    printf("Allocated memory for integers:\n");
    for (int i = 0; i < 10; ++i) {
        intPtr[i] = i * 2;
        printf("%d ", intPtr[i]);
    }
    printf("\n");

    printf("Allocated memory for doubles:\n");
    for (int i = 0; i < 5; ++i) {
        doublePtr[i] = i * 1.5;
        printf("%.2f ", doublePtr[i]);
    }
    printf("\n");

    printf("Allocated memory for characters:\n");
    for (int i = 0; i < 20; ++i) {
        charPtr[i] = 'A' + i;
        printf("%c ", charPtr[i]);
    }
    printf("\n");

    // Free memory
    free(intPtr);
    free(doublePtr);
    free(charPtr);

    // Try to allocate too much memory
    printf("\nAttempting to allocate too much memory:\n");
    void *ptr = malloc(5000);
    if (ptr == NULL) {
        printf("Memory allocation failed (as expected).\n");
    }

    // Try to free a NULL pointer
    printf("\nAttempting to free a NULL pointer:\n");
    free(NULL);

    // Try to free an invalid pointer
    printf("\nAttempting to free an invalid pointer:\n");
    int x;
    free(&x);

    return 0;
}
