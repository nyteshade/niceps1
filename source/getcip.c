#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <libgen.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IPS 10
#define MAX_NAME_LEN 32
#define MAX_IP_LEN 16
#define MAX_FORMAT_LEN 255

typedef struct IPA {
  char ip[MAX_IP_LEN];
  char name[MAX_NAME_LEN];
} IPA;

typedef struct Args {
  int showColor;
  int hideName;
  int maxCount;
  char *ipFormat;
  char *nameFormat;
} Args;

char *mkLowerCase(char *mixed) {
  char *s = mixed;
  for (; *s; ++s) *s = tolower(*s);
  return mixed;
}

int idxOf(char *substring, char *string) {
  char *result = strstr(string, substring);
  return result ? (int)(result - string) : -1;
}

int idxOfL(char *substring, char *string) {
  char *sub = strdup(substring);
  char *str = strdup(string);
  int res = 0;
  mkLowerCase(sub);
  mkLowerCase(str);
  res = idxOf(sub, str);
  free(sub);
  free(str);
  return res;
}

int main(int argc, char **argv) {
  struct ifaddrs *tmp, *addrs;
  char *s, format[MAX_FORMAT_LEN];
  char ipWColor[] = "\x1b[0m%s\x1b[0;34m%s";
  char ipWOColor[] = "%s%s";
  char nameWColor[] = "\x1b[1;37m:\x1b[0;33m%s\x1b[0m";
  char nameWOColor[] = ":%s";
  int first = 1, i, shown = 0, ipCount = 0;
  IPA ips[MAX_IPS];
  Args args;

  memset(&args, 0, sizeof(Args));
  args.maxCount = 10;
  args.showColor = 1;
  args.hideName = 0;
  getifaddrs(&addrs);
  tmp = addrs;

  if (argc > 1 && (~idxOfL("?", argv[1]) || ~idxOfL("help", argv[1]))) {
    printf("Usage: %s <args.maxCount> [--no-color] [--hide-name]\n", basename(argv[0]));
    return 0;
  }

  for (i = 1; i < argc; i++) {
    if (idxOfL("--hide-name", argv[i]) != -1) {
      args.hideName = 1;
      break;
    }
  }

  for (i = 1; i < argc; i++) {
    if (idxOfL("--no-color", argv[i]) != -1) {
      args.showColor = 0;
      break;
    }
  }

  for (i = 1; i < argc; i++) {
    int value = atoi(argv[i]);
    if (value != 0 || strcmp(argv[i], "0") == 0) {
      args.maxCount = value;
      break;
    }
  }

  i = 0;
  while (tmp && ipCount < MAX_IPS) {
    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
      struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
      snprintf(ips[i].ip, sizeof(ips[i].ip), "%s", inet_ntoa(pAddr->sin_addr));
      snprintf(ips[i].name, sizeof(ips[i].name), "%s", tmp->ifa_name);

      /* convert name to lowercase */
      s = ips[i].name;
      for (; *s; ++s) *s = tolower(*s);
      ipCount++;
      i++;
    }

    tmp = tmp->ifa_next;
  }

  snprintf(format, sizeof(format), "%s", args.showColor ? ipWColor : ipWOColor);
  if (!args.hideName) {
    strncat(format, args.showColor ? nameWColor : nameWOColor, sizeof(format) - strlen(format) - 1);
  }

  for (i = 0, first = 1; i < ipCount; i++) {
    if (idxOfL("lo", ips[i].name) == -1) {
      printf(
          format,
          first ? "" : ",",
          ips[i].ip,
          args.hideName ? "" : ips[i].name
      );
      first = 0;
      shown++;
      if (shown >= args.maxCount) {
        break;
      }
    }
  }
  printf("\n");

  freeifaddrs(addrs);
  return 0;
}
