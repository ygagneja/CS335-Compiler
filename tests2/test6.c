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
    int m,n;
    prints("Enter two numbers:\n");
    m = scani();// 2 2 expected output : 7
    n = scani();
    prints("\nAckerman Output :: ");
    printi(A(m, n));
    return 0;
}

