/* Leon Augusto Okida Gonçalves - GRR20190365 */
#include "operation.h"

/*
Checks if the schedule is serializable by conflict
*/
int test_conflict(operation_t operations[], int operationIndex, int transactionIds[], int numTransactions);

/*
Checks if the schedule is equivalent by view
*/
int test_view(operation_t operations[], int operationIndex, int transactionIds[], int numTransactions);