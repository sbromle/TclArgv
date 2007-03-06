#
# This file is part of MVTH - the Machine Vision Test Harness.
#
# Set a global variable.
#
# Copyright (C) 2003,2004 Samuel P. Bromley <sam@sambromley.com>
#
# This code is licensed under the terms and conditions of
# the GNU GPL. See the file COPYING in the top level directory of
# the MVTH archive for details.
#

namespace eval mvth {
	variable mvth_globals
	set mvth_globals(testglobal) 0

	proc gset { { varname {} } { value {} } } {
		variable mvth_globals
		set ans {}
		if { $varname=={} && $value=={} } {
			foreach n [array names mvth_globals] {
				append ans "$n\n"
			}
			return $ans
		} elseif { $value=={} } {
			if {$varname=="-h"} {
				return "gset -- query/set global variables\n"
			}
			if {[info exists mvth_globals($varname)]} {
				return $mvth_globals($varname)
			}
		} else {
			if {[info exists mvth_globals($varname)]} {
				set mvth_globals($varname) $value
				return $value
			} else {
				return "No such global variable registered."
			}
		}
	}
	namespace export gset
}
