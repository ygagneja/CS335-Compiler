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
    int arr[5];
    int size = 5, i, result;

    // User input search
    int x;
    prints("Enter array of size 5 (array should be sorted):\n");
    for(i = 0; i < size; i++){
        arr[i] = scani();
    }
    prints("Enter element to search in the array: ");
    x = scani();
    result = binarySearch(arr, 0, size-1, x);
    if (result == -1)
        prints("Element is not present in the array");
    else
    {
        prints("Element is present at index : ");
        printi(result);
    }
    return 0;
}