#include <nss.h> // nss_status
#include <pwd.h> // struct passwd
#include <stdlib.h> // strtoul
#include <errno.h> // errno
#include <string.h> // strncpy

#include <stdio.h>

enum nss_status
_nss_onlineta_setpwent (void)
{
  return NSS_STATUS_SUCCESS;
}

enum nss_status
_nss_onlineta_endpwent (void)
{
  return NSS_STATUS_SUCCESS;
}

enum nss_status
_nss_onlineta_getpwnam_r(const char *name, struct passwd *result,
  char *buffer, size_t buflen, int *errnop)
{
  char *kuid;

  if (strlen(name) != 6)
  {
    *errnop = errno;
    return NSS_STATUS_NOTFOUND;
  }

  kuid = (char*)malloc(7 * sizeof(char));
  strncpy(kuid, name, 6);
  kuid[7] = '\0';

  result->pw_name = kuid;
  result->pw_uid = strtoul(name, NULL, 36) - strtoul("aaa000", NULL, 36)
                    + 100000;
  result->pw_gid = 42; // TODO
  result->pw_dir = "/";
  result->pw_shell = "";

  buffer = buffer;
  buflen = buflen;
  errnop = errnop;
  *errnop = errno;
  return NSS_STATUS_SUCCESS;
}
