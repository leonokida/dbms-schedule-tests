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