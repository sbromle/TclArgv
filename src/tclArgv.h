#ifndef _TCL_PARSE_ARGV_H
#define _TCL_PARSE_ARGV_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *key;  /* The key string that flags the option in the argv array.*/
	int type;   /* Indicates the option type; see blow.                   */
	void *src;  /* Value to be used in setting dst; usage depends on type.*/
	void *dst;  /* Address of value to be modified; usage depends on type.*/
	char *help; /* Documentation message describing this option.          */
} Tcl_ArgvInfo;

extern int
Tcl_ParseArgsObjv(
	Tcl_Interp *interp,		/* Place to store error message. */
	int *objcPtr,		/* Number of arguments in objv.  Modified
									 * to hold # args left in objv at end. */
	Tcl_Obj *CONST *objv, /* Array of arguments to be parsed. */
	Tcl_Obj ***remObjv, /* any remaining unprocessed arguments */
	Tcl_ArgvInfo *argTable	/* Array of option descriptions */
);

/*
 * Legal values for the type field of a Tcl_ArgInfo: see the
 * user documentation for details.
 */

#define TCL_ARGV_OBJ               14
#define TCL_ARGV_CONSTANT          15
#define TCL_ARGV_INT               16
#define TCL_ARGV_STRING            17
#define TCL_ARGV_REST              18
#define TCL_ARGV_FLOAT             19
#define TCL_ARGV_FUNC              20
#define TCL_ARGV_GENFUNC           21
#define TCL_ARGV_HELP              22
#define TCL_ARGV_END               23
#define TCL_ARGV_IGNORE            24
#define TCL_ARGV_IGNORE_ARG        25

#define TCL_ARGV_AUTO_HELP \
	{"-help",	TCL_ARGV_HELP,	 NULL,	NULL, \
		"Print summary of command-line options and abort"}

#define TCL_ARGV_TABLE_END \
	{NULL,	TCL_ARGV_END, NULL,	NULL, (char *) NULL}

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif
