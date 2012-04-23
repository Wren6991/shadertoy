/***************************************************************
 * Name:      shadertoyApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Luke Wren (wren6991@gmail.com)
 * Created:   2012-04-19
 * Copyright: Luke Wren ()
 * License:
 **************************************************************/

#include "shadertoyApp.h"

//(*AppHeaders
#include "shadertoyMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(shadertoyApp);

bool shadertoyApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	shadertoyFrame* Frame = new shadertoyFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
