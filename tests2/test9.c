// Code to test mutual recursion
int hofstaderMale(int n);
// Female function
int hofstaderFemale(int n)
{
    if (n < 0)
        return 0;
    else
        if (n == 0)
            return 1;
        else
            return (n - hofstaderMale(n - 1));
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
            return (n-hofstaderFemale(n - 1));
}
  
// Driver Code
int main()
{
    int i;
    prints("F : ");
    for (i = 0; i < 5; i++) 
        printi(hofstaderFemale(i));

    prints("\nM : ");
    for (i = 0; i < 5; i++) 
        printi(hofstaderMale(i));
    //Expected output
    //F : 1 1 2 2 3 
    //M : 0 0 1 1 2
    return 0;
}