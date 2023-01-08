/* Leon Augusto Okida Gonçalves - GRR20190365 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "operation.h"
#include "tests.h"

#define MAX 100

void adds_index(operation_t operations[], int numOperations, int transactionIds[], int numTransactions) {
    for (int i=0; i<numTransactions; i++)
        for (int j=0; j<numOperations; j++) {
            if (operations[j].id == transactionIds[i])
                operations[j].matrixIndex = i;
        }
}

int main() {
    /* Read from input */
    int time, transaction;
    char operation, attribute;

    /* Counts schedules */ 
    int scheduleCounter = 1;

    /* Transactions ids */
    int transactionIds[MAX];
    int numTransactions = 0;
    int activeTransactions = 0;

    /* Arrays of time, transactions, operations and attributes */
    operation_t operations[MAX];
    int operationIndex = 0;

    while (scanf("%d %d %c %c", &time, &transaction, &operation, &attribute) != EOF) {
        /* If it's not a commit operation, add it to the operations array */
        if (operation != 'C') {
            /* If it's a new transaction, add it to the array of ids */
            if (!is_in(transactionIds, transaction, numTransactions)) {
                transactionIds[numTransactions] = transaction;
                numTransactions++;
                activeTransactions++;
            }
            /* Adds it to array of operations */
            operations[operationIndex].time = time;
            operations[operationIndex].id = transaction;
            operations[operationIndex].op = operation;
            operations[operationIndex].attr = attribute;
            operationIndex++;
        }
        else {
            activeTransactions--;
            /* If there are no active transactions, the schedule is ready to be processed */
            if (!activeTransactions) {
                /* Adds index to be used in adjacency matrix */
                adds_index(operations, operationIndex, transactionIds, numTransactions);

                /* Tests if it's serializable by conflict or equivalent by view */
                int conflict, view;
                conflict = test_conflict(operations, operationIndex, transactionIds, numTransactions);
                view = test_view(operations, operationIndex, transactionIds, numTransactions);

                /* Prints results */
                printf("%d ", scheduleCounter);

                for (int i = 0; i < numTransactions-1; i++) {
                    printf("%d,", transactionIds[i]);
                }
                printf("%d ", transactionIds[numTransactions - 1]);

                if (conflict)
                    printf("SS ");
                else
                    printf("NS ");

                if (view)
                    printf("SV\n");
                else
                    printf("NV\n");

                /* Resets array counters */
                numTransactions = 0;
                operationIndex = 0;

                scheduleCounter++;
            }
        }
    }

    return 0;
}