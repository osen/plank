#include <plank/plank.h>

#include <stdio.h>

struct Employee
{
  int wage;
  sstream *firstName;
  sstream *lastName;
};

struct Employee *EmployeeCreate()
{
  struct Employee *rtn = NULL;

  rtn = palloc(struct Employee);
  rtn->firstName = sstream_new();
  sstream_push_cstr(rtn->firstName, "Karsten");
  rtn->lastName = sstream_new();
  sstream_push_cstr(rtn->lastName, "Pedersen");

  return rtn;
}

void EmployeeDestroy(struct Employee *ctx)
{
  sstream_delete(ctx->firstName);
  sstream_delete(ctx->lastName);
  pfree(ctx);
}

void EmployeeDisplayName(struct Employee *ctx)
{
  printf("First name: %s\n", sstream_cstr(ctx->firstName));
  printf("Last name: %s\n", sstream_cstr(ctx->lastName));
}

int main(int argc, char *argv[])
{
  struct Employee *emp = NULL;

  printf("Hello World\n");

  emp = EmployeeCreate();
  emp->wage = 3;
  EmployeeDisplayName(emp);
  EmployeeDestroy(emp);

  return 0;
}
