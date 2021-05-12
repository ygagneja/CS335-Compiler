// passing ptrs as function arguments and return values
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
    int *ret;
    prints("Values of m and n before swapping : ");
    printi(m);
    printi(n);
    ret = swap(&m, &n);
    prints("Values of m and n after swapping : ");
    printi(m);
    printi(n);
    prints("Returned values is :");
    printi(*ret);
    return 0;
}
