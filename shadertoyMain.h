/***************************************************************
 * Name:      shadertoyMain.h
 * Purpose:   Defines Application Frame
 * Author:    Luke Wren (wren6991@gmail.com)
 * Created:   2012-04-19
 * Copyright: Luke Wren ()
 * License:
 **************************************************************/

#ifndef SHADERTOYMAIN_H
#define SHADERTOYMAIN_H

#include <GL/glew.h>

//(*Headers(shadertoyFrame)
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/glcanvas.h>
#include <wx/stopwatch.h>
#include <wx/panel.h>
#include <wx/filedlg.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/timer.h>
//*)

#include "bsm.h"

class shadertoyFrame: public wxFrame
{
    public:

        shadertoyFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~shadertoyFrame();

        struct
        {
            GLuint vshader;
            GLuint fshader;
            GLuint program;
            GLuint time;
            GLuint resolution;
            GLuint textures[4];
            GLuint texloc[4];

            struct
            {
                GLuint textures[4];
            } texdialog;

            GLuint vbo;
            GLuint ibo;

        } resources;


        int resolutionx;
        int resolutiony;

        int texturex, texturey;
        void drawtexture();

        int ntriangles;

    private:

        //(*Handlers(shadertoyFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnNotebook1PageChanged(wxNotebookEvent& event);
        void OnGLCanvas1Paint(wxPaintEvent& event);
        void OnTimer1Trigger(wxTimerEvent& event);
        void OnbtnCompileClick(wxCommandEvent& event);
        void OnGLCanvas1Resize(wxSizeEvent& event);
        void OnGLCanvas2Resize(wxSizeEvent& event);
        void OnbtnBrowseTextureClick(wxCommandEvent& event);
        void OnButton1Click(wxCommandEvent& event);
        void OnbtnLoadModelClick(wxCommandEvent& event);
        //*)

        void initgl();
        void endgl();

        //(*Identifiers(shadertoyFrame)
        static const long ID_GLCANVAS1;
        static const long ID_TEXTCTRL1;
        static const long ID_TEXTCTRL2;
        static const long ID_TEXTCTRL3;
        static const long ID_LISTBOX1;
        static const long ID_GLCANVAS2;
        static const long ID_BUTTON2;
        static const long ID_PANEL1;
        static const long ID_BUTTON3;
        static const long ID_NOTEBOOK1;
        static const long ID_BUTTON1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(shadertoyFrame)
        wxTextCtrl* txtVShader;
        wxButton* btnCompile;
        wxNotebook* Notebook1;
        wxGLCanvas* GLCanvas1;
        wxButton* btnBrowseTexture;
        wxPanel* Panel1;
        wxFileDialog* ModelFileDialog;
        wxGLCanvas* GLCanvas2;
        wxListBox* lstTextures;
        wxStopWatch StopWatch1;
        wxTextCtrl* txtError;
        wxButton* btnLoadModel;
        wxTextCtrl* txtFShader;
        wxFileDialog* TextureFileDialog;
        wxTimer Timer1;
        //*)

        wxGLContext *GLContext;
        wxGLContext *TextureContext;

        DECLARE_EVENT_TABLE()
};

#endif // SHADERTOYMAIN_H
