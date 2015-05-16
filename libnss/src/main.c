#include <nss.h> // nss_status
#include <pwd.h> // struct passwd
#include <stdlib.h> // strtoul
#include <errno.h> // errno

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
  printf("HEJ!\n");
  result->pw_uid = strtoul(name, NULL, 36);

  buffer = buffer;
  buflen = buflen;
  *errnop = errno;
  return NSS_STATUS_NOTFOUND;
}
