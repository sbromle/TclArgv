#!/usr/bin/env tclsh
#
# This file is part of MVTH - the Machine Vision Test Harness.
#
# Main event loop, client side.
#
# Copyright (C) 2003,2004 Samuel P. Bromley <sam@sambromley.com>
#
# This code is licensed under the terms and conditions of
# the GNU GPL. See the file COPYING in the top level directory of
# the MVTH archive for details.
#

proc getmoredata {sock} {
	gets $sock line
	if {$line==""} {
		close $sock
		exit
	}
	regsub -all -lineanchor -linestop {^.(.*$)} $line {\1} line
	puts stdout $line
}

set s [socket localhost 3494]
fconfigure $s -translation crlf -buffering line
puts $s $argv
fileevent $s readable [list getmoredata $s]
vwait forever
