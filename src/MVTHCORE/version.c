/* File to get the version of a library */
extern int VERSION_MAJOR;
extern int VERSION_MINOR;
extern int VERSION_PATCH;

int get_version(int *major, int *minor, int *patch)
{
	*major=VERSION_MAJOR;
	*minor=VERSION_MINOR;
	*patch=VERSION_PATCH;
	return 0;
}
