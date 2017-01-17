//
//  @file RouletteSelection.c
//  c
//
//  @author Benjamin
//  @date 2017-01-09.
//  @note Copyright © 2017 Benjamin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *  @function roulette_baskets
 *  @param probability An array containing the probabilities of drawing 
 *                      any specific object.
 *  @param baskets An array containing containg the maximum numbers that
 *                  can be used to select a given object.
 *  @param size The size of the %p probability and %p baskets arrays.
 *  @return void
 */
void roulette_baskets(float* probability,float* baskets, unsigned int size) {
    float sum = 0.0;
    unsigned int i;
    
    for(i = 0; i < size; i++) {
        sum += probability[i];
        baskets[i] = sum;
    }
}

/*
 *  @function match_selection
 *  @param baskets An array containing containg the maximum numbers that
 *                  can be used to select a given object.
 *  @param num An array of randomly generated numbers that are to be compared 
                    %p baskets to determine which objects have been selected.
 *  @param num_selections The number of objects to be selected.
 *  @param num_baskets The total number of objects that exist.
 *  @param selections An array of the selected objects.
 *  @return unsigned int* An array containing the selected object's index.
 */
unsigned int* match_selection(float* baskets, float* num, int num_selections,
                              unsigned int num_baskets, unsigned int* selections) {
    
    unsigned int index = 0;
    unsigned int* tmp_selections = selections;
    unsigned int i;
    
    for(i = 0 ; i< num_selections; i++) {
        if(num[i] > baskets[num_baskets-2]) {
            *tmp_selections = num_baskets - 1;
        }
        else if(num[i] < baskets[0]) {
            *tmp_selections = 0;
            
        }
        else {
            while(num[i] >= baskets[index]) {
                index++;
            }
            *tmp_selections = index - 1;
            index = 0;     //reset index
        }
        tmp_selections += sizeof(unsigned int);
    }
    
    return selections;
}

/*
 *  @functionmake_selection
 *  @param baskets An array containing containg the maximum numbers that
 *                  can be used to select a given object.
 *  @param num_selections The number of objects to be selected.
 *  @param num_baskets The total number of objects that exist.
 *  @param selections An array of the selected objects.
 *  @return unsigned int* An array containing the selected object's index.
 */
unsigned int* make_selection(float* baskets, int num_selections,
                             unsigned int num_baskets, unsigned int* selections){
    
    int i;
    float num[num_selections];
    
    for(i = 0; i < num_selections; i++) {
        num[i] = (float)rand() / (float)((unsigned)RAND_MAX + 1);
    }
    
    return match_selection(baskets, num, num_selections, num_baskets, selections);
}

/*
 *  @function roultette_selecion
 *  @param probabiltity An array containing the desired probaility of each object
 *      being selected.
 *  @param selections An array whose memory has already been alocated that will be
 *      used for storing the index of the selected objects
 *  @param The size of %p probability and %p selections
 */
unsigned int* roultette_selecion(float* probability, unsigned int* selections,
                                 unsigned int size_prob, unsigned int size_sel) {
    
    float baskets[size_sel];
    srand((unsigned int) time(NULL));   //initialise random seed
   
    roulette_baskets(probability, baskets, size_prob);
    return make_selection(baskets, size_sel, size_prob, selections);
}

