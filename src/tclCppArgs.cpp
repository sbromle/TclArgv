/*
 * Provides C++ variadic template for getting arguments from Tcl
 *
 * Copyright (C) 2013 Whitecap Scientific Corporation
 * Original author: Michael Barriault <mike.barriault@whitecapscientific.com>
 */

#include "tclCppArgs.h"
#include <cstring>

int Tcl_GetArgsFromObjs(Tcl_Interp* interp, int i, int objc, Tcl_Obj *CONST objv[]) {
	return TCL_OK;
}

template<typename T>
int Tcl_GetIntLikeFromObj(Tcl_Interp* interp, Tcl_Obj *obj, T* ptr) {
	int val=0;
	int ret = Tcl_GetIntFromObj(interp, obj, &val);
	*ptr = (T)val;
	return ret;
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, char* ptr) {
	const char* str = Tcl_GetString(obj);
	if ( strlen(str) == 1 ) { // Single character, set to ASCII value
		*ptr = str[0];
		return TCL_OK;
	}
	else // Multi-character, must be number
		return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned char* ptr) {
	return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned short* ptr) {
	return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned int* ptr) {
	return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, unsigned long* ptr) {
	return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, signed char* ptr) {
	return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, short* ptr) {
	return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, long* ptr) {
	return Tcl_GetIntLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, int* ptr) {
	return Tcl_GetIntFromObj(interp, obj, ptr);
}

template<typename T>
int Tcl_GetDoubleLikeFromObj(Tcl_Interp* interp, Tcl_Obj *obj, T* ptr) {
	double val=0;
	int ret = Tcl_GetDoubleFromObj(interp, obj, &val);
	*ptr = (T)val;
	return ret;
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, float* ptr) {
	return Tcl_GetDoubleLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, long double* ptr) {
	return Tcl_GetDoubleLikeFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, double* ptr) {
	return Tcl_GetDoubleFromObj(interp, obj, ptr);
}

template<>
int Tcl_GetArgFromObj(Tcl_Interp* interp, Tcl_Obj *obj, char** ptr) {
	*ptr = Tcl_GetString(obj);
	return TCL_OK;
}

