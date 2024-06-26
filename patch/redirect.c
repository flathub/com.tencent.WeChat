#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


void *(*real_dlopen)(char const *, int);
FILE *(*real_fopen)(char const *, char const *);
FILE *(*real_fopen64)(char const *, char const *);
int (*real_stat)(const char *, struct stat *);
int (*real_stat64)(const char *, struct stat64 *);

void hook_path(const char *func, const char **path) {
    char *redirect = NULL;

    if (strcmp(*path, "/usr/lib/license/libsystemactivation.so") == 0) {
        redirect = "/app/lib/libsystemactivation.so";
    } else if (strcmp(*path, "/etc/os-release") == 0) {
        redirect = "/app/lib/os-release";
    } else if (strcmp(*path, "/boot") == 0) {
        redirect = "/app";
    } else if (strcmp(*path, "/sys/class/dmi/id/product_uuid") == 0) {
        redirect = ".xwechat/uuid";
    }


    if (redirect != NULL) {
        printf("%s: redirect from %s to: %s\n", func, *path, redirect);
        *path = redirect;
    }
}

void *dlopen(const char *__file, int __mode) {
    if (real_dlopen == NULL) {
        real_dlopen = dlsym(RTLD_NEXT, "dlopen");
    }

    if (__file != NULL) {
        hook_path("dlopen", &__file);
    }

    return (*real_dlopen)(__file, __mode);
}

FILE *fopen(const char *__file, const char *__mode) {
    if (real_fopen == NULL) {
        real_fopen = dlsym(RTLD_NEXT, "fopen");
    }

    if (__file != NULL) {
        hook_path("fopen", &__file);
    }

    return (*real_fopen)(__file, __mode);
}

FILE *fopen64(const char *__file, const char *__mode) {
    if (real_fopen64 == NULL) {
        real_fopen64 = dlsym(RTLD_NEXT, "fopen64");
    }

    if (__file != NULL) {
        hook_path("fopen64", &__file);
    }

    return (*real_fopen64)(__file, __mode);
}

int stat(const char *__file, struct stat *__buf) {
    if (real_stat == NULL) {
        real_stat = dlsym(RTLD_NEXT, "stat");
    }

    if (__file != NULL) {
        hook_path("stat", &__file);
    }

    return (*real_stat)(__file, __buf);
}

int stat64(const char *__file, struct stat64 *__buf) {
    if (real_stat64 == NULL) {
        real_stat64 = dlsym(RTLD_NEXT, "stat64");
    }

    if (__file != NULL) {
        hook_path("stat64", &__file);
    }

    return (*real_stat64)(__file, __buf);
}
