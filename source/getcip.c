#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <libgen.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct IPA {
  char ip[16];
  char name[32];
} IPA;

typedef struct Args {
  short showColor;
  short hideName;
  short maxCount;
  char *ipFormat;
  char *nameFormat;
} Args;

char *mkLowerCase(char *mixed) {
  char *s = mixed;
  for (; *s; ++s) *s = tolower(*s);
  return mixed;
}

short idxOf(char *substring, char *string) {
  char *result = strstr(string, substring);
  return result ? result - string : -1;
}

short idxOfL(char *substring, char *string) {
  char *sub = strdup(substring), *str = strdup(string), res = 0;
  mkLowerCase(sub);mkLowerCase(str); res = idxOf(sub,str);
  free(sub); free(str); return (short)res;
}

int main(int argc, char **argv) {
  struct ifaddrs *tmp, *addrs;
  char *s, format[255];
  char ipWColor[] = "\x1b[0m%s\x1b[0;34m%s";
  char ipWOColor[] = "%s%s";
  char nameWColor[] = "\x1b[1;37m:\x1b[0;33m%s\x1b[0m";
  char nameWOColor[] = ":%s";
  int first = 1, i, shown = 0, ipCount = 0;
  IPA ips[10];
  Args args;

  memset(&args, 0L, sizeof(Args));
  args.maxCount = 10;
  args.showColor = 1;
  args.hideName = 0;
  getifaddrs(&addrs);
  tmp = addrs;

  if (argc > 1 && (~idxOfL("?", argv[1]) || ~idxOfL("help", argv[1]))) {
    printf("Usage: %s <args.maxCount> [--no-color] [--hide-name]\n", basename(argv[0]));
    return 0;
  }

  for (i=1;i<argc;i++)
    if (~idxOfL("--hide-name",argv[i])) { args.hideName = 1; break;}

  for (i=1;i<argc;i++)
    if (~idxOfL("--no-color",argv[i])) { args.showColor = 0; break; }

  for (i=1;i<argc;i++)
    if (!(args.maxCount = atoi(argv[i]))) { args.maxCount = 10; }

  i = 0;
  while(tmp) {
    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
      struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
      sprintf(ips[i].ip, "%s", inet_ntoa(pAddr->sin_addr));
      sprintf(ips[i].name, "%s", tmp->ifa_name);

      /* convert name tolower */
      s = ips[i].name; for (; *s; ++s) *s = tolower(*s); s = ips[i].name;
      ipCount++;
      i++;
    }

    tmp = tmp->ifa_next;
  }

  strcpy(format, args.showColor ? ipWColor : ipWOColor);
  if (!args.hideName) {
    strcat(format, args.showColor ? nameWColor : nameWOColor);
  }

  for (i = 0, first = 1; i < ipCount; i++) {
    if (!~idxOfL("lo", ips[i].name)) {
      printf(
        format,
        first ? "" : ",",
        ips[i].ip,
        args.hideName ? "" : ips[i].name
      );
      first = 0;
      shown++;
      if (shown >= args.maxCount) break;
    }
  }
  printf("\n");

  freeifaddrs(addrs);
}
