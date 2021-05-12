// Binary Search to test arrays and recursion
int binarySearch(int* arr, int l, int r, int x)
{
    int mid;
    if (r >= l)
    {
        mid = l + (r - l)/2;

        // If the element is present at the middle itself
        if (arr[mid] == x)  return mid;

        // If element is smaller than mid, then it can only be present
        // in left subarray
        if (arr[mid] > x) return binarySearch(arr, l, mid-1, x);

        // Else the element can only be present in right subarray
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
    prints("Enter array of size 5:");
    for(i = 0; i < size; i++){
        arr[i] = scani();
    }
    prints("Enter element to search in the array:");
    x = scani();
    result = binarySearch(arr, 0, size-1, x);
    if (result == -1)
        prints("Element is not present in array");
    else
    {
        prints("Element is present at index : ");
        printi(result);
    }
    return 0;
}