/* Leon Augusto Okida Gonçalves - GRR20190365 */
#include "util.h"
#include "operation.h"

int is_in(int array[], int n, int size) {
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] == n) {
            found = 1;
            break;
        }
    }
    return found;
}

int is_in_write_array(char array[], char oper, int size) {
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (array[i] == oper) {
            found = 1;
            break;
        }
    }
    return found;
}