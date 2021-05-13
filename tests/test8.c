// Binary Search to test arrays and recursion

int binarySearch(int* arr, int l, int r, int x)
{
    int mid;
    if (r >= l)
    {
        mid = l + (r - l)/2;
        if (arr[mid] == x)  return mid;
        if (arr[mid] > x) return binarySearch(arr, l, mid-1, x);
        return binarySearch(arr, mid+1, r, x);
    }
   return -1;
}
  
int main()
{
    int arr[50];
    int size, i, result;

    // User input search
    int x;
    prints("Enter the number of elements in the array (must be <= 50) :\n");
    size = scani();
    prints("Enter the elements of the array (must be sorted) :\n");
    for(i = 0; i < size; i++){
        arr[i] = scani();
    }
    prints("Enter element to search in the array :\n");
    x = scani();
    result = binarySearch(arr, 0, size-1, x);
    if (result == -1)
        prints("Element is not present in the array\n");
    else
    {
        prints("Element is present at index : ");
        printi(result);
        prints("\n");
    }
    return 0;
}