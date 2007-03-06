#ifndef VERSION_H
#define VERSION_H
/* File to get the version of a library */
extern int VERSION_MAJOR;
extern int VERSION_MINOR;
extern int VERSION_PATCH;

extern int get_version(int *major, int *minor, int *patch);

#endif
