#!/usr/bin/env tclsh

namespace eval mvth {
	proc ls {args} {
		if {[llength $args]==1 && [lindex $args 0]=="-h"} {
			return "ls  -- acts like typical unix ls command\n"
		}
		eval exec ls $args
	}
	namespace export ls
}
