/***************************************************************
 * Name:      Projekt2Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    EPC ()
 * Created:   2019-11-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "ProjektProzor.h"

wxDEFINE_EVENT(EVT_POSALJI_PORUKU, wxCommandEvent);

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
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
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

/* zbog korištenja UNICODE-a, stranica u kompileru mora biti podešena na UTF-8 */

GlavniProzor1::GlavniProzor1(wxFrame *frame, IGrafikaGDI* grafika)
    : GlavniProzor(frame)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(wxbuildinfo(short_f), 0);
#endif
    this->grafika = grafika;
    panelCrtanje->SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(EVT_POSALJI_PORUKU, &GlavniProzor1::obradiPoruku, this);
}

GlavniProzor1::~GlavniProzor1()
{
    Unbind(EVT_POSALJI_PORUKU, &GlavniProzor1::obradiPoruku, this);

}


void GlavniProzor1::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void GlavniProzor1::PanelCrtanje( wxPaintEvent& event )
{
    grafika->Blit(event);
}

void GlavniProzor1::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void GlavniProzor1::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg,wxT("Dobrodošli!"));

}

void GlavniProzor1::obradiPoruku(wxCommandEvent& event)
{
    PorukaPodaci* pp;
    if((pp = (PorukaPodaci *)(event.GetClientData()))==nullptr)
        return;
    if(pp->t == PorukaPodaci::tip::Konzola)
        konzola -> AppendText(pp->sadrzaj);
    else if(pp->t == PorukaPodaci::tip::Status)
    {
        #if wxUSE_STATUSBAR
        statusBar->SetStatusText(pp->sadrzaj, 0);
        #endif
    }
    delete pp;
}
