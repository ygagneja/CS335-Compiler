// Code to test struct/union data type as function parameter and return.
struct Employee {
    int Id;
};
  
// return type of the function is structure
void data(struct Employee* E)
{
  
    // Assigning the values to elements
    E->Id = 45;
    E->Id = 54;
}
  
// Driver code
int main()
{
    // creating object of Employee
    struct Employee Emp;
  
    // calling function data to assign value
    data(&Emp);
  
    // display the output
    printi(Emp.Id);
  
    return 0;
}