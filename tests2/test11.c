// Program to test multi dimensional arrays using matrix multiplicatuon

int main()
{
  int dim1 = 2, dim2 = 2, sum;
  int i, j, k;
  int firstMatrix[4], secondMatrix[4], resultMatrix[4];

  prints("Elements of first matrix : \n");

  for (i = 0; i < dim1; i++)
    for (j = 0; j < dim1; j++)
      firstMatrix[i*dim1 + j] = scani();

  prints("Elements of second matrix : \n");

  for (i = 0; i < dim2; i++)
    for (j = 0; j < dim2; j++)
      secondMatrix[i*dim2 + j] = scani();

  for (i = 0; i < dim1; i++) {
    for (j = 0; j < dim2; j++) {
      for (k = 0; k < dim2; k++) {
        sum = sum + firstMatrix[i*dim1 + k]*secondMatrix[k*dim2 + j];
      }

      resultMatrix[i*dim2 + j] = sum;
      sum = 0;
    }
  }

  prints("After Multiplication, the result is : \n");

  for (i = 0; i < dim2; i++) {
    for (j = 0; j < dim2; j++) {
      printi(resultMatrix[i*dim2 + j]);
    }
  }

  return 0;
}