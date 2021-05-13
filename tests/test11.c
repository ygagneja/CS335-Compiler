// Program to test arrays using matrix multiplication 

int main()
{
    int a[100], b[100], res[100], r1, c1, r2, c2, i, j, k;

    prints("Enter rows and columns for first matrix (r1*c1 <= 100) :\n");
    r1 = scani(); c1 = scani();
    prints("Enter rows and columns for second matrix (r1*c1 <= 100) :\n");
    r2 = scani(); c2 = scani();

    // If column of first matrix in not equal to row of second matrix,
    // ask the user to enter the size of matrix again.
    while (c1!=r2)
    {
        prints("Error! column of first matrix not equal to row of second\n");

        prints("Enter rows and columns for first matrix :\n");
        r1 = scani(); c1 = scani();
        prints("Enter rows and columns for second matrix :\n");
        r2 = scani(); c2 = scani();
    }

    // Storing elements of first matrix.
    prints("Enter elements of matrix 1 in row major form :\n");
    for(i = 0; i < r1; ++i)
        for(j = 0; j < c1; ++j)
        {
            a[i*c1 + j] = scani();
        }

    // Storing elements of second matrix.
    prints("Enter elements of matrix 2 in row major form :\n");
    for(i = 0; i < r2; ++i)
        for(j = 0; j < c2; ++j)
        {
            b[i*c2 + j] = scani();
        }

    // Multiplying matrix a and b and storing in array mult.
    for(i = 0; i < r1; ++i){
        for(j = 0; j < c2; ++j){
            res[i*c2 + j] = 0;
            for(k = 0; k < c1; ++k)
            {
                res[i*c2 + j] += a[i*c1 + k] * b[k*c2 + j];
            }
        }
    }

    // Displaying the multiplication of two matrix.
    prints("Output Matrix :\n");
    for(i = 0; i < r1; ++i)
    for(j = 0; j < c2; ++j)
    {
        printi(res[i*c2 + j]);
        if(j == c2-1)
            prints("\n");
    }

    return 0;
}