// Function with params and return types pointer
// Using prints according to new convention
int* swap (int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
    return a;
}
int main() {
    int m = 25;
    int n = 100;
    prints("m is %d, n is %d\n", m, n);
    int* ret = swap(&m, &n);
    prints("m is %d, n is %d\n", m, n);
    prints("Returned values is :%d\n", *ret);
    return 0;
}
