/*
 * This file is part of MVTH - the Machine Vision Test Harness.
 *
 * Memory utilities.
 *
 * Copyright (C) 2003,2004 Samuel P. Bromley <sam@sambromley.com>
 *
 * This code is licensed under the terms and conditions of
 * the GNU GPL. See the file COPYING in the top level directory of
 * the MVTH archive for details.
 */

#include <stdio.h>
#include <stdlib.h>

double **twoDdouble(int w, int h)
{
	int i;
	double **t;

	t=(double **)malloc(h*sizeof(double*));
	t[0]=(double*)calloc(w*h,sizeof(double));
	for (i=1;i<h;i++)
		t[i]=t[0]+i*w;
	return t;
}

int **twoDint(int w, int h)
{
	int i;
	int **t;

	t=(int **)malloc(h*sizeof(int*));
	t[0]=(int*)calloc(w*h,sizeof(int));
	for (i=1;i<h;i++)
		t[i]=t[0]+i*w;
	return t;
}

short **twoDshort(int w, int h)
{
	int i;
	short **t;

	t=(short **)malloc(h*sizeof(short*));
	t[0]=(short*)calloc(w*h,sizeof(short));
	for (i=1;i<h;i++)
		t[i]=t[0]+i*w;
	return t;
}

char **twoDchar(int w, int h)
{
	int i;
	char **t;

	t=(char **)malloc(h*sizeof(char*));
	t[0]=(char*)calloc(w*h,sizeof(char));
	for (i=1;i<h;i++)
		t[i]=t[0]+i*w;
	return t;
}

void free2D(void *t)
{
	char **p;
	p=(char**)t;
	if (p!=NULL)
	{
		if (p[0]!=NULL)
		{
			free(p[0]);
			p[0]=NULL;
		}
		free(p);
	}
	return;
}

unsigned char ***threeDuchar(int cols, int rows, int bands)
{
	int i;
	unsigned char ***t;
	t=(unsigned char***)malloc(rows*sizeof(unsigned char **));
	t[0]=(unsigned char**)malloc(rows*cols*sizeof(unsigned char*));
	t[0][0]=(unsigned char*)calloc(rows*cols*bands,sizeof(unsigned char));
	for (i=1;i<rows;i++) t[i]=t[0]+i*cols;
	for (i=1;i<rows*cols;i++) t[0][i]=t[0][0]+i*bands;
	return t;
}

double ***threeDdouble(int nx, int ny, int nz)
{
	int i;
	double ***zptr;
	double **yptr;
	double *xptr;

	/* needs to hold enough to store every element */
	xptr=(double*)calloc(nx*ny*nz,sizeof(double));
	/* needs to hold enough to store pointers to every row */
	yptr=(double**)malloc(ny*nz*sizeof(double*));
	/* needs to hold enough to store pointers to every plane */
	zptr=(double***)malloc(nz*sizeof(double**));
	/* set up the other planes */
	zptr[0]=yptr; /* first plane points to first row */
	/* other planes point to first row of each plane */
	for (i=1;i<nz;i++) zptr[i]=zptr[0]+i*ny;
	/* set up all the rows */
	yptr[0]=xptr; /* first row points to very first column */
	/* other rows point to first element of each row */
	for (i=1;i<ny*nz;i++) yptr[i]=yptr[0]+i*nx;
	return zptr;
}

void free3D(void *t)
{
	char ***p;
	p=(char***)t;
	if (p!=NULL)
	{
		if (p[0]!=NULL)
		{
			if (p[0][0]!=NULL)
			{
				free(p[0][0]);
				p[0][0]=NULL;
			}
			free(p[0]);
			p[0]=NULL;
		}
		free(p);
	}
	return;
}
