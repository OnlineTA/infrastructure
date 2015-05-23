#include <nss.h>    // nss_status
#include <pwd.h>    // struct passwd
#include <stdlib.h> // strtoul
#include <errno.h>  // errno
#include <string.h> // strncpy
#include <limits.h> // INT_MAX

#include <stdio.h>

#define SC sizeof(char)

#define SIZEOF_KUID 7

#define SET_ENT(to, src, dst, len)                  \
  if (bufpos + SC*len + SC > buflen)                \
    goto  erange;                                   \
  to = memcpy(dst + SC*bufpos, src, len);           \
  bufpos = bufpos + SC*len;                         \
  dst[bufpos] = '\0';                               \
  bufpos = bufpos + SC;


static const char shell[] = "/bin/false";
static const char dir[] = "/tmp";
static const char gecos[] = ",,,,";
static const char password[] = "x";

#define KUID_BASE 621924480
#define MYUID_BASE 100000
#define KUID_MAX 1554823197 + MYUID_BASE

const char base36[] = {'0','1','2','3','4','5','6','7','8',    \
                       '9','a','b','c','d','e','f','g','h',    \
                       'i','j','k','l','m','n','o','p','q',    \
                       'r','s','t','u','v','w','x','y','z'};

int to_base_36(unsigned long uid, char buffer[], int buflen) {
  int rem = 0;

  // Size of 64-bit ULONG_MAX in base36 + 1
  const int tmpbuflen = 14;
  char tmpbuf[tmpbuflen];
  memset(tmpbuf, 0, tmpbuflen);


  int i = tmpbuflen - 1;
  tmpbuf[i--] = '\0';
  while (uid > 0 && i >= 0) {
      rem = uid % 36;
      uid = uid / 36;
      tmpbuf[i--] = base36[rem];
    }
  int size = tmpbuflen - (i + 1);
  // Check that nothing is left of our number and
  // that we have enough buffer space to \0-terminate
  if ((buflen - size >= 0) && uid == 0) {
    // Copy reversed string to start of return buffer
    strncpy(buffer, &tmpbuf[i+1], buflen);
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
  SET_ENT(pwd->pw_passwd, password, buffer, sizeof(password));

  return bufpos;

 erange:
  return -1;
}

/** Check whether pointee is a valid, null-terminated kuid.
 *
 * A valid kuid matches the regex ^[a-z]{3}[0-9]{3}$.
 *
 * Return 0 if pointee is valid and null-terminated, and -1 otherwise.
 **/
int
validate_alphanum_kuid(const char* ptr) {
  int i;

  for (i = 0; i < 3; ++i, ++ptr) {
    if (*ptr < 'a' || *ptr > 'z') {
      return -1;
    }
  }
  // ptr should now point to first digit.

  for (i = 0; i < 3; ++i, ++ptr) {
    if (*ptr < '0' || *ptr > '9') {
      return -1;
    }
  }
  // ptr should now point to null-byte.

  if (*ptr) {
    return -1;
  }

  return 0;
}

int validate_uid(unsigned long uid) {
  if (!(uid >= MYUID_BASE && uid <= KUID_MAX)) {
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
  if (validate_alphanum_kuid(name) != 0) {
    goto enoent;
  }

  unsigned long uid = strtoul(name, NULL, 36) - KUID_BASE + MYUID_BASE;
  if (validate_uid(uid) < 0) {
    goto enoent;
  }

  // Current position counter
  int bufpos;
  if ((bufpos = init_buf(result, buffer, buflen)) <  0) {
    goto erange;
  }

  SET_ENT(result->pw_name, name, buffer, SIZEOF_KUID);
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
                                       char* buffer, size_t buflen,
                                       int *errnop) {

  if (validate_uid(uid) != 0) {
    goto enoent;
  }

  // Restore original uid range
  unsigned long luid = uid + KUID_BASE - MYUID_BASE;

  // Buffer for username
  #define namebuf_len 7
  char name[namebuf_len];

  // Convert uid back to base36
  if (to_base_36(luid, name, namebuf_len) != 0) {
    goto enoent;
  }

  if (validate_alphanum_kuid((const char*) name)) {
    goto enoent;
  }

  int name_len = strlen(name);

  int bufpos;
  if ((bufpos = init_buf(result, buffer, buflen)) < 0) {
    goto erange;
  }

  SET_ENT(result->pw_name, name, buffer, name_len);
  result->pw_uid = uid;
  result->pw_gid = 42;

  return NSS_STATUS_SUCCESS;

 enoent:
  *errnop = ENOENT;
  return NSS_STATUS_NOTFOUND;
 erange:
  // Request a larger buffer
  *errnop = ERANGE;
  return NSS_STATUS_TRYAGAIN;
}
