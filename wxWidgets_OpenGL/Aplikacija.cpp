/***************************************************************
 * Name:      OpenGL_iz_nuleApp.cpp
 * Purpose:   Code for Application Class
 * Author:    EPC ()
 * Created:   2019-12-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "Aplikacija.h"
#include "Prozor.h"
#include "GL_Panel.h"

wxDECLARE_EVENT(EVT_PORUKA_PROZORU, wxCommandEvent);

IMPLEMENT_APP(Aplikacija);

bool Aplikacija::OnInit()
{
    wxString sadrzaj;
    model = new Model();
    Prozor* prozor = new Prozor(0L, model);
    prozor->SetIcon(wxICON(aaaa)); // To Set App Icon
    prozorEvtHandler = (wxEvtHandler*)prozor;
    prozor->Show();

    model->PoveziSaProzorom(prozor);


    return true;
}

int Aplikacija::OnExit()
{
    delete model;
    return wxApp::OnExit();
}

