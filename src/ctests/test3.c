#include <plank/plank.h>

struct Employee
{
  int wage;
};

int main()
{
  struct Employee *emp = NULL;

  emp = palloc(struct Employee);

  /* emp should leak */

  return 0;
}

