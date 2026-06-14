#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define TARGET   "/usr/share/applications/fake-open.desktop"
#define REDIRECT "/app/share/applications/fake-open.desktop"

static int redirect_match(const char *path)
{
    return path && strcmp(path, TARGET) == 0;
}

static const char *redirect_path(const char *path)
{
    return redirect_match(path) ? REDIRECT : path;
}

/* ---- open ---- */
typedef int (*orig_open_t)(const char *, int, ...);
int open(const char *pathname, int flags, ...)
{
    orig_open_t orig = (orig_open_t)dlsym(RTLD_NEXT, "open");
    const char *rp = redirect_path(pathname);
    if (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = va_arg(ap, mode_t);
        va_end(ap);
        return orig(rp, flags, mode);
    }
    return orig(rp, flags);
}

int open64(const char *pathname, int flags, ...)
{
    orig_open_t orig = (orig_open_t)dlsym(RTLD_NEXT, "open64");
    const char *rp = redirect_path(pathname);
    if (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = va_arg(ap, mode_t);
        va_end(ap);
        return orig(rp, flags, mode);
    }
    return orig(rp, flags);
}

/* ---- openat ---- */
typedef int (*orig_openat_t)(int, const char *, int, ...);
int openat(int dirfd, const char *pathname, int flags, ...)
{
    orig_openat_t orig = (orig_openat_t)dlsym(RTLD_NEXT, "openat");
    const char *rp = redirect_path(pathname);
    if (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode_t mode = va_arg(ap, mode_t);
        va_end(ap);
        return orig(dirfd, rp, flags, mode);
    }
    return orig(dirfd, rp, flags);
}

/* ---- stat (64-bit: stat64 is same as stat) ---- */
typedef int (*orig_stat_t)(const char *, struct stat *);
int stat(const char *path, struct stat *buf)
{
    orig_stat_t orig = (orig_stat_t)dlsym(RTLD_NEXT, "stat");
    return orig(redirect_path(path), buf);
}

/* __xstat is the glibc internal that stat() resolves to */
typedef int (*orig_xstat_t)(int, const char *, struct stat *);
int __xstat(int ver, const char *path, struct stat *buf)
{
    orig_xstat_t orig = (orig_xstat_t)dlsym(RTLD_NEXT, "__xstat");
    return orig(ver, redirect_path(path), buf);
}

/* ---- lstat ---- */
typedef int (*orig_lstat_t)(const char *, struct stat *);
int lstat(const char *path, struct stat *buf)
{
    orig_lstat_t orig = (orig_lstat_t)dlsym(RTLD_NEXT, "lstat");
    return orig(redirect_path(path), buf);
}

int __lxstat(int ver, const char *path, struct stat *buf)
{
    orig_xstat_t orig = (orig_xstat_t)dlsym(RTLD_NEXT, "__lxstat");
    return orig(ver, redirect_path(path), buf);
}

/* ---- access ---- */
typedef int (*orig_access_t)(const char *, int);
int access(const char *pathname, int mode)
{
    orig_access_t orig = (orig_access_t)dlsym(RTLD_NEXT, "access");
    return orig(redirect_path(pathname), mode);
}

int faccessat(int dirfd, const char *pathname, int mode, int flags)
{
    typedef int (*orig_faccessat_t)(int, const char *, int, int);
    orig_faccessat_t orig = (orig_faccessat_t)dlsym(RTLD_NEXT, "faccessat");
    return orig(dirfd, redirect_path(pathname), mode, flags);
}

/* ---- fopen ---- */
typedef FILE *(*orig_fopen_t)(const char *, const char *);
FILE *fopen(const char *pathname, const char *mode)
{
    orig_fopen_t orig = (orig_fopen_t)dlsym(RTLD_NEXT, "fopen");
    return orig(redirect_path(pathname), mode);
}

FILE *fopen64(const char *pathname, const char *mode)
{
    orig_fopen_t orig = (orig_fopen_t)dlsym(RTLD_NEXT, "fopen64");
    return orig(redirect_path(pathname), mode);
}
