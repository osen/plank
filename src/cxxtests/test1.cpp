class Employee
{
public:
  int wage;
};

int main()
{
  Employee* emp = new Employee();

  /* emp will leak */

  return 0;
}
