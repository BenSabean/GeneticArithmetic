/*
 * GeneticArithmetic.c
 *
 *  Created on: 2016-11-22
 *      Author: Benjamin Sabean
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define ADD 0xA
#define SUBTRACT 0xB
#define MULTIPLY 0xC
#define DIVIDE 0xD

#define NUM_CHROMOSONES 50
#define CHROMO_SIZE 0xFFFFFFF      //28-bit integer
#define NIBBLE_SIZE 0xF
#define NIBBLE_MASK 0xF
#define NIBBLE  4
#define DEBUG 0

unsigned int generate_chromosone() {

    unsigned int bitstring = 0;
    unsigned int size = CHROMO_SIZE;

    while (size > 0) {
        size >>= NIBBLE;
        bitstring += (unsigned int) rand() % NIBBLE_SIZE;
        if (size > 0) {
            bitstring <<= NIBBLE;
        }
    }
    return bitstring;

}


void evaluate_chromosone(unsigned int* chrom_array) {  //broken
    int result = 0;
    unsigned int result_array[NUM_CHROMOSONES];
    uint8_t num1 = 0;          //Variables to hold number literals
    uint8_t num2 = 0;
    uint32_t cur_chromosome;
    int i;
    int cur_nibble;

    for (i = 0; i < NUM_CHROMOSONES; i++) {
        printf("Random number[%d](in function): %x\n", i, chrom_array[i]);
        cur_chromosome = chrom_array[i];
        
        while (cur_chromosome > 0) {
            cur_nibble = chrom_array[i] & NIBBLE_MASK;
            if(cur_nibble < 10){
                num1 = cur_nibble;
            }
            if(cur_nibble == ADD){

            }
            if(cur_nibble == SUBTRACT){

            }
            if(cur_nibble == MULTIPLY){

            }
            if(cur_nibble == DIVIDE){

            }
            cur_chromosome >>= NIBBLE;
           
        }
    }
}

int main(int argc, char** arg) {
    unsigned int chrom_array[NUM_CHROMOSONES];
    /* Initialise random seed: */
    srand((unsigned int) time(NULL));

    int i;
    for (i = 0; i < NUM_CHROMOSONES; i++) {
        chrom_array[i] = generate_chromosone();
        printf("Random number[%d]: %x\n", i, chrom_array[i]);
    }
    evaluate_chromosone(chrom_array);
    return 0;

}


