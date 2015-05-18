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
#define MYUID_BASE 100000
#define KUID_MAX 1554823197 + MYUID_BASE


const char base36[] = {'0','1','2','3','4','5','6','7','8',    \
                       '9','a','b','c','d','e','f','g','h',    \
                       'i','j','k','l','m','n','o','p','q',    \
                       'r','s','t','u','v','w','x','y','z'};

int
to_base_36(unsigned int uid, char buffer[], int buflen)
{
  int i = 0;
  int rem = 0;

  if (!(uid <= INT_MAX))
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

int init_buf (struct passwd *pwd, char *buffer,
               size_t buflen) {

  int bufpos = 0;

  // Clear buffer
  memset (buffer, 0, buflen);

  SET_ENT(pwd->pw_dir, dir, buffer, sizeof(dir));
  SET_ENT(pwd->pw_shell, shell, buffer, sizeof(shell));
  SET_ENT(pwd->pw_gecos, gecos, buffer, sizeof(gecos));

  return bufpos;

 erange:
  return -1;
}

int validate_uid(unsigned long uid) {
  if (!(uid > MYUID_BASE && uid < KUID_MAX)) {
    return -1;
  }
  return 0;
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

  printf("base36 reporting in a %s\n", name);

  if (name_len != 6) {
    goto enoent;
  }
  printf("base36 reporting in b %s\n", name);
  unsigned long uid = strtoul(name, NULL, 36) - KUID_BASE + MYUID_BASE;
  if (validate_uid(uid) < 0) {
    goto enoent;
  }
  printf("base36 reporting in c %s\n", name);

  // Current position counter
  int bufpos;
  if ((bufpos = init_buf(result, buffer, buflen)) <  0) {
    goto erange;
  }
  printf("base36 reporting in d %s\n", name);

  SET_ENT(result->pw_name, name, buffer, name_len);
  result->pw_uid = (int) uid;
  result->pw_gid = 42; // TODO

  return NSS_STATUS_SUCCESS;

  enoent:
    *errnop = ENOENT;
    return NSS_STATUS_NOTFOUND;

 erange:
    // Request a larger buffer from the NSS subsystem
    *errnop = ERANGE;
    return NSS_STATUS_TRYAGAIN;

}

enum nss_status _nss_base36_getpwuid_r(uid_t uid, struct passwd *result,
                         char* buffer, size_t buflen, int *errnop) {

  if (validate_uid(uid) != 0) {
    goto enoent;
  }

  // Restore original uid range
  unsigned long luid = uid + KUID_BASE - MYUID_BASE;

  // Buffer for username
  char name[7];

  // Convert uid back to base36
  if (to_base_36(luid, name, 7) != 0) {
    goto enoent;
  }

  int name_len = strlen(name);

  int bufpos;
  if ((bufpos = init_buf(result, buffer, buflen)) < 0) {
    goto erange;
  }

  SET_ENT(result->pw_name, name, buffer, name_len);
  result->pw_uid = uid;
  result->pw_uid = 42;

  return NSS_STATUS_SUCCESS;

 enoent:
  *errnop = ENOENT;
  return NSS_STATUS_NOTFOUND;
 erange:
  // Request a larger buffer
  *errnop = ERANGE;
  return NSS_STATUS_TRYAGAIN;
}

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
