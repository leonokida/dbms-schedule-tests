/* Leon Augusto Okida Gonçalves - GRR20190365 */
#include "operation.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>

#include "util.h"

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
        if (matrix[i*size + j] == 1) {
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

int test_vision(operation_t operations[], int operationIndex, int transactionIds[], int numTransactions) {
    /* Matrix with two more nodes (start and final transactions) */
    int finalIndex = operationIndex;
    int *adjMatrix = createMatrix(numTransactions + 2);


    free(adjMatrix);
    return 0;
}