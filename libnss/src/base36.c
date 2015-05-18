#include <nss.h> // nss_status
#include <pwd.h> // struct passwd
#include <stdlib.h> // strtoul
#include <errno.h> // errno
#include <string.h> // strncpy
#include <limits.h> //INT_MAX

#include <stdio.h>

#define SC sizeof(char)

#define SET_ENT(to, src, dst, len)                  \
  if (bufpos + SC*len + SC > buflen)                \
    goto  erange;                                   \
  to = memcpy(dst + SC*bufpos, src, len);           \
  bufpos = bufpos + SC*len;                         \
  dst[bufpos] = '\0';                               \
  bufpos = bufpos + SC;


const char shell[] = "/bin/false";
const char dir[] = "/tmp";
const char gecos[] = ",,,,";


#define KUID_BASE 621924480
#define KUID_MAX 1554823197
#define MYUID_BASE 100000

const char base36[] = {'0','1','2','3','4','5','6','7','8',    \
                       '9','a','b','c','d','e','f','g','h',    \
                       'i','j','k','l','m','n','o','p','q',    \
                       'r','s','t','u','v','w','x','y','z'};

int
to_base_36(int uid, char buffer[], int buflen)
{
  int i = 0;
  int rem = 0;

  if (!(uid >= 0 && uid <= INT_MAX))
    return -1;

  while (uid > 0 && i < buflen)
    {
      rem = uid % 36;
      uid = uid / 36;
      buffer[i++] = base36[rem];
    }
  // Check that nothing is left of our number and
  // that we have enough buffer space to \0-terminate
  printf("%d\n", buflen);
  if (((buflen) - i > 0) && uid == 0)
    {
      buffer[i] = '\0';
      return 0;
    }
  return -1;
}


enum nss_status _nss_base36_setpwent (void) {
  return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_base36_endpwent (void) {
  return NSS_STATUS_NOTFOUND;
}

enum nss_status _nss_base36_getpwnam_r(const char *name,
                                         struct passwd *result,
                                         char *buffer, size_t buflen,
                                         int *errnop) {
  int name_len = strlen(name);

  printf("base36 reporting in %s\n", name);

  if (name_len != 6) {
    goto enoent;
  }
  printf("base36 reporting in %s\n", name);
  unsigned long uid = strtoul(name, NULL, 36) - KUID_BASE;
  if (!(uid > 0 && uid < KUID_MAX)) {
    goto enoent;
  }
  printf("base36 reporting in %s\n", name);
  uid = uid + MYUID_BASE;

  // Clear buffer
  memset (buffer, 0, buflen);

  // Current position counter
  int bufpos = 0;

  SET_ENT(result->pw_name, name, buffer, name_len);
  result->pw_uid = (int) uid;
  result->pw_gid = 42; // TODO
  SET_ENT(result->pw_dir, dir, buffer, sizeof(dir));
  SET_ENT(result->pw_shell, shell, buffer, sizeof(shell));
  SET_ENT(result->pw_gecos, gecos, buffer, sizeof(gecos));

  return NSS_STATUS_SUCCESS;

  enoent:
    *errnop = ENOENT;
    return NSS_STATUS_NOTFOUND;

  erange:
  *errnop = ERANGE;
  return NSS_STATUS_TRYAGAIN;

}

/*enum nss_status _nss_base36_getpwuid_r(uid_t uid, struct passwd *result,
                         char* buffer, size_t buflen, int *errnop)

  
{
  *errnop = ENOENT;
  return NSS_STATUS_NOTFOUND;
  }*/

/*int main() {
  char uname[7];
  //int ret =  to_base_36(621931145, uname, 7);
  int ret =  to_base_36(35, uname, 7);
  if (ret != 0) {
    printf("error\n");
  } else {
    puts(uname);
  }
  return 0;
  }*/
