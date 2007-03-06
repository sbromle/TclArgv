/*
 * This file is part of MVTH - the Machine Vision Test Harness.
 *
 * Reload the plugins.
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
#include "cmd_utils.h"
#include "MVTHCORE/server_reply_codes.h"
#include "functions.h"
#include "MVTHCORE/mvthprint.h"

static void usage(void)
{
	mvthprint(stderr,"\nplugins syntax:\n");
	mvthprint(stderr,"plugins [-h]\n\n");
	return;
}

MvthReplyCode plugins_cmd(Command *cmd)
{

	if (cmd->narg==2 && strncmp(cmd->arglist->next->str,"-h",2)==0)
	{
		usage();
		return S_HANDLED;
	}
	if (cmd->narg!=1) return S_USAGE;

	mvthprint(stderr,"Saving plugin state...\n");
	saveAllPluginStates();
	mvthprint(stderr,"Deregistering plugins...\n");
	deregisterAllPlugins();
	mvthprint(stderr,"Reloading all commands...\n");
	reloadAllCommands();
	mvthprint(stderr,"Reloading all plugins...\n");
	registerAllPlugins(PLUGINDIR);
	mvthprint(stderr,"Restoring plugin states...\n");
	restoreAllPluginStates();

	return S_SUCCESS;
}
