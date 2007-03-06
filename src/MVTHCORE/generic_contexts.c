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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <assert.h>
/* the following header contains the GenericContext typedef */
#include "generic_contexts_types.h"
#include "generic_contexts.h"

enum {MAX_VAR_NAME_LEN=128}; /* maximum number of entries
																	stored in any generic context */
#define CONTEXT_NAME_LENGTH 128
/* the form of this structure is actually hidden from
 * everyone external to this file. Use the functions
 * to access the elements of it rather than accessing them
 * directly.*/
struct GC_Struct {
	char name[CONTEXT_NAME_LENGTH]; /* human readable name given to context at creation time*/
	GHashTable *vars;  /* a hashtable of variables referenced by name */
	void *undo_data;    /* place hold to the datatype preserved from last action*/
	char *undo_varname; /* ptr to name of last modified variable */
	GHashTable *blobs; /* a hash of data blobs. A plugin can use
												this to register any blob of data within MVTH
												so that it will be preserved across plugin reloads. */
};

struct _PackedVarStruct {
	char name[MAX_VAR_NAME_LEN];
	void *var;
};

/* our master list of generic contexts */
GHashTable *_gcmaster=NULL;

/* create a new GenericContext within which we can
 * store elements */
/* Returns:
 *   0 on SUCCESS
 *  -1 if a context already exists with that name 
 *  -2 if there is no room left in the _gcmaster array */
int generic_context_new(char *name)
{
	gpointer orig_key;
	gpointer value;
	GenericContext *new_gc;
	/* first make sure we've allocated the array of
	 * contexts */
	if (_gcmaster==NULL)
	{
		_gcmaster=g_hash_table_new_full(g_str_hash,g_str_equal,
				NULL,free);
	}
	/* first check to see if there is already
	 * a context with the same name */
	if (g_hash_table_lookup_extended(_gcmaster,name,&orig_key,&value)==TRUE)
	{
		/* then we've already added a contexts with this name */
		return -1;
	}
	/* otherwise, we can store a new context in _gcmaster */
	new_gc=(GenericContext*)calloc(1,sizeof(GenericContext));
	snprintf(new_gc->name,CONTEXT_NAME_LENGTH,name);
	g_hash_table_insert(_gcmaster,new_gc->name,(gpointer)new_gc);
	/* initialize the variable hash table within the new context */
	new_gc->vars=g_hash_table_new(g_str_hash,g_str_equal);
	/* initialize the data blob hash table within the new context */
	new_gc->blobs=g_hash_table_new(g_str_hash,g_str_equal);
	return 0;
}

/* return a pointer to a context that has been
 * registered with MVTH under a specified name.
 */
GenericContext *generic_context_get_by_name(const char *name)
{
	GenericContext *gc;
	if (_gcmaster==NULL) return NULL;
	gc=(GenericContext*)g_hash_table_lookup(_gcmaster,name);
	if (gc==NULL) fprintf(stderr,"No gc called \"%s\" found\n",name);
	return gc;
}

/* return a pointer to an object reference by name "varname"
 * within the Context 'gc'.
 */
void *generic_context_get_var(GenericContext *gc, char *varname)
{
	struct _PackedVarStruct *pvs=NULL;
	if (gc->vars==NULL)
	{
		fprintf(stderr,"generic_context_get_var(): hash table not initialized!\n");
		return NULL;
	}
	pvs=g_hash_table_lookup(gc->vars,varname);
	if (pvs==NULL) return NULL;
	return pvs->var; /* return a pointer to the actual var */
}

/* return a pointer to a data blob reference by "blobname"
 * within the Context 'gc' */
void *generic_context_get_blob(GenericContext *gc, char *blobname)
{
	struct _PackedVarStruct *pvs=NULL;
	if (gc->blobs==NULL)
	{
		fprintf(stderr,"generic_context_get_var(): hash table not initialized!\n");
		return NULL;
	}
	pvs=g_hash_table_lookup(gc->blobs,blobname);
	if (pvs==NULL) return NULL;
	return pvs->var; /* return a pointer to the actual var */
}

/* save data in the context under a given name */
int generic_context_set_var(GenericContext *gc, char *varname, void *ptr)
{
	struct _PackedVarStruct *pvs=NULL;
	/* warn user if attempting to doubly assign a variable.
	 * (The variable should have been removed from the hash table at this
	 * point.)
	 */
	if (g_hash_table_lookup(gc->vars,varname)!=NULL)
		fprintf(stderr,"generic_context_set_var: Overwriting non-NULL variable!\n");
	pvs=(struct _PackedVarStruct*)malloc(sizeof(struct _PackedVarStruct));
	snprintf(pvs->name,sizeof(pvs->name),"%s",varname);
	pvs->var=ptr;
	g_hash_table_insert(gc->vars,pvs->name,pvs);
	return 0;
}

/* save a data blob in the context under a given name */
int generic_context_set_blob(GenericContext *gc, char *blobname, void *ptr)
{
	struct _PackedVarStruct *pvs=NULL;
	/* warn user if attempting to doubly assign a variable.
	 * (The variable should have been removed from the hash table at this
	 * point.)
	 */
	if (g_hash_table_lookup(gc->blobs,blobname)!=NULL)
		fprintf(stderr,"generic_context_set_var: Overwriting non-NULL variable!\n");
	pvs=(struct _PackedVarStruct*)malloc(sizeof(struct _PackedVarStruct));
	snprintf(pvs->name,sizeof(pvs->name),"%s",blobname);
	pvs->var=ptr;
	g_hash_table_insert(gc->blobs,pvs->name,pvs);
	return 0;
}

/* remove an variable entry */
void *generic_context_steal_var(GenericContext *gc, char *varname)
{
	struct _PackedVarStruct *pvs=NULL;
	void *ptr=NULL;
	if (gc->vars==NULL) return NULL;
	pvs=g_hash_table_lookup(gc->vars,varname);
	g_hash_table_steal(gc->vars,varname);
	if (pvs==NULL || pvs->var==NULL) return NULL;
	ptr=pvs->var;
	free(pvs);
	return ptr;
}
/* remove a blob */
void *generic_context_steal_blob(GenericContext *gc, char *blobname)
{
	struct _PackedVarStruct *pvs=NULL;
	void *ptr=NULL;
	if (gc->blobs==NULL) return NULL;
	pvs=g_hash_table_lookup(gc->blobs,blobname);
	g_hash_table_steal(gc->blobs,blobname);
	if (pvs==NULL || pvs->var==NULL) return NULL;
	ptr=pvs->var;
	free(pvs);
	return ptr;
}
/* Register the data stored in Context gc as "varname"
 * with the undo structure stored in gc.
 * Returns a pointer to the data previously
 * stored under reference "varname".
 * NOTE: The user must make a copy before registering
 *       it in the undo structure, or else changes will
 *       clobber it! So, copy whatever you want to preserve
 *       first, and then register the copy with the undo
 *       infrastructure.
 * NOTE: The user should free this returned data if
 *       it is no longer of use.
 */
void *generic_context_register_undo_var(GenericContext *gc, void *copy, char *varname)
{
	void *old_undo_data = gc->undo_data;
	if (gc->undo_varname!=NULL) free(gc->undo_varname);
	gc->undo_varname=strdup(varname);
	gc->undo_data=copy;
	return old_undo_data;
}

/* Perform the context undo (that is, undo the last action) */
char *generic_context_undo_var(GenericContext *gc, int *errorid)
{
	struct _PackedVarStruct *oldvar;
	struct _PackedVarStruct *pvs;
	void *olddata=NULL;
	/* has the undo structure been initialied */
	if (gc->undo_varname==NULL || gc->undo_data==NULL)
	{
		*errorid=-2;
		return NULL;
	}
	/* does the variable still exist since last registered with undo? */
	oldvar=
		(struct _PackedVarStruct *)g_hash_table_lookup(gc->vars,gc->undo_varname);
	if (oldvar==NULL)
	{
		*errorid=-1;
		return NULL; /* variable no longer exists */
	}
	olddata=oldvar->var;
	/* make a new packed var struct to replace the one being undone */
	pvs=(struct _PackedVarStruct*)malloc(sizeof(struct _PackedVarStruct));
	snprintf(pvs->name,sizeof(pvs->name),"%s",gc->undo_varname);
	pvs->var=gc->undo_data;
	/* remove the old key and value from the hash table */
	g_hash_table_steal(gc->vars,pvs->name);
	g_hash_table_insert(gc->vars,pvs->name,pvs);
	//gc->undo_data=oldvar->var;
	gc->undo_data=olddata;
	free(oldvar);
	return gc->undo_varname;
}
/* get the number of variables stored in the context */
unsigned int generic_context_get_size_var(GenericContext *gc)
{
	if (gc->vars==NULL) return 0;
	return g_hash_table_size(gc->vars);
}
/* get the number of blobs stored in the context */
unsigned int generic_context_get_num_blobs(GenericContext *gc)
{
	if (gc->vars==NULL) return 0;
	return g_hash_table_size(gc->blobs);
}


struct gavn {
	char **varnames;
	int curvar;
};

static void
vappend(gpointer key, gpointer value, gpointer user_data)
{
	struct gavn *gv=(struct gavn*)user_data;
	struct _PackedVarStruct *pvs;
	pvs=(struct _PackedVarStruct*)value;
	gv->varnames[gv->curvar]=strdup(pvs->name);
	gv->curvar++;
}

/* return a NULL terminated list of variable names stored in the hash table */
char **generic_context_get_vars(GenericContext *gc, int *numvars)
{
	unsigned int nelements=generic_context_get_size_var(gc);
	char **varnames=NULL;
	struct gavn gv;
	if (numvars!=NULL) *numvars=(int)nelements;
	varnames=(char**)malloc((nelements+1)*sizeof(char*));
	varnames[nelements]=NULL;
	gv.varnames=varnames;
	gv.curvar=0;
	g_hash_table_foreach(gc->vars,vappend,&gv);
	return varnames;
}
/* return a NULL terminated list of blob names stored in the hash table */
char **generic_context_get_blobs(GenericContext *gc, int *numvars)
{
	unsigned int nelements=generic_context_get_num_blobs(gc);
	char **varnames=NULL;
	struct gavn gv;
	if (numvars!=NULL) *numvars=(int)nelements;
	varnames=(char**)malloc((nelements+1)*sizeof(char*));
	varnames[nelements]=NULL;
	gv.varnames=varnames;
	gv.curvar=0;
	g_hash_table_foreach(gc->blobs,vappend,&gv);
	return varnames;
}
/* following function should not be exported via generic_contexts.h */
/* It is used internally by the MVTH core only */
static gboolean yes(gpointer key, gpointer value, gpointer user_data)
{
	free(value); /* not sure if we need this */
	return 1;
}
static gboolean free_all_variables(gpointer key, gpointer value, gpointer user_data)
{
	GenericContext *gc=(GenericContext*)value;
	g_hash_table_foreach_remove(gc->vars,yes,NULL);
	return 1;
}
static gboolean free_all_blobs(gpointer key, gpointer value, gpointer user_data)
{
	GenericContext *gc=(GenericContext*)value;
	g_hash_table_foreach_remove(gc->blobs,yes,NULL);
	return 1;
}

int free_all_generic_contexts(void)
{
	g_hash_table_foreach_remove(_gcmaster,free_all_variables,NULL);
	g_hash_table_foreach_remove(_gcmaster,free_all_blobs,NULL);
	return 0;
}
