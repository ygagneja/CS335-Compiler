// Binary Search to test arrays and recursion
void printf(char* str, ...);
void scanf(char* str, ...);

int binarySearch(int arr[], int l, int r, int x)
{
   if (r >= l)
   {
        int mid = l + (r - l)/2;
  
        // If the element is present at the middle itself
        if (arr[mid] == x)  return mid;
  
        // If element is smaller than mid, then it can only be present
        // in left subarray
        if (arr[mid] > x) return binarySearch(arr, l, mid-1, x);
  
        // Else the element can only be present in right subarray
        return binarySearch(arr, mid+1, r, x);
   }
  
   // Element is not present in array
   return -1;
}
  
int main(void)
{

    // Both implementations of fixed array and user input array

    // User input array

    // int size,i;
    // printf("Enter the number of elements: ");
    // scanf("%d", &size); 
    // int arr[size];
    // printf("Enter the elements of the array to be sorted:\n");
    // for (i = 0; i < size; i++)
    // {
    //     scanf("%d", &arr[i]);
    // } 

    // Fixed array

    int arr[] = {2, 3, 4, 10, 40};
    int size = 5;

    // User input search
    int x;
    scanf("%d", &x);
    int result = binarySearch(arr, 0, size-1, x);
    if (result == -1)
        printf("Element is not present in array");
    else
        printf("Element is present at index %d", result);
    return 0;
}