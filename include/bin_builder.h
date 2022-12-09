#pragma once
#include "./array_list.h"


/**
 * Sanatize coordinates that are in form of "lattitude,longitude"
 * @param lattitude
 * @param longitude
 * @param coordinates
 */
void sanatize_coordinates(double* lattitude, double* longitude, char* coordinates);

/**
 * Write serialized data to fp.
 * Return 1 if done it successfuly, otherwise 0.
 * fp is not closed into this function.
 * @param str
 * @param fp
 * @param n
 * @return 1 if done it successfuly, otherwise 0
 * @note fp is not closed into this function.
 */
int write_to_bin(char** str, FILE* fp, int n);

/**
 * Read the content of fp, serialized and write it into path_bin. 
 * Return 1 if done it succesfuly, otherwise 0.
 * fp is not closed into this function. 
 * @param fp
 * @param path_bin
 * @return 1 if done it succesfuly, otherwise 0
 * @note fp is not closed into this function.
 */
int build_bin(FILE* fp, char* path_bin);

/**
 * Read and deserialize data from fp 
 * and return a list of theses data. 
 * @param fp
 * @return list of data_t
 * @note fp is not closed into this function.
 */
list_t* get_data_bin(FILE* fp);