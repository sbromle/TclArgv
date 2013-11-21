/*
 * Provides C++ variadic template for getting arguments from Tcl
 * Declare the variables, and call
 * Tcl_GetArgsFromObjs(interp, objc, objv, &obj1, &obj2, &obj3, ...etc)
 * Write template specializations to Tcl_GetArgFromObj to support custom types.
 *
 * Copyright (C) 2013 Whitecap Scientific Corporation
 * Original author: Michael Barriault <mike.barriault@whitecapscientific.com>
 */

#ifndef CPP_ARGS_H
#define CPP_ARGS_H

#include <tcl.h>
#include <typeinfo>

template<typename T>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, T* ptr) {
	static_assert(sizeof(T*) == 0, "Type not implemented.");
	return TCL_ERROR;
}

// GetArgFromObj predefined types
//
// NOTE ON char USAGE
// char is often used for both 8-bit numbers and ASCII characters.
// Sending a string of length one is treated as an ASCII character.
// Sending a string longer than one character is treated as a number.
// To pass single-digit numbers, prefix with 0.
//
// Define your own template specialization to add new types. Return should be TCL_OK
// or TCL_ERROR depending on success.
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, void* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, char* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned char* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, signed char* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned short* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, short* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned int* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, int* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned long* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, long* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, float* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, double* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, long double* ptr);
template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, char** ptr);

int Tcl_GetArgsFromObjs(Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[]);

template<typename T, typename... Args>
int Tcl_GetArgsFromObjs(Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[], T* ptr, Args... args) {
	if ( objc <= 0 ) {
		Tcl_AppendResult(interp, "Not enough arguments.", NULL);
		return TCL_ERROR;
	}
	else if ( ptr != NULL && Tcl_GetArgFromObj(interp, *objv, ptr) != TCL_OK ) {
		Tcl_AppendResult(interp, "Couldn't interpret argment ", Tcl_GetString(Tcl_NewIntObj(objc)), NULL);
		return TCL_ERROR;
	}
	else {
		// This bit here is to prevent objv pointer from being incremented too far
		--objc;
		if ( objc > 0 )
			++objv;
		else // We're at the end of known objects anyway, increment i one extra time in case there are more args
			--objc;
		return Tcl_GetArgsFromObjs(interp, objc, objv, args...);
	}
}

#endif
