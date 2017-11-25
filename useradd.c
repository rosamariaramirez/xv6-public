#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#define passwdFile "/passwd"
#define maxLength 100

char *
itoa(int value, char *result, int base)
{
  // check that the base if valid
  if (base < 2 || base > 36)
  {
    *result = '\0';
    return result;
  }

  char *ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do
  {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
  } while (value);

  // Apply negative sign
  if (tmp_value < 0)
    *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr)
  {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}

int contains(char *str, char *pattern){
    int i=0, j=0;
    int sizeStr = strlen(str);
    int sizePattern = strlen(pattern);

    while(i<sizePattern && j<sizeStr && pattern[i] != str[j]){
      j++;
    }
    if(j==sizeStr){
      return 0;
    }
    while(i<sizePattern && j<sizeStr && pattern[i] == str[j]){
      i++;
      j++;
    }
    if(i==sizePattern){
      return 1;
    }else{
      return 0;
    }
}

int readLine(int fd, char *buf, int size)
{
    int i = 0, n = 0;
    char c;
    while(read(fd, &c, 1) > 0){
      n++;
      if(c != '\n'){
        buf[i] = c;
        i++;
        if(i == size-1){
          break;
        }
      }else{
        break;
      }
    }
    buf[i] = '\0';
    return n;
}

void nextId(char *currentId, char *buffer){
  int i = 0;
  char temp[2];
  while(currentId[i] != ':') {
    i++;
  }
  i++;
  int j = 0;
  while(currentId[i] != ':') {
    temp[j] = currentId[i];
    i++;
    j++;
  }
  j++;
  temp[j] = 0;
  itoa(atoi(temp) + 1, buffer, 10);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
 {
   printf(2, "Usage: useradd username password\n");
   exit();
 }

 int fd, n;
 char record[maxLength], id[2];

 fd = open(passwdFile, O_RDWR);
 while ((n = readLine(fd, record, sizeof(record))) > 0)
 {
   nextId(record, id);
   if (contains(record, argv[1]))
   {
     printf(2, "User already exists.\n");
     close(fd);
     exit();
   }
 }

 int len_uname;
 int len_paswd;
 len_uname = strlen(argv[1]);
 len_paswd = strlen(argv[2]);
 char newline = '\n';
 char colon = ':';
 write(fd, argv[1], len_uname);
 write(fd, &colon, 1);
 write(fd, id, strlen(id));
 write(fd, &colon, 1);
 write(fd, argv[2], len_paswd);
 write(fd, &colon, 1);
 write(fd, &newline, 1);
 close(fd);
 exit();
}
