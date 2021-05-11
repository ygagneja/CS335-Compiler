// Program to catch all syntax errors together
int main(){
    int a =3, b= 5 // missing semicolon
    int c = a ** b; // ** should be an invalid operator
    int d = a / 0; // Division by zero error
    e = a + b; // Undeclared variable error
    int arr[6];
    arr[7] = 10; // Out of memory access error
    return 0;
}