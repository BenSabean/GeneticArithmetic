/*
 *  @file GeneticArithmetic.c
 *
 *  @date 2016-11-22
 *  @author Benjamin Sabean
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/** Op-codes **/
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

/*
 *  @function generate_chromosone
 *  @param none
 *  @return The generated virtual chromosone as a bitstring.
 *  Generates a 28-bit integer to be used as an artificial chromosone.
 */
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

/*
 *  @function perform_op
 *  @param num1 The cumulaive total to be manipulated.
 *  @param num2 The second number to be manipulated.
 *  @param op A pointer to the bit code of the operation to be used on the two numbers.
 *  @return the new cumulative total
 *  Performs the operation designated by @p op on @p num1 and @p num2
 */
int perform_op(int total, int num, int8_t* op) {
    
    int result = 0;
    
    if(*op == ADD){
        result = total + num;
    }
    if(*op == SUBTRACT){
        result = total - num;
    }
    if(*op == MULTIPLY) {
        result = total * num;
    }
    if(*op == DIVIDE) {
        if(num == 0 ) {  //Handles divide by zero exception
            *op = -1;
            return 0;
        }
        result = total / num;
    }
    *op = -1;
    return result;
}

/*
 *  @function evaluate_chromosone
 *  @param chrom_array Array of artificial chromosones.
 *  @return void
 *  Evaluates all artificial chromosones in @p chrom_array.
 */
void evaluate_chromosone(unsigned int* chrom_array) {  //under developement
    
    unsigned int result_array[NUM_CHROMOSONES];
    int8_t total = 0;         //Cumulative total
    int8_t num = 0;           //Variable to hold number literal
    int8_t operation = 0;     //variable to hold integer representation of operation
    uint8_t chrom_init = 0;   //flag to determine if the chromosone has been initialized
    uint32_t cur_chromosome;
    int cur_nibble = 0;

    unsigned int i;
    for (i = 0; i < NUM_CHROMOSONES; i++) {
        cur_chromosome = chrom_array[i];
        
        while (cur_chromosome > 0) {
            cur_nibble = cur_chromosome & NIBBLE_MASK;
           
            if(cur_nibble < 10){        //check if value is a binary literal
                if(operation == 0) {    //check if an operation has been defined yet
                    total = cur_nibble;
                    chrom_init = 1;     //set true
                }
                else if(operation >= 0xA){
                    num = cur_nibble;
                    total = perform_op(total, num, &operation);
                }
            }
            if(cur_nibble == ADD && chrom_init){
                operation = ADD;
            }
            if(cur_nibble == SUBTRACT && chrom_init){
                operation = SUBTRACT;
            }
            if(cur_nibble == MULTIPLY && chrom_init){
                operation = MULTIPLY;
            }
            if(cur_nibble == DIVIDE && chrom_init){
                operation = DIVIDE;
            }
            cur_chromosome >>= NIBBLE;
           
        }
        result_array[i] = total;
#if DEBUG
        printf("Random number[%d]result: %d\n", i, result_array[i]);
#endif
        total = 0;        //reset local vaiables
        operation = 0;
        chrom_init = 0;
    }
}

int main(int argc, char** arg) {
    
    unsigned int chrom_array[NUM_CHROMOSONES];
    srand((unsigned int) time(NULL));   //initialise random seed

    unsigned int i;
    for (i = 0; i < NUM_CHROMOSONES; i++) {
        chrom_array[i] = generate_chromosone();
#if DEBUG
        printf("Random number[%d]: %x\n", i, chrom_array[i]);
#endif
    }
    evaluate_chromosone(chrom_array);
    return 0;

}


