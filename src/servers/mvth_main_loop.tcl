#!/usr/bin/env tclsh

proc start_mvth_server {port} {
	global mvth_server
	set mvth_server(main) [socket -server [list MvthAccept] $port]
}

proc pack_for_transmission {msg} {
	upvar $msg var
	regsub -all -lineanchor -linestop {(^.*$)} $var {.\1} var
	return $var
}

proc reply {sock msg} {
	set var $msg
	puts $sock [pack_for_transmission var]
}

proc req_close {sock args} {
	global mvth_server
	set mvth_server(req_close,$sock) 1
}

proc mvthprint {sock msg {fd {stderr}}} {
	puts $fd $msg
	set var $msg
	puts $sock [pack_for_transmission var]
}

proc MvthAccept {sock addr port} {
	global mvth_server
	puts "Accept $sock from $addr port $port"
	set mvth_server(addr,$sock) [list $addr $port]
	set mvth_server(req_close,$sock) 0
	fconfigure $sock -buffering line -translation crlf
	interp share {} $sock $mvth_server(slave)
	interp alias $mvth_server(slave) puts {} reply $sock
	interp alias $mvth_server(slave) exit {} req_close $sock
	fileevent $sock readable [list MvthHandleRequest $sock]
}

proc mvth_get_code_string {code} {
	global mvth_rcode
	if {![info exists mvth_rcode($code,id)]} {
		return "$mvth_rcode(S_ERROR,id) $mvth_rcode(S_ERROR,msg)\n"
	} else {
		return "$mvth_rcode($code,id) $mvth_rcode($code,msg)\n"
	}
}

proc MvthHandleRequest {sock} {
	global mvth_server
	global mvth_rcode
	if {[eof $sock] || [catch {gets $sock line}]} {
		# end of file or abnormal connection drop */
		close $sock
		puts "Closed $mvth_server(addr,$sock)"
		unset mvth_server(addr,$sock)
	} else {
		# handle the command that we received 
		# (execute the command in the slave interpreter)
		puts "Command: $line"
		set ans [$mvth_server(slave) eval [list catch $line __result]]
		set ans [$mvth_server(slave) eval {set __result}]
		if {$ans!={}} {reply $sock $ans}
		# end the payload
		puts $sock "";# end of transmission indicator.
		if {$mvth_server(req_close,$sock)} {
			$mvth_server(slave) eval [list close $sock]
			close $sock
			puts "Closed $mvth_server(addr,$sock)"
			unset mvth_server(addr,$sock)
			unset mvth_server(req_close,$sock)
		}
	}
}

proc mvth_loop_chain {} {
#	loop_registered_chain
	after 100 mvth_loop_chain
}

proc register_core_commands {} {
	global mvth_server
	set s $mvth_server(slave) ;# to save typing
	$s eval [list source ls_cmd.tcl]
	$s eval [list source set_cmd.tcl]
	$s eval [list source help_cmd.tcl]
	$s eval [list namespace import ::mvth::*]
}

proc register_c_commands {} {
	global mvth_server
	set s $mvth_server(slave) ;# to save typing
	interp alias $s load {} cload
}

# load all of the C enabled commands
lappend auto_path {/home/sbromley/src/MVTH/mvth--sam-dev-tcl--0.1/=build/lib/mvth}
package require mvthcore

proc mvth_main {} {
	global env
	global mvth_server
	# create a slave interpreter
	set mvth_server(slave) [interp create slave]
	# register the core commands
	register_core_commands
	register_c_commands
	# start waiting for connections
	start_mvth_server 3494
	vwait forever
}

