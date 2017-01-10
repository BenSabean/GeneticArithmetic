/*
 *  @file GeneticArithmetic.c
 *
 *  @date 2016-11-22
 *  @author Benjamin Sabean
 */
#include "RouletteSelection.h"

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

#define GOAL 42

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
 *  @return An array of evaluated bitstrings.
 *  Evaluates all artificial chromosones in @p chrom_array.
 */
 int* evaluate_chromosone(unsigned int* chrom_array) {  //under developement
    
    static int result_array[NUM_CHROMOSONES];
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
        total = 0;        //reset local vaiables
        operation = 0;
        chrom_init = 0;
    }
    return result_array;
}
/*
 *  @function assign_fitness
 *  @param results Array of evaluated chromosones
 *  @param goal The number we want a chromosone to evaluate to
 *  @return An array denoting the fitness level of each chromosone as denoted by 
 *      the equation: fitness[n] = (1/(goal-result[n]). The function will return
 *      2.0 if the goal has been achieved.
 */
float* assign_fitness(int* results, int goal) {
    
    static float fitness_scores[NUM_CHROMOSONES];
    unsigned int difference = 0;
    int i;
    
    for(i = 0; i< NUM_CHROMOSONES; i++) {
        difference = (float) goal - (float) results[i];
        if(difference > 0){
            fitness_scores[i] = 1.0 / difference;
        }
        else {
            fitness_scores[i] = 2.0;     //impossible value to indicate goal has been reached
        }
    }
    
    return fitness_scores;
}

/*
 *  @function find_probablility
 *  @param fitness_scores The fitness scores prduced by assign_fitness()
 *  @return The probability of selecting each chromosone based off their 
 *      @p fitness_scores
 */
float* find_probablility(float* fitness_scores) {
    
    static float probability[NUM_CHROMOSONES];
    float sum = 0.0;
    unsigned int i;
    
    for(i = 0; i< NUM_CHROMOSONES; i++) {
        sum += fitness_scores[i];
    }
    for(i = 0; i < NUM_CHROMOSONES; i++) {
        probability[i] = fitness_scores[i] / sum;
    }
    return probability;
}

int main(int argc, char** arg) {
    
    unsigned int chrom_array[NUM_CHROMOSONES];
    int* result_array;
    float* fitness_scores;
    float* probability;

    srand((unsigned int) time(NULL));   //initialise random seed

    unsigned int i;
    for (i = 0; i < NUM_CHROMOSONES; i++) {
        chrom_array[i] = generate_chromosone();
#if DEBUG
        printf("Random number[%d]: %x\n", i, chrom_array[i]);
#endif
    }
    
    result_array = evaluate_chromosone(chrom_array);
#if DEBUG
    for (i = 0; i < NUM_CHROMOSONES; i++) {
        printf("Random number[%d] result: %d\n", i, result_array[i]);
    }
#endif
    
    fitness_scores = assign_fitness(result_array, GOAL);
#if DEBUG
    for (i = 0; i < NUM_CHROMOSONES; i++) {
        printf("Random number[%d] fitness: %f\n", i, fitness_scores[i]);
        if(fitness_scores[i] == 2.0) {
            printf("goal has been achieved\n");
            return 0;
        }
    }
#endif
    
    probability = find_probablility(fitness_scores);
#if DEBUG
        for (i = 0; i < NUM_CHROMOSONES; i++) {
            printf("Random number[%d] probability: %f\n", i, probability[i]);
        }
#endif
    
    return 0;

}



