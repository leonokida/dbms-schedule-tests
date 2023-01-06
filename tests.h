#include "operation.h"

/*
Checks if the schedule is serializable by conflict
*/
int test_conflict(operation_t operations[], int operationIndex, int transactionIds[], int numTransactions);

/*
Checks if the schedule is equivalent by vision
*/
int test_vision(operation_t operations[], int operationIndex, int transactionIds[], int numTransactions);