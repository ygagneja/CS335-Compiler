// Code to test mutual recursion

bool even(int n);

bool odd(int n){
    if (n==0) return false;
    return even(n-1);
}

bool even(int n){
    return n==0 ? true : odd(n-1);
}

// Driver Code
int main()
{
    prints("Give a positive number to check if its odd :\n");
    printi(odd(scani()));
    prints("\n");
    return 0;
}