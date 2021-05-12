// Code to test mutual recursion
  
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
            return (n-hofstaderMale(n - 1));
}
  
// Driver Code
int main()
{
    int i;
    prints("F");
    for (i = 0; i < 20; i++) 
        printi(hofstaderFemale(i));

    prints("M");
    for (i = 0; i < 20; i++) 
        printi(hofstaderMale(i));
  
    return 0;
}