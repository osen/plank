#include <plank/plank.h>

struct Employee
{
  int wage;
};

int main()
{
  vector(struct Employee) *emps = NULL;
  struct Employee emp = {0};
  sstream *helloMessage = NULL;

  emps = vector_new(struct Employee);
  vector_push_back(emps, emp);
  vector_push_back(emps, emp);
  vector_push_back(emps, emp);
  vector_push_back(emps, emp);

  helloMessage = sstream_new();
  sstream_push_cstr(helloMessage, "Hello World");
  printf("%s\n", sstream_cstr(helloMessage));

  vector_delete(emps);
  sstream_delete(helloMessage);

  return 0;
}

