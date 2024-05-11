NetID: nv274
Name: Nandish Verma
Partners: none
Date: 2/21/2024

For Stress Test 1, I allocate 8 chunks of 504 bytes, and after all 8 are allocated, they are deallocated. This repeats for 50 times.

For Stress Test 2, I allocate chunks of random size of 8 up to 256 until 128 chunks have been allocated, or until malloc fails to allocate memory of that size. If successful,
it incremements num_allocs, otherwise it breaks out of the loop. It then enters another loop where it selects a random index within the range of
num_allocs, calls free() to deallocate the block at that index, moves the pointer at that index to the end of the array, and decrements num_allocs.
It repeats this 50 times.

In memtestcase.c, I call malloc for data of types integer, double and char. I fill the allocated areas with data of these types, and check if all 
the correct bytes are there. I try to allocate too much memory, free NULL pointers, and free invalid addresses.

In the free() function I wrote, rather than directly looking up the address, it starts from the header and looks for the given address-8 to ensure that free 
won't affect actual data. This does affect performance.
