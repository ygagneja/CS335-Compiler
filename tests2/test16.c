// Code to test struct/union data type as function parameter and return.

void printf(char* str, ...);
void scanf(char* str, ...);

struct Employee {
    int Id;
};
  
// return type of the function is structure
Employee data(Employee E)
{
  
    // Assigning the values to elements
    E.Id = 45;
  
    // returning structure
    return (E);
}
  
// Driver code
int main()
{
    // creating object of Employee
    Employee Emp;
  
    // calling function data to assign value
    Emp = data(Emp);
  
    // display the output
    printf("%d", Emp.Id);
  
    return 0;
}