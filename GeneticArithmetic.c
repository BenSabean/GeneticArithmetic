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
#define NIBBLE_SIZE 0xF            //Max size
#define NIBBLE_MASK 0xF            //Mask to grab the least significant nibble
#define NIBBLE  4                  //4 bits
#define DEBUG 1

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

int perform_op(int num1, int num2, int op) {
    
    if(op == ADD){
        return num2 + num1;
    }
    if(op == SUBTRACT){
        return num2 - num1;
    }
    if(op == MULTIPLY) {
        return num2 * num1;
    }
    if(op == DIVIDE) {    //possible divide by zero exception
        return num2 / num1;
    }
    return 0;             //something went wrong, return 0
}

void evaluate_chromosone(unsigned int* chrom_array) {  //under developement
    unsigned int result_array[NUM_CHROMOSONES];
    uint8_t num1 = 0;          //Variables to hold number literals
    uint8_t num2 = 0;          //     ^^
    uint8_t operation = 0;     //variable to hold decimenal representation of operation
    uint32_t cur_chromosome;
    int i;
    int cur_nibble;

    for (i = 0; i < NUM_CHROMOSONES; i++) {
#if DEBUG
        printf("Random number[%d](in function): %x\n", i, chrom_array[i]);
#endif
        cur_chromosome = chrom_array[i];
        
        while (cur_chromosome > 0) {
            cur_nibble = chrom_array[i] & NIBBLE_MASK;
           
            if(cur_nibble < 10){        //check if value is a binary literal
                if(operation == 0) {    //check if an operation has been defined yet
                    num1 = cur_nibble;
                }
                else {
                    num2 = cur_nibble;
                    num1 = perform_op(num1, num2, operation);  //num1 is the cumulative total
                }
            }
            if(cur_nibble == ADD){
                operation = ADD;
            }
            if(cur_nibble == SUBTRACT){
                operation = SUBTRACT;
            }
            if(cur_nibble == MULTIPLY){
                operation = MULTIPLY;
            }
            if(cur_nibble == DIVIDE){
                operation = DIVIDE;
            }
            cur_chromosome >>= NIBBLE;
           
        }
        result_array[i] = num1;
#if DEBUG
        printf("Random number[%d]result: %d\n", i, result_array[i]);
#endif
        num1 = 0;
    }
}

int main(int argc, char** arg) {
    unsigned int chrom_array[NUM_CHROMOSONES];
    srand((unsigned int) time(NULL));   //initialise random seed

    int i;
    for (i = 0; i < NUM_CHROMOSONES; i++) {
        chrom_array[i] = generate_chromosone();
#if DEBUG
        printf("Random number[%d]: %x\n", i, chrom_array[i]);
#endif
    }
    evaluate_chromosone(chrom_array);
    return 0;

}


