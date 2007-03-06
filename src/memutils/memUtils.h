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

#ifndef MEMUTILS_H
#define MEMUTILS_H


extern double **twoDdouble(int w, int h);
extern int **twoDint(int w, int h);
extern short **twoDshort(int w, int h);
extern char **twoDchar(int w, int h);
extern void free2D(void *t);

extern unsigned char ***threeDuchar(int cols, int rows, int bands);
extern double ***threeDdouble(int nx, int ny, int nz);
extern void free3D(void *t);

#endif
