// Bubble sort to test array and nested loops

int main()
{
    int size, i, j, temp;
    int list[50];
    
    prints("Enter the number of elements in the array (must be <= 50) :\n");
    size = scani();
    prints("Enter the elements of the array to be sorted :\n");
    for (i = 0; i < size; i++)
    {
        list[i] = scani();
    } 

    for(i = 0; i < size - 1; i++)
    {
        for(j = 0; j < size - i - 1; j++)
        {
            if(list[j] > list[j + 1])
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
    prints("List Sorted in Ascending Order :\n");
    for(i = 0; i < size; i++)
    {
        printi(list[i]);
    }
    prints("\n");
    return 0;
}