/* Leon Augusto Okida Gonçalves - GRR20190365 */
#ifndef ___UTIL___
#define ___UTIL___

/*
Returns 1 if n is in array, 0 if it's not
*/
int is_in(int array[], int n, int size);

/*
Returns 1 if is in the write operations array, 0 if it's not
*/
int is_in_write_array(char array[], char oper, int size);

#endif