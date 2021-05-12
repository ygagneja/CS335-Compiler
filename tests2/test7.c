// Program to test arrays and recursion

void quicksort(int* list, int low, int high)
{
    int pivot, i, j, temp;
    if (low < high)
    {
        pivot = low;
        i = low;
        j = high;
        while (i < j) 
        {
            while (list[i] <= list[pivot] && i <= high)
            {
                i = i+1;
            }
            while (list[j] > list[pivot] && j >= low)
            {
                j = j-1;
            }
            if (i < j)
            {
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
        temp = list[j];
        list[j] = list[pivot];
        list[pivot] = temp;
        quicksort(list, low, j - 1);
        quicksort(list, j + 1, high);
    }
}

int main()
{
    int size = 10, i;
 
    int list[10];
    prints("Enter 10 elements of the array to be sorted:\n");
    for (i = 0; i < size; i++)
    {
        list[i] = scani();
    } 
    quicksort(list, 0, size - 1);
    prints("After applying quick sort\n");
    for (i = 0; i < size; i++)
    {
        printi(list[i]);
    } 
    return 0;
}
