/*
 * This file is part of MVTH - the Machine Vision Test Harness.
 *
 * Maintain contexts for generic data structures.
 *
 * Copyright (C) 2005 Samuel P. Bromley <sam@sambromley.com>
 *
 * This code is licensed under the terms and conditions of
 * the GNU GPL. See the file COPYING in the top level directory of
 * the MVTH archive for details.
 */

#ifndef GENERIC_CONTEXT_H
#define GENERIC_CONTEXT_H
#include "generic_contexts_types.h"

/* create a new GenericContext within which we can
 * store elements */
/* Returns:
 *   0 on SUCCESS
 *  -1 if a context already exists with that name 
 *  -2 if there is no room left in the _gcontexts array */
/* User must provide pointers to the CopyMemberFunc, FreeMemberFunc,
 * and ListMembersFunc functions */
extern int generic_context_new(char *name);

/* return a pointer to an object stored in variable 'varname'
 * within the Context 'gc'.
 */
extern void *generic_context_get_var(GenericContext *gc, char *varname);
extern void *generic_context_get_blob(GenericContext *gc, char *blobname);
/* save a pointer to an object stored in variable "varname"
 * within the Context 'gc'.
 */
extern int generic_context_set_var(GenericContext *gc, char *varname,void *ptr);
extern int generic_context_set_blob(GenericContext *gc, char *blobname,void *ptr);
/* remove an entry */
extern void *generic_context_steal_var(GenericContext *gc, char *varname);
extern void *generic_context_steal_blob(GenericContext *gc, char *blobname);
/* return a pointer to a context that has been
 * registered with MVTH under a specified name.
 */
extern GenericContext *generic_context_get_by_name(const char *name);
/* Register the data passed in via `copy` 
 * with the undo structure stored in gc. Note, the gc
 * structure must have been initialized to contain a pointer
 * to the function that knows how to register with the undo
 * of that context.
 */
extern void * generic_context_register_undo_var(GenericContext *gc, void *copy, char *varname);
/* Perform the context undo (that is, redo the last action) */
extern char * generic_context_undo_var(GenericContext *gc,int *errorid);
/* get the number of items stored in the context */
extern unsigned int generic_context_get_size_var(GenericContext *gc);
extern unsigned int generic_context_get_num_blobs(GenericContext *gc);
/* return a NULL terminated list of variable names in given context */
extern char ** generic_context_get_vars(GenericContext *gc,int *numvars);
extern char ** generic_context_get_blobs(GenericContext *gc,int *numblobs);
#endif
