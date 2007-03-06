#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>

int load_plugin(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[])
{
	Tcl_Obj *resultPtr;
	char string[]="this is the c plugin loader.";
	resultPtr=Tcl_GetObjResult(interp);
	Tcl_SetStringObj(resultPtr,string,sizeof(string));
	return TCL_OK;
}

int Mvthcore_Init(Tcl_Interp *interp) {
	/* Initialize the stub table interface. */
	if (Tcl_InitStubs(interp,"8.1",0)==NULL) {
		return TCL_ERROR;
	}
	Tcl_CreateObjCommand(interp,"cload",load_plugin,
			(ClientData)NULL,(Tcl_CmdDeleteProc *)NULL);
	/* Declare that we provide the Mvthcore package */
	Tcl_PkgProvide(interp,"mvthcore","1.0");
	return TCL_OK;
}
