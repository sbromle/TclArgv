#!/usr/bin/env tclsh

proc find_plugins {pluginPath} {
	set pattern [file join $pluginPath *.so]
	set files [glob -nocomplain $pattern]
	set files [lsort -dictionary $files]
	return $files
}


# Returns a two-element lists of lists.
# The first element is a list of cached system installed plugins.
# The second element is a list of cached user installed plugins.
proc scanForPlugins {plugindir} {
	global plugins_info
	global env
	if {[info exists plugins_info(Nlibs,sys)]} {
		puts stderr "Warning: scanForPlugins only set up for startup only."
		puts stderr "Warning: Cannot yet rescan for plugins."
		return 0
	}

	# look for the system installed plugins
	set sysplugs [find_plugins $plugindir]
	puts "Found [llength $sysplugs] plugins on the system."
	set plugins_info(Nlibs,sys) [llength $sysplugs]
	# cache the plugins that were found
	set cachepath [file join $env(HOME) .mvth cache]
	file mkdir $cachepath
	# copy the libraries to this directory
	set sysplugcache {}
	foreach p $sysplugs {
		file copy $p $cachepath
		lappend sysplugcache [file join $cachepath [file tail $p]]
	}
	set plugins_info(libs,sys) $sysplugcache

	# look for the user installed plugins
	set userplugsdir [file join $env(HOME) .mvth plugins]
	set userplugs [find_plugins $userplugsdir]
	puts "Found [llength $userplugs] plugins in $userplugsdir"
	set plugins_info(Nlibs,user) [llength $userplugs]
	# cach(copy) the libraries to this directory
	set userplugcache {}
	foreach p $userplugs {
		file copy $p $cachepath
		lappend userplugcache [file join $cachepath [file tail $p]]
	}
	set plugins_info(libs,user) $userplugcache

	return [list $sysplugcache $userplugcache]
}
