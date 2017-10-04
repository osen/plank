class Employee
{
public:
  int wage;
};

int main()
{
  Employee* emp = new Employee();
  delete emp;

  /* SIGSEGV should be emitted */
  emp->wage = 5;

  return 0;
}
