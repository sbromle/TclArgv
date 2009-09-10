/*
 * tclArgv.c --
 *
 *	This file contains a procedure that handles table-based
 *	argv-argc parsing.
 *
 * Modifications from original are Copyright (c) 2006  Sam Bromley
 *
 * Based heavily on tkArgv.c :
 * 	Copyright (c) 1990-1994 The Regents of the University of California.
 * 	Copyright (c) 1994-1997 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include "tclArgv.h"

/*
 * Forward declarations for procedures defined in this file:
 */

static void	PrintUsage _ANSI_ARGS_((Tcl_Interp *interp,
		    Tcl_ArgvInfo *argTable));


/*
 *----------------------------------------------------------------------
 *
 * Tcl_ParseArgsObjv --
 *
 *	Process an objv array according to a table of expected
 *	command-line options.  See the manual page for more details.
 *
 * Results:
 *	The return value is a standard Tcl return value.  If an
 *	error occurs then an error message is left in the interp's result.
 *	Under normal conditions, both *objcPtr and *objv are modified
 *	to return the arguments that couldn't be processed here (they
 *	didn't match the option table, or followed an TCL_ARGV_REST
 *	argument).
 *
 * Side effects:
 *	Variables may be modified, or procedures may be called.
 *	It all depends on the arguments and their entries in
 *	argTable.  See the user documentation	for details.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_ParseArgsObjv(interp, objcPtr, objv, remObjv, argTable)
	Tcl_Interp *interp;		/* Place to store error message. */
	int *objcPtr;		/* Number of arguments in objv.  Modified
									 * to hold # args left in objv at end. */
	Tcl_Obj *CONST *objv;/* Array of arguments to be parsed. */
	Tcl_Obj ***remObjv;	/* Pointer to array of arguments that
												 * were not processed here.  */
	Tcl_ArgvInfo *argTable;	/* Array of option descriptions */
{
	Tcl_Obj **leftovers=NULL;
	int nrem=0;
	register Tcl_ArgvInfo *infoPtr;
	/* Pointer to the current entry in the
	 * table of argument descriptions. */
	Tcl_ArgvInfo *matchPtr;	/* Descriptor that matches current argument. */
	Tcl_Obj *curArg;		/* Current argument */
	char *str=NULL;
	register char c;		/* Second character of current arg (used for
											 * quick check for matching;  use 2nd char.
											 * because first char. will almost always
											 * be '-'). */
	int srcIndex;		/* Location from which to read next argument
									 * from objv. */
	int dstIndex;   /* used to keep track of current arguments being processed,
	                   primarily for error reporting. */
	int objc;			/* # arguments in objv still to process. */
	size_t length;		/* Number of characters in current argument. */

	if (remObjv!=NULL) {
		/* then we should copy the name of the command (0th argument) */
		nrem++;
		leftovers=(Tcl_Obj **)realloc((void*)leftovers,(nrem+1)*sizeof(Tcl_Obj*));
		leftovers[nrem-1]=objv[0];
		leftovers[nrem]=NULL;
	}

	/* ok, now start processing from the second element (1st argument) */
	srcIndex = dstIndex = 1;
	objc = *objcPtr-1;

	while (objc > 0) {
		curArg = (Tcl_Obj*)objv[srcIndex];
		srcIndex++;
		objc--;
		str=Tcl_GetStringFromObj(curArg,NULL);
		length = strlen(str);
		if (length > 0) {
			c = str[1];
		} else {
			c = 0;
		}

		/*
		 * Loop throught the argument descriptors searching for one with
		 * the matching key string.  If found, leave a pointer to it in
		 * matchPtr.
		 */

		matchPtr = NULL;
		infoPtr = argTable;
		for ( ;
				(infoPtr != NULL) && (infoPtr->type != TCL_ARGV_END);
				infoPtr++) {
			if (infoPtr->key == NULL) {
				continue;
			}
			if ((infoPtr->key[1] != c)
					|| (strncmp(infoPtr->key, str, length) != 0)) {
				continue;
			}
			if (infoPtr->key[length] == 0) {
				matchPtr = infoPtr;
				goto gotMatch;
			}
			if (matchPtr != NULL) {
				Tcl_AppendResult(interp, "ambiguous option \"", str,
						"\"", (char *) NULL);
				return TCL_ERROR;
			}
			matchPtr = infoPtr;
		}
		if (matchPtr == NULL) {

			/*
			 * Unrecognized argument.  Just copy it down, unless the caller
			 * prefers an error to be registered.
			 */

			if (remObjv==NULL) {
				Tcl_AppendResult(interp, "unrecognized argument \"",
						str, "\"", (char *) NULL);
				return TCL_ERROR;
			}
			dstIndex++; /* this argument is now handled */
			nrem++;
			/* allocate nrem (+1 extra for NULL terminator) pointers */
			leftovers=(Tcl_Obj **)realloc((void*)leftovers,(nrem+1)*sizeof(Tcl_Obj*));
			leftovers[nrem-1]=curArg;
			continue;
		}

		/*
		 * Take the appropriate action based on the option type
		 */

gotMatch:
		infoPtr = matchPtr;
		switch (infoPtr->type) {
			case TCL_ARGV_OBJ:
				if (objc == 0) {
					goto missingArg;
				} else {
					*(Tcl_Obj**)infoPtr->dst=(Tcl_Obj*)objv[srcIndex];
					srcIndex++;
					objc--;
				}
				break;
			case TCL_ARGV_CONSTANT:
				*((int *) infoPtr->dst) = (int) ((long)infoPtr->src);
				break;
			case TCL_ARGV_INT:
				if (objc == 0) {
					goto missingArg;
				} else {
					if(Tcl_GetIntFromObj(interp,
								(Tcl_Obj*)objv[srcIndex],(int*)infoPtr->dst)==TCL_ERROR)
					{
						Tcl_AppendResult(interp, "expected integer argument ",
								"for \"", infoPtr->key, "\" but got \"",
								Tcl_GetStringFromObj((Tcl_Obj*)objv[srcIndex],NULL),
								"\"", (char *) NULL);
						return TCL_ERROR;
					}
					srcIndex++;
					objc--;
				}
				break;
			case TCL_ARGV_STRING:
				if (objc == 0) {
					goto missingArg;
				} else {
					*((CONST char **)infoPtr->dst)
						= Tcl_GetStringFromObj((Tcl_Obj*)objv[srcIndex],NULL);
					srcIndex++;
					objc--;
				}
				break;
			case TCL_ARGV_REST:
				*((int *) infoPtr->dst) = dstIndex;
				goto argsDone;
			case TCL_ARGV_FLOAT:
				if (objc == 0) {
					goto missingArg;
				} else {
					if(Tcl_GetDoubleFromObj( interp,
								(Tcl_Obj*)objv[srcIndex], (double*)infoPtr->dst)
							== TCL_ERROR)
					{
						Tcl_AppendResult(interp, "expected floating-point argument ",
								"for \"", infoPtr->key, "\" but got \"",
								Tcl_GetStringFromObj((Tcl_Obj*)objv[srcIndex],NULL),
								"\"", (char *) NULL);
						return TCL_ERROR;
					}
					srcIndex++;
					objc--;
				}
				break;
			case TCL_ARGV_FUNC:
				{
					typedef int (ArgvFunc) _ANSI_ARGS_ ((char *, char *,
								CONST Tcl_Obj *));
					ArgvFunc *handlerProc;

					handlerProc = (ArgvFunc *) infoPtr->src;
					if ((*handlerProc)(infoPtr->dst, infoPtr->key,
								objv[srcIndex])) {
						srcIndex += 1;
						objc -= 1;
					}
					break;
				}
			case TCL_ARGV_GENFUNC:
				{
					typedef int (ArgvGenFunc)_ANSI_ARGS_((char *, Tcl_Interp *, 
								char *, int, Tcl_Obj *CONST*));
					ArgvGenFunc *handlerProc;

					handlerProc = (ArgvGenFunc *) infoPtr->src;
					objc = (*handlerProc)(infoPtr->dst, interp, infoPtr->key,
							objc, objv+srcIndex);
					if (objc < 0) {
						return TCL_ERROR;
					}
					break;
				}
			case TCL_ARGV_HELP:
				PrintUsage (interp, argTable);
				return TCL_ERROR;
			case TCL_ARGV_IGNORE:
				break;
			case TCL_ARGV_IGNORE_ARG:
				if (objc > 0) {
					srcIndex++;
					objc--;
				}
				break;
			default:
				{
					char buf[64 + TCL_INTEGER_SPACE];

					sprintf(buf, "bad argument type %d in Tcl_ArgvInfo",
							infoPtr->type);
					Tcl_SetResult(interp, buf, TCL_VOLATILE);
					return TCL_ERROR;
				}
		}
	}

	/*
	 * If we broke out of the loop because of an OPT_REST argument,
	 * copy the remaining arguments down.
	 */

argsDone:
	if (remObjv==NULL) /* nothing to do */
		return TCL_OK;
	if (objc>0) {
		leftovers=(Tcl_Obj **)realloc((void*)leftovers,(nrem+objc+1)*sizeof(Tcl_Obj*));
		while (objc) {
			leftovers[nrem]=objv[srcIndex];
			nrem++;
			srcIndex++;
			objc--;
		}
	}
	leftovers[nrem] = (Tcl_Obj*) NULL;
	*objcPtr = nrem;
	*remObjv=leftovers;
	return TCL_OK;

missingArg:
	Tcl_AppendResult(interp, "\"", str,
			"\" option requires an additional argument", (char *) NULL);
	return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * PrintUsage --
 *
 *	Generate a help string describing command-line options.
 *
 * Results:
 *	The interp's result will be modified to hold a help string
 *	describing all the options in argTable.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

	static void
PrintUsage(interp, argTable)
	Tcl_Interp *interp;		/* Place information in this interp's
												 * result area. */
	Tcl_ArgvInfo *argTable;	/* Array of command-specific argument
													 * descriptions. */
{
	register Tcl_ArgvInfo *infoPtr;
	int width, numSpaces;
#define NUM_SPACES 20
	static char spaces[] = "                    ";
	char tmp[TCL_DOUBLE_SPACE];

	/*
	 * First, compute the width of the widest option key, so that we
	 * can make everything line up.
	 */

	width = 4;
	for (infoPtr = argTable;
			infoPtr->type != TCL_ARGV_END;
			infoPtr++) {
		int length;
		if (infoPtr->key == NULL) {
			continue;
		}
		length = strlen(infoPtr->key);
		if (length > width) {
			width = length;
		}
	}

	Tcl_AppendResult(interp, "Command-specific options:", (char *) NULL);
	for (infoPtr = argTable;
			infoPtr->type != TCL_ARGV_END; infoPtr++) {
		if ((infoPtr->type == TCL_ARGV_HELP) && (infoPtr->key == NULL)) {
			Tcl_AppendResult(interp, "\n", infoPtr->help, (char *) NULL);
			continue;
		}
		Tcl_AppendResult(interp, "\n ", infoPtr->key, ":", (char *) NULL);
		numSpaces = width + 1 - strlen(infoPtr->key);
		while (numSpaces > 0) {
			if (numSpaces >= NUM_SPACES) {
				Tcl_AppendResult(interp, spaces, (char *) NULL);
			} else {
				Tcl_AppendResult(interp, spaces+NUM_SPACES-numSpaces,
						(char *) NULL);
			}
			numSpaces -= NUM_SPACES;
		}
		Tcl_AppendResult(interp, infoPtr->help, (char *) NULL);
		switch (infoPtr->type) {
			case TCL_ARGV_INT:
				{
					sprintf(tmp, "%d", *((int *) infoPtr->dst));
					Tcl_AppendResult(interp, "\n\t\tDefault value: ",
							tmp, (char *) NULL);
					break;
				}
			case TCL_ARGV_FLOAT:
				{
					sprintf(tmp, "%g", *((double *) infoPtr->dst));
					Tcl_AppendResult(interp, "\n\t\tDefault value: ",
							tmp, (char *) NULL);
					break;
				}
			case TCL_ARGV_STRING:
				{
					char *string;

					string = *((char **) infoPtr->dst);
					if (string != NULL) {
						Tcl_AppendResult(interp, "\n\t\tDefault value: \"",
								string, "\"", (char *) NULL);
					}
					break;
				}
			case TCL_ARGV_IGNORE:
			case TCL_ARGV_IGNORE_ARG:
				{
					Tcl_AppendResult(interp, "Ignored for compatibility.",
							(char *) NULL);
					break;
				}
			default:
				{
					break;
				}
		}
	}
}
