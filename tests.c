/* Leon Augusto Okida Gonçalves - GRR20190365 */
#include "operation.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>

#include "util.h"

typedef struct trio {
    int i;
    int j;
    int k;
} trio;

/*
Creates adjacency matrix (inline matrix)
*/
int *createMatrix(int size) {
    return calloc(size*size, sizeof(int));
}

/*
Detects cycle in the graph represented by the adjacency matrix
*/
int has_cycle(int *matrix, int size, int nodesExplored[], int indexNodes, int i) {
    /* Adds node i to list of explored nodes */
    int initialIndexNodes = indexNodes;
    nodesExplored[indexNodes] = i;
    indexNodes++;

    /* Iterates through arcs of the current node */
    for (int j = 0; j < size; j++)
        /* If there's an arc */
        if (matrix[i*size + j] >= 1) {
            /* Finds if j is already in the explored nodes, or if there's a cycle exploring from j */
            if (
                (is_in(nodesExplored, j, indexNodes)) ||
                (has_cycle(matrix, size, nodesExplored, indexNodes, j))
            ) {
                return 1;
            }
        }
    
    indexNodes = initialIndexNodes;
    return 0;
}

int test_conflict(operation_t operations[], int operationIndex, int transactionIds[], int numTransactions) {
    int *adjMatrix = createMatrix(numTransactions);

    /* Iterate through the operations (i), from last to first */
    for (int i = operationIndex - 1; i >= 1; i--) {
        /* Iterates through the operations (j) that happened before i */
        for (int j = i - 1; j >= 0; j--) {
            /* If the operations i and j are conflictive */
            if (
                (operations[i].id != operations[j].id) &&
                (operations[i].attr == operations[j].attr) &&
                ((operations[i].op == 'W') || (operations[j].op == 'W'))
            ) {
                /* Creates arc from node i to node j */
                int adjMatrixIndex = operations[i].matrixIndex * numTransactions + operations[j].matrixIndex;
                adjMatrix[adjMatrixIndex] = 1;
            }
        }
    }

    /* Detects if the adjacency matrix has any cycles: if it has, it's not serializable; if it has not, it's serialiazable */
    int nodesExplored[100];
    int indexNodes = 0;
    int cycleFree = !has_cycle(adjMatrix, numTransactions, nodesExplored, indexNodes, 0);

    free(adjMatrix);
    return cycleFree;
}

int *copy_matrix(int *adjMatrix, int finalIndex) {
    int *copy = createMatrix(finalIndex);
    for (int i = 0; i < finalIndex; i++)
        for (int j = 0; j < finalIndex; j++)
            copy[i*finalIndex + j] = adjMatrix[i*finalIndex + j];
    return copy;
}

/*
Creates all possible graphs by selecting only one of the pair of arcs created by each occurences of Tk (see test_vision function)
*/
int runs_test(int *adjMatrix, int numTransactions, trio triosArray[], int numTrios, int i) {
    if (i < numTrios) {
        /* Creates two copies */
        int *cpy1, *cpy2;
        
        /* Tests removing arc from Tk to Ti */
        cpy1 = copy_matrix(adjMatrix, numTransactions);
        int matrixIndex = triosArray[i].k * numTransactions + triosArray[i].i;
        if (cpy1[matrixIndex] > 0)
            cpy1[matrixIndex] -= 1;
        if (runs_test(cpy1, numTransactions, triosArray, numTrios, i+1)) {
            free(cpy1);
            return 1;
        }
        free(cpy1);
        /* Tests removing arc from Tj to Tk */
        cpy2 = copy_matrix(adjMatrix, numTransactions);
        matrixIndex = triosArray[i].j * numTransactions + triosArray[i].k;
        if (cpy2[matrixIndex] > 0)
            cpy2[matrixIndex] -= 1;
        if (runs_test(cpy2, numTransactions, triosArray, numTrios, i+1)) {
            free(cpy2);
            return 1;
        }
        free(cpy2);

        /* Backtracks */
        return 0;
    }

    int nodesExplored[100];
    int indexNodes = 0;
    return !has_cycle(adjMatrix, numTransactions, nodesExplored, indexNodes, 0);
}

int test_vision(operation_t operations[], int operationIndex, int transactionIds[], int numTransactions) {
    /* Matrix with two more nodes (start and final transactions) */
    int finalIndex = numTransactions + 1;
    numTransactions+=2;
    int *adjMatrix = createMatrix(numTransactions);

    /* Array that registers last write operation for each attribute */
    char lastWrites[100];
    int numLastWrites = 0;

    /* Registers all situations where there's a Tk transaction (see loop below) */
    trio triosArray[100];
    int numTrios = 0;

    /* Iterates through the operations (j), from last to first */
    for (int j = operationIndex - 1; j >= 0; j--) {
        if (operations[j].op == 'W') {
            /* Checks if it's the last write operation */
            if (!is_in_write_array(lastWrites, operations[j].attr, numLastWrites)) {
                /* Tf reads what operation j wrote */
                int adjMatrixIndex = (operations[j].matrixIndex + 1) * numTransactions + finalIndex;
                adjMatrix[adjMatrixIndex] = 1;

                /* There's a final write operation registered for the attribute */
                lastWrites[numLastWrites] = operations[j].attr;
                numLastWrites++;

                /* Finds whether a Tk operation that also writes the same attribute exists */
                /* If it does, creates arc from Tk to Tj */
                for (int k = 0; k < numTransactions; k++) {
                    if (
                        (operations[k].op == 'W') &&
                        (operations[k].attr == operations[j].attr) &&
                        (operations[k].id != operations[j].id)
                    ) {
                        adjMatrixIndex = (operations[k].matrixIndex + 1) * numTransactions + operations[j].matrixIndex+1;
                        adjMatrix[adjMatrixIndex] = 1;

                        triosArray[numTrios].i = operations[j].matrixIndex+1;
                        triosArray[numTrios].j = finalIndex;
                        triosArray[numTrios].k = operations[k].matrixIndex+1;
                        numTrios++;
                    }
                }
            }
        }
        else {
            /* Find operation i that wrote the data read by operation j */

            /* If there's an operation i that wrote the data or if it's written by T0 */
            int writtenByTransaction = 0;
            int transI, idI, transJ = operations[j].matrixIndex + 1;

            for (int i = j - 1; i >= 0; i--) {
                if (
                    (operations[i].op == 'W') &&
                    (operations[i].attr == operations[j].attr) &&
                    (operations[i].id != operations[j].id)
                ) {
                    idI = operations[i].id;
                    transI = operations[i].matrixIndex + 1;
                    writtenByTransaction = 1;
                    break;
                }
            }
            /* T0 wrote the data read by operation j */
            if (!writtenByTransaction)
                transI = 0;
            
            /* Creates arc from Ti to Tj */ 
            int adjMatrixIndex = transI * numTransactions + transJ;
            adjMatrix[adjMatrixIndex] = 1;

            /* Finds whether a Tk operation that also writes the same attribute exists */
            /* If it does, creates arcs from Tk to Ti and from Tj to Tk */
            for (int k = 0; k < numTransactions; k++) {
                if (
                    (operations[k].op == 'W') &&
                    (operations[k].attr == operations[j].attr) &&
                    (operations[k].id != operations[j].id) &&
                    (operations[k].id != idI)
                ) {
                    if (transI != 0) {
                        int adjMatrixIndex1 = (operations[k].matrixIndex + 1) * numTransactions + transI;
                        adjMatrix[adjMatrixIndex1] = 1;
                    }
                    int adjMatrixIndex2 = transJ * numTransactions + (operations[k].matrixIndex + 1);
                    adjMatrix[adjMatrixIndex2] = 1;

                    triosArray[numTrios].i = transI;
                    triosArray[numTrios].j = transJ;
                    triosArray[numTrios].k = operations[k].matrixIndex+1;
                    numTrios++;
                }
            }
        }
    }

    /* Runs tests for all possible graphs */
    /* If it's cycle free in all of them, it's equivalent by view */
    int cycleFree = runs_test(adjMatrix, numTransactions, triosArray, numTrios, 0);

    free(adjMatrix);
    return cycleFree;
}