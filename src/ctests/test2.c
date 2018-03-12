#include <plank/plank.h>

struct Employee
{
  int wage;
};

int main()
{
  struct Employee *emp = NULL;
  int tmp = 0;

  emp = palloc(struct Employee);

  /* emp->wage is guaranteed to be 0 */

  pfree(emp);

  /*emp->wage = 3;*/ /* A SIGSEGV signal is guaranteed to be produced */
  tmp = emp->wage; /* A SIGSEGV signal is guarenteed to be produced */

  return 0;
}

