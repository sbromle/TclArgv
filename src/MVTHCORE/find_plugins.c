/*
 * This file is part of MVTH - the Machine Vision Test Harness.
 *
 * Find plugins.
 *
 * Copyright (C) 2003,2004 Samuel P. Bromley <sam@sambromley.com>
 *
 * This code is licensed under the terms and conditions of
 * the GNU GPL. See the file COPYING in the top level directory of
 * the MVTH archive for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

static int filter(const struct dirent *d)
{
	int len;
	len=strlen(d->d_name);
	if (len<4) return 0;

	if (strcoll(".so",&(d->d_name[len-3]))==0
			&& d->d_name[0]!='.') return 1; /* a library */

	return 0;
}

/* returns an array of strings of plugin filenames,
 * and n is set to the number of plugins found.
 */
char **find_plugins(const char *plugin_path, int *n)
{
	struct dirent **listing;
	int i,nl;
	char **plugins=NULL;

	nl=scandir(plugin_path,&listing,filter,alphasort);
	if (nl<=0)
	{
		fprintf(stderr,"Found 0 plugins.\n");
		*n=0;
		return NULL;
	}

	*n=nl;

	fprintf(stderr,"Found %d plugins.\n",nl);

	if (nl>0)
	{
		plugins=(char**)malloc(nl*sizeof(char *));
		for (i=0;i<nl;i++)
		{
			plugins[i]=strdup(listing[i]->d_name);
			free(listing[i]);
		}
		free(listing);
	}
	return plugins;
}
