//Error when input is 1. Wrong answer in case of input 3, 4

int binarySearch(int* arr, int l, int r, int x)
{
   if (r >= l)
   {
        int mid = l + (r - l)/2;
        if (arr[mid] == x)  return mid;
        if (arr[mid] > x) return binarySearch(arr, l, mid-1, x);
        return binarySearch(arr, mid+1, r, x);
   }
    return -1;
}
  
int main()
{
    int result, x;
    int size = 5;
    int arr[5];
    // User input search
    x = scani();
    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 2;
    arr[3] = 3;
    arr[4] = 4;
    result = binarySearch(arr, 0, size-1, x);
    printi(result);
    if (result == -1)
        prints("Element is not present in array");
    else{
        prints("Element is present at index");
        printi(result);
    }
    return 0;
}