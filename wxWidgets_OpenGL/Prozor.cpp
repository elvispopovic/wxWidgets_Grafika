/***************************************************************
 * Name:      OpenGL_iz_nuleMain.cpp
 * Purpose:   Code for Application Frame
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

#include "Prozor.h"

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

wxDEFINE_EVENT(EVT_PORUKA_PROZORU, wxCommandEvent);

Prozor::Prozor(wxFrame *prozor, Model* model)
    : GlavniProzor(prozor)
{
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(wxbuildinfo(long_f), 0);
#endif
    wxString sadrzaj;
    panelEvtHandler = nullptr;
    modelEvtHandler = (wxEvtHandler*) model;
    Bind(EVT_PORUKA_PROZORU, &Prozor::obradiPoruku, this);
    Bind(wxEVT_CHAR_HOOK, &Prozor::OnKeyDown, this);
    SetMinSize(wxSize(480, 480));

    wxGLAttributes atributi;
    atributi.PlatformDefaults().Defaults().EndList();
    bool prihvaceno = wxGLCanvas::IsDisplaySupported(atributi);
    if(prihvaceno)
    {
        sadrzaj << wxT("Prihvaćeni su Open GL atributi za wxGLCanvas.\n");
    }
    else
    {
        atributi.Reset();
        atributi.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();
        prihvaceno = wxGLCanvas::IsDisplaySupported(atributi);
        if(prihvaceno)
            sadrzaj << wxT("Prihvaćeni su osnovni Open GL atributi iz drugog pokušaja za wxGLCanvas.\n");
        else
            sadrzaj << wxT("Nisu prihvaćeni Open GL atributi.\n");
    }
    konzola -> AppendText(sadrzaj);
    if(prihvaceno)
    {
        glPanel = new GL_Panel(this, atributi); // Ne treba ga brisati, briše ga wxWidgets
        panelEvtHandler = (wxEvtHandler*)glPanel;
        sizerCrtanje->Add(glPanel,1, wxEXPAND);
        glPanel->Inicijaliziraj();
    }
}

Prozor::~Prozor()
{
    Unbind(EVT_PORUKA_PROZORU, &Prozor::obradiPoruku, this);
    Unbind(wxEVT_CHAR_HOOK, &Prozor::OnKeyDown, this);
}

GL_Panel* Prozor::DohvatiGLPanel()
{
    return this->glPanel;
}

void Prozor::OnKeyDown(wxKeyEvent& event)
{
    int tipka;
    wxString sadrzaj;
    event.Skip();
    wxCommandEvent* obradaTipki;
    PorukaPaneluPodaci* pp;

    tipka = toupper(event.GetKeyCode());
    switch(tipka)
    {
    case 'W':
    case 'A':
    case 'D':
    case 'S':   pp = new PorukaPaneluPodaci;
                pp->t=PorukaPaneluPodaci::tip::PomakniKameru;
                pp->i=tipka;
                obradaTipki = new wxCommandEvent(EVT_PORUKA_PANELU);
                obradaTipki->SetClientData((void *)pp);
                wxQueueEvent(panelEvtHandler,obradaTipki);
                break;
    default: sadrzaj << "Pritisnuta je tipka broj " << tipka << ".\n";
    }

    konzola->AppendText(sadrzaj);
}

void Prozor::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void Prozor::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void Prozor::OnAbout(wxCommandEvent &event)
{
    wxString build = "OpenGL test - ";
    build << wxbuildinfo(short_f);
    wxString sadrzaj;
    sadrzaj << "OpenGL verzija: " << reinterpret_cast<const char *>(glGetString(GL_VERSION)) << "\n";
    sadrzaj << wxT("Proizvođač: ") << reinterpret_cast<const char *>(glGetString(GL_VENDOR)) << "\n";
    sadrzaj << wxT("Renderer: ") << reinterpret_cast<const char *>(glGetString(GL_RENDERER)) << "\n";
    wxMessageBox(sadrzaj, build);
}

void Prozor::GumbPomakni( wxMouseEvent& event)
{
    wxCommandEvent* osvjezenje;
    event.Skip(); //omoguæava i ostalima da reagiraju na poruku
    if(panelEvtHandler == nullptr)
        return;
    PorukaPaneluPodaci *pp = new PorukaPaneluPodaci;
    pp->t=PorukaPaneluPodaci::tip::Pomicanje;
    osvjezenje = new wxCommandEvent(EVT_PORUKA_PANELU);
    osvjezenje->SetClientData((void *)pp);
    wxQueueEvent(panelEvtHandler,osvjezenje);

}

void Prozor::GumbPokreniZaustavi( wxMouseEvent& event )
{
    wxCommandEvent* pokretanjeZaustavljanje;
    event.Skip(); //omoguæava i ostalima da reagiraju na poruku
    if(modelEvtHandler == nullptr)
        return;
    PorukaModeluPodaci *pp = new PorukaModeluPodaci;
    pp->t=PorukaModeluPodaci::tip::PokreniZaustavi;
    pokretanjeZaustavljanje = new wxCommandEvent(EVT_PORUKA_MODELU);
    pokretanjeZaustavljanje->SetClientData((void *)pp);
    wxQueueEvent(modelEvtHandler,pokretanjeZaustavljanje);
}

void Prozor::PomakKuta( wxSpinDoubleEvent& event )
{
    double dohvaceno;
    wxCommandEvent* pomicanje;
    event.Skip(); //omoguæava i ostalima da reagiraju na poruku
    if(panelEvtHandler == nullptr)
        return;
    PorukaPaneluPodaci *pp = new PorukaPaneluPodaci;
    dohvaceno = brojacKuta->GetValue();
    if(dohvaceno < 0.0)
        dohvaceno = 360.0+dohvaceno;
    else if(dohvaceno>=360.0)
        dohvaceno -=360.0;
    pp->t=PorukaPaneluPodaci::tip::PromijeniKut;
    pp->kut=dohvaceno;
    brojacKuta->SetValue(dohvaceno);
    pomicanje = new wxCommandEvent(EVT_PORUKA_PANELU);
    pomicanje->SetClientData((void *)pp);
    wxQueueEvent(panelEvtHandler,pomicanje);
}

void Prozor::TijeloPromijenjeno( wxCommandEvent& event )
{
    wxCommandEvent* promjena;
    event.Skip();
    if(panelEvtHandler == nullptr)
        return;
    PorukaPaneluPodaci *pp = new PorukaPaneluPodaci;

    wxRadioBox *rb = wxDynamicCast(event.GetEventObject(),wxRadioBox);
    pp->t=PorukaPaneluPodaci::tip::PromijeniTijelo;
    pp->i=rb->GetSelection();
    promjena = new wxCommandEvent(EVT_PORUKA_PANELU);
    promjena->SetClientData((void *)pp);
    wxQueueEvent(panelEvtHandler,promjena);
}

void Prozor::obradiPoruku(wxCommandEvent& event)
{
    PorukaProzoruPodaci* pp;
    if((pp = (PorukaProzoruPodaci *)(event.GetClientData()))==nullptr)
        return;
    if(pp->t == PorukaProzoruPodaci::tip::Konzola)
        konzola -> AppendText(pp->sadrzaj);
    else if(pp->t == PorukaProzoruPodaci::tip::Animacija && panelEvtHandler != nullptr)
    {
        /* prosljedjujemo poruku od modela prema panelu. Model nema direktni kontakt s panelom prozora */
        wxCommandEvent* animacija;
        PorukaPaneluPodaci *pp = new PorukaPaneluPodaci;
        pp->t=PorukaPaneluPodaci::tip::Pomicanje;
        animacija = new wxCommandEvent(EVT_PORUKA_PANELU);
        animacija->SetClientData((void *)pp);
        wxQueueEvent(panelEvtHandler,animacija);
    }
    else if(pp->t == PorukaProzoruPodaci::tip::PokreniZaustavi)
    {
        if(pp->sadrzaj == "pokreni")
        {
            gumbPokreni->SetLabel("Zaustavi");
        }

        else
        {
            gumbPokreni->SetLabel("Pokreni");
        }

    }
    else if(pp->t == PorukaProzoruPodaci::tip::Kut)
    {
        if(pp->kut>=0.0 && pp->kut<=360.0)
        {
            brojacKuta->SetValue(pp->kut);
        }
    }
    else if(pp->t == PorukaProzoruPodaci::tip::Status)
    {
        #if wxUSE_STATUSBAR
        statusBar->SetStatusText(pp->sadrzaj, 0);
        #endif
    }
    delete pp;
}
