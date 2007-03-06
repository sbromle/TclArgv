#!/usr/bin/env tclsh
#
# This file is part of MVTH - the Machine Vision Test Harness.
#
# Print list of commands. 
#
# Copyright (C) 2006 Samuel P. Bromley <sam@sambromley.com>
#
# This code is licensed under the terms and conditions of
# the GNU GPL. See the file COPYING in the top level directory of
# the MVTH archive for details.
#/

proc help { {option {}} } {
	set hstring {}

	if {$option=="-h"} {return "help -- print list of commands\n"}
	# find any namespaces
	set n [namespace children]
	puts $n
	foreach space [split $n] {
		puts $space
		if {$space=="::tcl"} continue
		set cmds [info commands ${space}::*]
		foreach c $cmds {
			append hstring [$c -h]
		}
	}
	return $hstring
}
