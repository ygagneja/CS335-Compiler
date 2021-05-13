// Code to test struct/union data type as function parameter and return. **Modify to handle struct ptrs** 
struct Employee {
    int Id;
};

void data(struct Employee* E)
{
    // Assigning the values to elements
    E->Id = 45;
    E->Id = 54;
}
  
int main()
{
    struct Employee Emp;
    data(&Emp);
    printi(Emp.Id);  
    return 0;
}