// Program to test multi dimensional arrays using matrix multiplicatuon

int main()
{
  int rowCountOne, columnCountOne, rowCountTwo, columnCountTwo, i, j, k, sum = 0;
  int max = 5; // Adjust it to allow larger matrices
  int firstMatrix[max][max], secondMatrix[max][max], resultMatrix[max][max];

  prints("Number of rows in first matrix : ");
  scanf("%d", &rowCountOne);

  prints("Number of columns in first matrix : ");
  scanf("%d", &columnCountOne);

  prints("Elements of first matrix : \n");

  for (i = 0; i < rowCountOne; i++)
    for (j = 0; j < columnCountOne; j++)
      scanf("%d", &firstMatrix[i][j]);

  prints("Number of rows of second matrix : ");
  scanf("%d", &rowCountTwo);

  prints("Number of columns of second matrix : ");
  scanf("%d", &columnCountTwo);

  if (columnCountOne != rowCountTwo)
    prints("Matrices with entered orders cannot be multiplied.\n");
  else
  {
    prints("Elements of second matrix : \n");

    for (i = 0; i < rowCountTwo; i++)
      for (j = 0; j < columnCountTwo; j++)
        scanf("%d", &secondMatrix[i][j]);

    for (i = 0; i < rowCountOne; i++) {
      for (j = 0; j < columnCountTwo; j++) {
        for (k = 0; k < rowCountTwo; k++) {
          sum = sum + firstMatrix[i][k]*secondMatrix[k][j];
        }

        resultMatrix[i][j] = sum;
        sum = 0;
      }
    }

    prints("After Multiplication, the result is : \n");

    for (i = 0; i < rowCountOne; i++) {
      for (j = 0; j < columnCountTwo; j++)
        printi(resultMatrix[i][j]);
    }
  }

  return 0;
}