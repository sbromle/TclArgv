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

#ifndef FIND_PLUGINS_H
#define FIND_PLUGINS_H

/* returns an array of strings of plugin filenames,
 * and n is set to the number of plugins found.
 */
extern char **find_plugins(const char *plugin_path, int *n);

#endif
