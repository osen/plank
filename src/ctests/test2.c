#include <plank/plank.h>

struct Employee
{
  int wage;
};

int main()
{
  struct Employee *emp = NULL;

  emp = palloc(struct Employee);

  /* emp->wage is guaranteed to be 0 */

  pfree(emp);
  emp->wage = 3; /* A SIGSEGV signal is guaranteed to be produced */

  return 0;
}

