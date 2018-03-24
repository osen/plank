#include <iostream>
#include <string>
#include <memory>
#include <vector>

class Employee
{
public:
  int wage;
  std::string firstName;
  std::string lastName;
};

class Manager : public Employee
{
public:
  int stuff;
};

int main(int argc, char* argv[])
{
  std::cout << "Hello World" << std::endl;

  std::vector<std::shared_ptr<Employee> > emps;

  for(size_t i = 0; i < 10; i++)
  {
    for(size_t j = 0; j < 9999; j++)
    {
      std::shared_ptr<Employee> emp(new Manager());
      emps.push_back(emp);
    }

    emps.clear();
  }

  Manager* man = new Manager();
  man->firstName = "Karsten";
  man->lastName = "Pedersen";
  delete man;
  //int wage = man->wage;
  //man->wage = 9999;
  //man->firstName = "Invalid";

  int* wages = new int[456];
  delete[] wages;
  //int wage = wages[9];

  return 0;
}
