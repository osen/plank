<img src="https://raw.githubusercontent.com/osen/plank/master/docs/logo.png" width="250" />

# Plank #
## Memory instrumentation library ##
Plank is a library designed to expose heap related memory errors. It works by marking memory as forbidden rather than freeing it. This means that if a read or write attempt is made, the kernel produces a SIGSEGV signal.

*Plank can help detect the following kinds of memory errors:*

 - Use after free
 - Double delete
 - Memory leak
 - Dangling pointer

## C++ Usage ##
Just add the **src/plankmm/plank.cpp** file to your project. This then overrides the default new/delete and new[]/delete[] operators to instead use the internal memory allocation scheme used by Plank. When you start your program, after allocating some memory you should see some output in the console showing that Plank has successfully been hooked in. When the application exits, a memory overview will be output to the console so you will be notified of any leaks. For more examples, check out the example programs in **src/cxxtests**.

## C Usage ##
After including the library or integrating Plank with your project, use the **palloc(type)** and **pfree(ptr)** macro functions (defined from **plank/plank.h**) in place of **malloc(size)** and **free(ptr)**.

*An example can be seen below:*

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
      
      /* A SIGSEGV signal is guaranteed to be produced */
      emp->wage = 3;
    
      return 0;
    }

When this program is run, the first allocation will show some output in the console so you know Plank is being used. When the application exits, a memory overview will be output to the console so you can see any memory you have forgotten to free (with **palloc(ptr)**).

*An example output containing a leak can be seen below:*

    [plank]Statistics
      Allocations: 1
      Allocated Size: 4
      Total Allocations: 1
      Total Size: 4
      Unmanaged Deletions: 0
    [plank]Leak
      Type: struct Employee
      Size: 4

### Additional Utilities for C ###

Plank also provides utilities which aim to increase memory safety when writing programs in C. These include a dynamic array container (**vector**) and string building tool (**sstream**). Their usage is as follows:

    #include <plank/plank.h>
    
    #include <stdio.h>
    
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

I welcome feedback, feature requests and bug reports!
