#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char *
fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), '\0', DIRSIZ - strlen(p));
  return buf;
}
void Find(char *path, char *ans)
{
  int fd;
  char buf[512], *p;
  struct stat st;
  struct dirent de;
  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
  }

  switch (st.type)
  {
  case T_FILE:
    return;
  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
   
    p = buf + strlen(buf); //指向buf尾巴
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    { 
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);   //在buf后面加de.name
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0)
      {
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if (strcmp(fmtname(buf), ans) == 0)
        printf("%s\n", buf);
    }

    close(fd);
    if ((fd = open(path, 0)) < 0)
    {
      fprintf(2, "find: cannot open %s\n", path);
      return;
    }
    if (fstat(fd, &st) < 0)
    {
      fprintf(2, "find: cannot stat %s\n", path);
    }
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0)
      {
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_DIR)
      {
        buf[strlen(buf)] = '/';
        Find(buf, ans);
      }
    }

    break;
  }
  close(fd);
}
int main(int argc, char *argv[])
{
  char path[256];
  strcpy(path, argv[1]);

  if (argc != 3)
  {
    fprintf(1, "argc count error");
    exit(0);
  }
  int len = strlen(argv[2]);
  char p[256];
  strcpy(p, argv[2]);
  p[strlen(p)] = '\0';
  if (argv[1][strlen(argv[1]) - 1] != '/')
    path[strlen(argv[1])] = '/';
  Find(path, p);

  exit(0);
}
