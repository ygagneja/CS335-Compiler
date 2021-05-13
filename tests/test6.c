// Program to test recursion using Ackerman

int A(int m, int n)
{
    if(m==0)
        return n+1;
    else if(n==0)
        return A(m-1,1);
    else
        return A(m-1,A(m,n-1));
}

int main()
{
    int res;
    prints("Enter two numbers :\n");
    res = A(scani(), scani());
    prints("Ackerman Output :\n");
    printi(res);
    prints("\n");
    return 0;
}

// (2, 2) : 7
// (1, 0) : 2
// (0, 5) : 6