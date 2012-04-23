/***************************************************************
 * Name:      shadertoyMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Luke Wren (wren6991@gmail.com)
 * Created:   2012-04-19
 * Copyright: Luke Wren ()
 * License:
 **************************************************************/

#include "shadertoyMain.h"
#include <wx/msgdlg.h>
#include "util.h"

//(*InternalHeaders(shadertoyFrame)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

GLfloat quaddata[32] = {                                  // pos, normal, texcoord
    -1.0, -1.0, -1.0,   0.0, 0.0, 1.0,   0.0, 0.0,
    -1.0,  1.0, -1.0,   0.0, 0.0, 1.0,   0.0, 1.0,
     1.0,  1.0, -1.0,   0.0, 0.0, 1.0,   1.0, 1.0,
     1.0, -1.0, -1.0,   0.0, 0.0, 1.0,   1.0, 0.0
};

GLushort quadindex[6] = {0, 1, 2, 0, 2, 3};

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(shadertoyFrame)
const long shadertoyFrame::ID_GLCANVAS1 = wxNewId();
const long shadertoyFrame::ID_TEXTCTRL1 = wxNewId();
const long shadertoyFrame::ID_TEXTCTRL2 = wxNewId();
const long shadertoyFrame::ID_TEXTCTRL3 = wxNewId();
const long shadertoyFrame::ID_LISTBOX1 = wxNewId();
const long shadertoyFrame::ID_GLCANVAS2 = wxNewId();
const long shadertoyFrame::ID_BUTTON2 = wxNewId();
const long shadertoyFrame::ID_PANEL1 = wxNewId();
const long shadertoyFrame::ID_NOTEBOOK1 = wxNewId();
const long shadertoyFrame::ID_BUTTON1 = wxNewId();
const long shadertoyFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(shadertoyFrame,wxFrame)
    //(*EventTable(shadertoyFrame)
    //*)
END_EVENT_TABLE()

shadertoyFrame::shadertoyFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(shadertoyFrame)
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer3;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    int GLCanvasAttributes_1[] = {
    	WX_GL_RGBA,
    	WX_GL_DOUBLEBUFFER,
    	WX_GL_DEPTH_SIZE,      16,
    	WX_GL_STENCIL_SIZE,    0,
    	0, 0 };
    GLCanvas1 = new wxGLCanvas(this, ID_GLCANVAS1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GLCANVAS1"), GLCanvasAttributes_1);
    GLCanvas1->SetMinSize(wxSize(400,400));
    BoxSizer1->Add(GLCanvas1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
    txtVShader = new wxTextCtrl(Notebook1, ID_TEXTCTRL1, _("#version 110\n\nattribute vec3 pos;\n\nvoid main()\n{\n    gl_Position = gl_ProjectionMatrix * gl_Vertex;\n}"), wxPoint(32,38), wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_MULTILINE|wxVSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    wxFont txtVShaderFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    txtVShader->SetFont(txtVShaderFont);
    txtFShader = new wxTextCtrl(Notebook1, ID_TEXTCTRL2, _("#version 110\n\nuniform vec2 resolution;\nuniform float time;\n\nvoid main()\n{\n    gl_FragColor = vec4(gl_FragCoord.xy / resolution, sin(time), 1.0);\n}"), wxPoint(88,7), wxDefaultSize, wxTE_AUTO_SCROLL|wxTE_PROCESS_TAB|wxTE_MULTILINE|wxVSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    wxFont txtFShaderFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    txtFShader->SetFont(txtFShaderFont);
    txtError = new wxTextCtrl(Notebook1, ID_TEXTCTRL3, wxEmptyString, wxPoint(279,15), wxDefaultSize, wxTE_AUTO_SCROLL|wxTE_PROCESS_TAB|wxTE_MULTILINE|wxVSCROLL, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    wxFont txtErrorFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Consolas"),wxFONTENCODING_DEFAULT);
    txtError->SetFont(txtErrorFont);
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxPoint(259,18), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    lstTextures = new wxListBox(Panel1, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    lstTextures->SetSelection( lstTextures->Append(_("Tex 0")) );
    lstTextures->Append(_("Tex 1"));
    lstTextures->Append(_("Tex 2"));
    lstTextures->Append(_("Tex 3"));
    lstTextures->SetMaxSize(wxSize(20,0));
    BoxSizer3->Add(lstTextures, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    int GLCanvasAttributes_2[] = {
    	WX_GL_RGBA,
    	WX_GL_DOUBLEBUFFER,
    	WX_GL_DEPTH_SIZE,      16,
    	WX_GL_STENCIL_SIZE,    0,
    	0, 0 };
    GLCanvas2 = new wxGLCanvas(Panel1, ID_GLCANVAS2, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GLCANVAS2"), GLCanvasAttributes_2);
    BoxSizer4->Add(GLCanvas2, 1, wxALL|wxSHAPED|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnBrowseTexture = new wxButton(Panel1, ID_BUTTON2, _("Browse"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer4->Add(btnBrowseTexture, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer3->Add(BoxSizer4, 5, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    Panel1->SetSizer(BoxSizer3);
    BoxSizer3->Fit(Panel1);
    BoxSizer3->SetSizeHints(Panel1);
    Notebook1->AddPage(txtVShader, _("Vertex Shader"), false);
    Notebook1->AddPage(txtFShader, _("Fragment Shader"), true);
    Notebook1->AddPage(txtError, _("Errors"), false);
    Notebook1->AddPage(Panel1, _("Textures"), false);
    BoxSizer2->Add(Notebook1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    btnCompile = new wxButton(this, ID_BUTTON1, _("Compile Shaders"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(btnCompile, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    SetSizer(BoxSizer1);
    Timer1.SetOwner(this, ID_TIMER1);
    Timer1.Start(10, false);
    TextureFileDialog = new wxFileDialog(this, _("Select texture file"), wxEmptyString, wxEmptyString, _("TGA file (*.tga)|*.tga"), wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    GLCanvas1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&shadertoyFrame::OnGLCanvas1Paint,0,this);
    GLCanvas1->Connect(wxEVT_SIZE,(wxObjectEventFunction)&shadertoyFrame::OnGLCanvas1Resize,0,this);
    GLCanvas2->Connect(wxEVT_SIZE,(wxObjectEventFunction)&shadertoyFrame::OnGLCanvas2Resize,0,this);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&shadertoyFrame::OnbtnBrowseTextureClick);
    Connect(ID_NOTEBOOK1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&shadertoyFrame::OnNotebook1PageChanged);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&shadertoyFrame::OnbtnCompileClick);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&shadertoyFrame::OnTimer1Trigger);
    //*)

    this->SetTitle("Shader Toy");

    TextureContext = new wxGLContext(GLCanvas2);
    TextureContext->SetCurrent(*GLCanvas2);

    GLContext = new wxGLContext(GLCanvas1);
    GLContext->SetCurrent(*GLCanvas1);

    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        std::cout << "GLEW init failed: \"" << glewGetErrorString(err) << "\". Shutting down...\n";
        char c;
        std::cin >> c;
        this->Close();
        return;
    }
    else
    {
        std::cout << "GLEW OK! OGL version: " << GLEW_VERSION_MAJOR << "." << GLEW_VERSION_MINOR << "\n";
    }


    resources.fshader = makeShader(GL_FRAGMENT_SHADER, txtFShader->GetValue().ToUTF8().data(), txtError);
    resources.vshader = makeShader(GL_VERTEX_SHADER,   txtVShader->GetValue().ToUTF8().data(), txtError);
    resources.program = makeProgram(resources.vshader, resources.fshader);

    resources.resolution = glGetUniformLocation(resources.program, "resolution");
    resources.time = glGetUniformLocation(resources.program, "time");

    resources.textures[0] = makeTexture("data/tex0.tga");
    resources.textures[1] = makeTexture("data/tex1.tga");
    resources.textures[2] = makeTexture("data/tex2.tga");
    resources.textures[3] = makeTexture("data/tex3.tga");

    resources.texloc[0] = glGetUniformLocation(resources.program, "tex0");
    resources.texloc[1] = glGetUniformLocation(resources.program, "tex1");
    resources.texloc[2] = glGetUniformLocation(resources.program, "tex2");
    resources.texloc[3] = glGetUniformLocation(resources.program, "tex3");

    glGenBuffers(1, &resources.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, resources.vbo);
    glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(GLfloat), quaddata, GL_STATIC_DRAW);

    glGenBuffers(1, &resources.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), quadindex, GL_STATIC_DRAW);

    TextureContext->SetCurrent(*GLCanvas2);

    resources.texdialog.textures[0] = makeTexture("data/tex0.tga");
    resources.texdialog.textures[1] = makeTexture("data/tex1.tga");
    resources.texdialog.textures[2] = makeTexture("data/tex2.tga");
    resources.texdialog.textures[3] = makeTexture("data/tex3.tga");

    resolutionx = 400;
    resolutiony = 400;

    texturex = GLCanvas2->GetSize().GetX();
    texturey = GLCanvas2->GetSize().GetY();

    StopWatch1.Start(0);
}

shadertoyFrame::~shadertoyFrame()
{
    //(*Destroy(shadertoyFrame)
    //*)
}

void shadertoyFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void shadertoyFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void shadertoyFrame::OnNotebook1PageChanged(wxNotebookEvent& event)
{
}

void shadertoyFrame::initgl()
{
    GLContext->SetCurrent(*GLCanvas1);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, resolutionx, resolutiony);
}

void shadertoyFrame::endgl()
{
    glFlush();
    GLCanvas1->SwapBuffers();
}

void shadertoyFrame::OnGLCanvas1Paint(wxPaintEvent& event)
{
    initgl();

    glUseProgram(resources.program);

    if (resources.resolution)
        glUniform2f(resources.resolution, resolutionx, resolutiony);
    if (resources.time)
        glUniform1f(resources.time, StopWatch1.Time() * 0.001f);
    for (int i = 0; i < 4; i++)
    {
        if(resources.texloc[i])
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, resources.textures[i]);
            glUniform1i(resources.texloc[i], i);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, resources.vbo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 32, BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, 32, BUFFER_OFFSET(12));
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 32, BUFFER_OFFSET(24));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.ibo);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));


    glUseProgram(0);

    endgl();

    drawtexture();
}

void shadertoyFrame::OnTimer1Trigger(wxTimerEvent& event)
{
    wxPaintEvent evt;
    OnGLCanvas1Paint(evt);
}

void shadertoyFrame::OnbtnCompileClick(wxCommandEvent& event)
{
    glDeleteShader(resources.fshader);
    glDeleteShader(resources.vshader);
    glDeleteProgram(resources.program);

    resources.fshader = makeShader(GL_FRAGMENT_SHADER, txtFShader->GetValue().ToUTF8().data(), txtError);
    resources.vshader = makeShader(GL_VERTEX_SHADER,   txtVShader->GetValue().ToUTF8().data(), txtError);
    resources.program = makeProgram(resources.vshader, resources.fshader);

    resources.texloc[0] = glGetUniformLocation(resources.program, "tex0");
    resources.texloc[1] = glGetUniformLocation(resources.program, "tex1");
    resources.texloc[2] = glGetUniformLocation(resources.program, "tex2");
    resources.texloc[3] = glGetUniformLocation(resources.program, "tex3");

    resources.resolution = glGetUniformLocation(resources.program, "resolution");
    resources.time = glGetUniformLocation(resources.program, "time");

    if (!(resources.fshader && resources.vshader && resources.program))
        Notebook1->ChangeSelection(2);

}

void shadertoyFrame::OnGLCanvas1Resize(wxSizeEvent& event)
{
    resolutionx = event.GetSize().GetX();
    resolutiony = event.GetSize().GetY();
}

void shadertoyFrame::drawtexture()
{

    TextureContext->SetCurrent(*GLCanvas2);

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, texturex, texturey);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, resources.texdialog.textures[lstTextures->GetSelection()]);


    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0,  1.0, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f( 1.0,  1.0, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f( 1.0, -1.0, -1.0);
    glEnd();

    glFlush();
    GLCanvas2->SwapBuffers();

    GLContext->SetCurrent(*GLCanvas1);

}

void shadertoyFrame::OnGLCanvas2Resize(wxSizeEvent& event)
{
    texturex = event.GetSize().GetX();
    texturey = event.GetSize().GetY();
}

void shadertoyFrame::OnbtnBrowseTextureClick(wxCommandEvent& event)
{
    if (TextureFileDialog->ShowModal() == wxID_OK)
    {
        wxString filename = TextureFileDialog->GetPath();
        std::cout << filename << "\n";
        GLContext->SetCurrent(*GLCanvas1);
        resources.textures[lstTextures->GetSelection()] = makeTexture(filename);
        TextureContext->SetCurrent(*GLCanvas2);
        resources.texdialog.textures[lstTextures->GetSelection()] = makeTexture(filename);
    }
}
