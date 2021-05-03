// Bubble sort to test array and nested loops
void printf(char* str, ...);
void scanf(char* str, ...);


int main()
{
    int number, i, j, temp;
    
    printf("\n Enter the total Number of Elements  :  ");
    scanf("%d", &number);

    int a[number];
    
    printf("\n Enter the Array Elements  :  ");
    for(i = 0; i < number; i++)
        scanf("%d", &a[i]);

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
    printf("\n List Sorted in Ascending Order : ");
    for(i = 0; i < number; i++)
    {
        printf(" %d \t", a[i]);
    }
    printf("\n");
    return 0;
}