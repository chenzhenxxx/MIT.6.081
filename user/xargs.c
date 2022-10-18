#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
int main(int argc, char *argv[])
{
  int n = 0;
  char *canshu[MAXARG];
  char *cmd = argv[1];
  int index = 0;
  for (int i = 1; i < argc; i++)
  {
    canshu[index++] = argv[i];
  }
  int m = index;
  char c;
  while (1)
  {
    char buf[512];
    memset(buf, 0, sizeof(buf));
    char *p = buf;
    index = m;
    while (n = read(0, &c, 1) > 0)
    {
      
      if (c == '\n')
      {
        canshu[index] = buf;
        index++;
        canshu[index] = 0;
        if (fork() == 0)
        {
          canshu[index] = 0;
          exec(cmd, canshu);
        }
        else
          wait(0);
          break;
      }
      else
      {
        *p = c;
        p++;
      }
    }

    if (n == 0)
      break;
  }
  exit(0);
}
