/*
 * This file is part of MVTH - the Machine Vision Test Harness.
 *
 * Function definition and data types needed for
 * building an MVTH plugin.
 *
 * Copyright (C) 2005 Samuel P. Bromley <sam@sambromley.com>
 *
 * This code is licensed under the terms and conditions of
 * the GNU GPL. See the file COPYING in the top level directory of
 * the MVTH archive for details.
 */

#ifndef MVTH_PLUGIN_H
#define MVTH_PLUGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ltdl.h>
#include <mvthCore.h>

/* a flag controlling verbosity level */
extern int verbose; 

/* function to register a command with the command tree structure */
extern void registerCommand2(const char *cname, /* name of the command */
		int matchlen, /* number of letters that should be matched */
		char *symbol, /* pointer to the function */
		lt_dlhandle handle, /* handle to library */
		const char *desc);  /* description of command */

/* the DEPRECATED Arg structure */
/********** new code should not use this! ********/
typedef struct Arg {
	char *str;
	struct Arg *next;
} Arg, Arglist;

/* the command structure */
typedef struct {
  int narg;
  char *cmdline; /* original, unparsed command line */
  Arglist *arglist;
} Command;


/* a function to reply back to the caller */
extern int mvthprint(FILE *fp, char *fmt, ...);

/* functions to register callbacks to handle global settings */
typedef int (*SetGlobalCallback)(char *argv, char *value);
extern void registerSGcallback(const char *vname, /* name of the command */
		int matchlen, /* number of letters that should be matched */
		const char *desc,  /* description of global variable */
		char *cbfunc, /* internal name of callback function */
		lt_dlhandle handle); /* handle to library */

/* reply codes for returning command success or failure to mvth */
typedef enum {
	S_HANDLED=0,  /* command already handled server response. */
	S_SUCCESS=100,
	S_QUIT   =105,
	S_PAYLOAD=200,
	S_ERROR  =400,
	S_USAGE,
	S_ARGS,
	S_NOEXIST,
	S_NOTFOUND,
	S_FULL,
	S_NOMEM,
	S_NOCMD,
	S_MISMATCH,
	S_IFORM,
	S_NOCLOBBER } MvthReplyCode;

#define SERVER_SUCCESS   "SUCCESS"
#define SERVER_QUIT      "QUIT"
/* a payload terminated by a line of only '.\r\n' */
#define SERVER_PAYLOAD   "PAYLOAD"  
#define SERVER_ERROR     "ERROR"
#define SERVER_USAGE     "USAGE"
#define SERVER_ARGS      "BADARGS"
#define SERVER_NOEXIST   "NOEXIST"
#define SERVER_NOTFOUND  "NOTFOUND"
#define SERVER_FULL      "NOROOM"
#define SERVER_NOMEM     "NOMEM"
#define SERVER_NOCMD     "NOSUCHCMD"
#define SERVER_MISMATCH  "MISMATCH"
#define SERVER_IFORM     "IMGFORMAT"
#define SERVER_NOCLOBBER "NOCLOBBER"

#endif
