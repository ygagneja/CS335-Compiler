// Code to test mutual recursion
void printf(char* str, ...);
void scanf(char* str, ...);

int hofstaderFemale(int);
int hofstaderMale(int);
  
// Female function
int hofstaderFemale(int n)
{
    if (n < 0)
        return 0;
    else
        if (n == 0)
            return 1;
        else
            return (n - hofstaderFemale(n - 1));
}
  
// Male function
int hofstaderMale(int n)
{
    if (n < 0)
        return 0;
    else
        if (n == 0)
            return 0;
        else
            return (n - hofstaderMale(n - 1));
}
  
// Driver Code
int main()
{
    int i;
    printf("F\n");
    for (i = 0; i < 20; i++) 
        printf("%d ",hofstaderFemale(i));
      
    printf("\n");
  
    printf("M\n");
    for (i = 0; i < 20; i++) 
        printf("%d ",hofstaderMale(i));
  
    return 0;
}