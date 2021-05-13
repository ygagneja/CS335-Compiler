// Quicksort to test arrays and recursion

void swap(int* x, int* y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

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
                swap(list+i, list+j);
            }
        }
        swap(&list[j], &list[pivot]);
        quicksort(list, low, j - 1);
        quicksort(list, j + 1, high);
    }
}

int main()
{
    int list[50];
    int size, i;
    prints("Enter the number of elements in the array (must be <= 50) :\n");
    size = scani();
    prints("Enter the elements of the array to be sorted :\n");
    for (i = 0; i < size; i++)
    {
        list[i] = scani();
    } 
    quicksort(list, 0, size-1);
    prints("Array after applying quick sort :\n");
    for (i = 0; i < size; i++)
    {
        printi(list[i]);
    }
    prints("\n");
    return 0;
}
