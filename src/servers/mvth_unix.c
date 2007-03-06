/*
 * This file is part of MVTH - the Machine Vision Test Harness.
 *
 * Main event loop, server side.
 *
 * Copyright (C) 2003,2004 Samuel P. Bromley <sam@sambromley.com>
 *
 * This code is licensed under the terms and conditions of
 * the GNU GPL. See the file COPYING in the top level directory of
 * the MVTH archive for details.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
/* next two lines for mkdir() */
#include <sys/stat.h>
#include <sys/types.h>
/* next line for readdir and opendir */
#include <dirent.h>

#include "MVTHCORE/cmd_utils.h"
#include "MVTHCORE/getCommand.h"
#include "MVTHCORE/do_command.h"
#include "MVTHCORE/functions.h"
#include "MVTHCORE/unix_reply.h"

/* external function that will be dynamically loaded */
extern int loopChain(void);

int verbose=1; /* a flag that one can toggle on off for extraneous
							  * output.
								*/

char *mmapdirname=NULL; /* name of directory holding mmap'ed files */
char *USERNAME=NULL;

void cleandirs(int a, void *ptr)
{
	DIR *tmpdir;
	struct dirent *tmpdirent;
	
	if (mmapdirname==NULL) return;
	tmpdir=opendir(mmapdirname);
	if (tmpdir!=NULL)
	{
		tmpdirent=readdir(tmpdir);
		while (tmpdirent!=NULL)
		{
			char path[1024];
			if (strcmp(tmpdirent->d_name,".")==0 || strcmp(tmpdirent->d_name,"..")==0)
			{
				tmpdirent=readdir(tmpdir);
				continue;
			}
			sprintf(path,"%s/%s",mmapdirname,tmpdirent->d_name);
			unlink(path);
			fprintf(stderr,"Unlinked `%s'\n",path);
			tmpdirent=readdir(tmpdir);
		}
	}
	closedir(tmpdir);
	rmdir(mmapdirname);
	fprintf(stderr,"Removed dir `%s'\n",mmapdirname);
	if (mmapdirname!=NULL) free(mmapdirname);
}

void exitfunc(int a, void *ptr)
{
	shutdown_unix_server();
	cleanup_dynamic_libraries();
}

extern void unix_signal_handler(int signum);

/* unix_server implementation of mvthprint function */
int cursock=-1;

int mvthprint(FILE *fp, char *fmt, ...)
{
	va_list ap;
	int n;
	char buff[1024];
	char *bigbuff=buff;
	va_start(ap,fmt);
	n=vsnprintf(buff,sizeof(buff),fmt,ap);
	if (n>=sizeof(buff))
	{
		/* then it did not fit in buff */
		bigbuff=(char*)malloc((n+1)*sizeof(char));
		n=vsnprintf(bigbuff,(n+1)*sizeof(char),fmt,ap);
	}
	va_end(ap);
	if (fp!=NULL)
	{
		fprintf(fp,bigbuff);
	}
	if (cursock>=0) unix_reply_(cursock,bigbuff);
	if (bigbuff!=buff) free(bigbuff);

	return n;
}

int main( int argc, char *argv[])
{
	int ret;
	Command *cmd=NULL;
	char *ptr=NULL;
	struct timespec ts;
	mode_t oldmask;
	char *env=NULL; /* environment variable to hold $USER */
	char homedir[1024];
	char mkdircmd[1024];

	/* try to get the username of the user */
	env=getenv("USER");
	if (env==NULL)
	{
		fprintf(stderr,"Problem determining username.\n");
		return EXIT_FAILURE;
	}
	else
	{
		fprintf(stderr,"Got USER=`%s'\n",env);
	}
	USERNAME=strdup(env);

	/* create the .mvth directories in the user directory */
	sprintf(homedir,getenv("HOME"));
	sprintf(mkdircmd,"mkdir -p %s/.mvth/plugins",homedir);
	system(mkdircmd);
	sprintf(mkdircmd,"mkdir -p %s/.mvth/cache",homedir);
	system(mkdircmd);

	mmapdirname=(char*)malloc(strlen(USERNAME)+strlen("/tmp/mvth.mmap_")+1);
	sprintf(mmapdirname,"/tmp/mvth.mmap_%s",USERNAME);
	oldmask=umask(0);
	ret=mkdir(mmapdirname,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	umask(oldmask);
	if (ret!=0)
	{
		if (errno==EEXIST)
		{
			fprintf(stderr,"***************************************\n");
			fprintf(stderr,"* WARNING: Directory %s already exists.\n",mmapdirname);
			fprintf(stderr,"* Make sure another instance of mvth is not running.\n");
			fprintf(stderr,"***************************************\n");
		}
		else
		{
			fprintf(stderr,"Error creating directory %s. Exiting.\n",mmapdirname);
			return EXIT_FAILURE;
		}
	}

	on_exit(cleandirs,NULL);

	init_dynamic_libraries();
	registerAllCommands();
	scanForPlugins(PLUGINDIR);
	registerAllPlugins(PLUGINDIR);

	if (argc!=1)
	{
		fprintf(stderr,"Usage: %s\n",argv[0]);
		return 1;
	}

	if (init_unix_server()!=0)
	{
		fprintf(stderr,"Error initializing server.\n");
		return EXIT_FAILURE;
	}

	/* Keep going until someone presses Ctrl-C */
	signal(SIGINT,unix_signal_handler);
	/* make sure things are cleaned up on exit */
	on_exit(exitfunc,NULL);


	while(1) {
		int client=-1;
		int ret;

		client=get_unix_client();
		if (client>0)
		{
			cursock=client;
			ret=get_unix_request(client,&ptr);
			if (ret!=0)
			{
				fprintf(stderr,"bad request from client.\n");
			}
			else
			{
				if (strcmp(ptr,"quit")==0)
				{
					unix_reply(client,"%s\n",SERVER_QUIT);
					end_unix_payload(client);
					break;
				}
				else
				{
					cmd=getCommand(ptr);
					if (cmd!=NULL)
					{
						ret=doCommand(cmd);
						freeCommand(cmd);
						switch(ret)
						{
							case -2:
								break;
							case S_NOCMD:
								unix_reply(client,"%d %s\n",S_NOCMD,SERVER_NOCMD);
								end_unix_payload(client);
								break;
							case S_HANDLED:
								unix_reply(client,"%d %s\n",S_HANDLED,SERVER_SUCCESS);
								end_unix_payload(client);
								break;
							case S_SUCCESS:
								unix_reply(client,"%d %s\n",S_SUCCESS,SERVER_SUCCESS);
								end_unix_payload(client);
							break;
							case S_QUIT:
								unix_reply(client,"%d %s\n",S_QUIT,SERVER_QUIT);
								end_unix_payload(client);
								break;
							case S_ERROR:
								unix_reply(client,"%d %s\n",S_ERROR,SERVER_ERROR);
								end_unix_payload(client);
								break;
							case S_USAGE:
								unix_reply(client,"%d %s\n",S_USAGE,SERVER_USAGE);
								end_unix_payload(client);
								break;
							case S_ARGS:
								unix_reply(client,"%d %s\n",S_ARGS,SERVER_ARGS);
								end_unix_payload(client);
								break;
							case S_NOEXIST:
								unix_reply(client,"%d %s\n",S_NOEXIST,SERVER_NOEXIST);
								end_unix_payload(client);
								break;
							case S_NOTFOUND:
								unix_reply(client,"%d %s\n",S_NOTFOUND,SERVER_NOTFOUND);
								end_unix_payload(client);
								break;
							case S_FULL:
								unix_reply(client,"%d %s\n",S_FULL,SERVER_FULL);
								end_unix_payload(client);
								break;
							case S_NOMEM:
								unix_reply(client,"%d %s\n",S_NOMEM,SERVER_NOMEM);
								end_unix_payload(client);
								break;
							case S_MISMATCH:
								unix_reply(client,"%d %s\n",S_MISMATCH,SERVER_MISMATCH);
								end_unix_payload(client);
								break;
							case S_IFORM:
								unix_reply(client,"%d %s\n",S_IFORM,SERVER_IFORM);
								end_unix_payload(client);
								break;
							case S_NOCLOBBER:
								unix_reply(client,"%d %s\n",S_NOCLOBBER,SERVER_NOCLOBBER);
								end_unix_payload(client);
								break;
							default:
								fprintf(stderr,"Odd reply from server.\n");
								break;
						}
					}
				}
				free(ptr);
			}

			close(client);
			cursock=-1;
		}
		/* ok, handle other events */
		loopChain(); /* execute any regisisterd chain loops */

		/* sleep for 0.001 seconds */
		ts.tv_sec=0;
		ts.tv_nsec=1000000L;
		nanosleep(&ts,NULL);
	}

	return EXIT_SUCCESS;
}
