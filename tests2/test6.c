// Program to test recursion using Ackerman
void printf(char* str, ...);
void scanf(char* str, ...);

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
    printf("Enter two numbers :: \n");
    scanf("%d %d",&m,&n);
    printf("\nAckerman Output :: %d\n",A(m,n));
    return 0;
}

