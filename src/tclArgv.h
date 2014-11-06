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
	const char *key;  /* The key string that flags the option in the argv array.*/
	int type;   /* Indicates the option type; see blow.                   */
	void *src;  /* Value to be used in setting dst; usage depends on type.*/
	void *dst;  /* Address of value to be modified; usage depends on type.*/
	const char *help; /* Documentation message describing this option.          */
} Tcl_ArgvInfoSB;

extern int
Tcl_ParseArgsObjvSB(
	Tcl_Interp *interp,		/* Place to store error message. */
	int *objcPtr,		/* Number of arguments in objv.  Modified
									 * to hold # args left in objv at end. */
	Tcl_Obj *CONST *objv, /* Array of arguments to be parsed. */
	Tcl_Obj ***remObjv, /* any remaining unprocessed arguments */
	Tcl_ArgvInfoSB *argTable	/* Array of option descriptions */
);

/* Append an array of double to a list */
extern int Tcl_ListObjAppendDoubles(Tcl_Interp *interp,
		double *array, unsigned int n,
		Tcl_Obj *rlist);

/*
 * Legal values for the type field of a Tcl_ArgInfo: see the
 * user documentation for details.
 */

#define TCL_ARGV_SB_OBJ               14
#define TCL_ARGV_SB_CONSTANT          15
#define TCL_ARGV_SB_INT               16
#define TCL_ARGV_SB_STRING            17
#define TCL_ARGV_SB_REST              18
#define TCL_ARGV_SB_FLOAT             19
#define TCL_ARGV_SB_FUNC              20
#define TCL_ARGV_SB_GENFUNC           21
#define TCL_ARGV_SB_HELP              22
#define TCL_ARGV_SB_END               23
#define TCL_ARGV_SB_IGNORE            24
#define TCL_ARGV_SB_IGNORE_ARG        25

#define TCL_ARGV_SB_AUTO_HELP \
	{"-help",	TCL_ARGV_SB_HELP,	 NULL,	NULL, \
		"Print summary of command-line options and abort"}

#define TCL_ARGV_SB_TABLE_END \
	{NULL,	TCL_ARGV_SB_END, NULL,	NULL, (char *) NULL}

#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif

#endif
