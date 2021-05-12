// Bubble sort to test array and nested loops

int main()
{
    int number, i, j, temp;
    int a[10];
    
    number = 10;
    
    prints("Enter an array of size 10:  ");
    for(i = 0; i < number; i++)
        a[i] = scani();

    for(i = 0; i < number - 1; i++)
    {
        for(j = 0; j < number - i - 1; j++)
        {
            if(a[j] > a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
    prints("List Sorted in Ascending Order : ");
    for(i = 0; i < number; i++)
    {
        printi(a[i]);
    }
    return 0;
}