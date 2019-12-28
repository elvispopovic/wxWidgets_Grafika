#include "GL_Panel.h"

wxDEFINE_EVENT(EVT_PORUKA_PANELU, wxCommandEvent);

GL_Panel::GL_Panel(wxWindow *parent, const wxGLAttributes& atributi) : wxGLCanvas(parent, atributi)
{
    inicijalizirano = false;
    oglKontekst = nullptr;
    prozorEvtHandler = (wxEvtHandler*)parent;
    Bind(wxEVT_PAINT, wxPaintEventHandler( GL_Panel::Osvjezi ),this);
    Bind(wxEVT_SIZE, wxSizeEventHandler(GL_Panel::PromijeniVelicinu),this);
    Bind(EVT_PORUKA_PANELU, &GL_Panel::obradiPoruku, this);
    kut = glm::radians(45.0f);
}

GL_Panel::~GL_Panel()
{
    Unbind(wxEVT_SIZE, wxSizeEventHandler(GL_Panel::PromijeniVelicinu),this);
    Unbind(wxEVT_PAINT, wxPaintEventHandler( GL_Panel::Osvjezi ),this);
    if(oglKontekst!=nullptr)
        delete oglKontekst;
}

bool GL_Panel::Inicijaliziraj()
{
    static const int attribs[3] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0};
    wxString sadrzaj;
    GLenum error;
    if (inicijalizirano == true)
        return true;
    inicijalizirano = false;
    oglKontekst = new GL_Kontekst(this);
    if ( !oglKontekst->IsOK() )
    {
        sadrzaj << wxT("Open GL kontekst za verziju se ne može inicijalizirati.\n");
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
        delete oglKontekst;
        return false;
    }

    sadrzaj.clear();
    sadrzaj << "Kreiran je OpenGL kontekst\n";
    oglKontekst->SetCurrent(*this);


    if(oglKontekst->Inicijaliziraj()==false)
    {
        delete oglKontekst;
        oglKontekst = nullptr;
        return false;
    }

    if(wxGLCanvas::IsDisplaySupported(attribs))
       sadrzaj << wxT("Podržan je double buffer.\n");
    else
        sadrzaj << wxT("Nije podržan double buffer.\n");
    IspisiInfo();
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    inicijalizirano = true;
    return true;
}

void GL_Panel::Osvjezi(wxPaintEvent& WXUNUSED(event))
{
    wxString sadrzaj;
    wxPaintDC dc(this);
    if(!inicijalizirano || oglKontekst==nullptr)
        return;
    wxSize velicina = GetClientSize();
    oglKontekst->SetCurrent(*this);
    glViewport(0, 0, velicina.x, velicina.y);
    oglKontekst->Render(velicina, kut);
    SwapBuffers();
}

void GL_Panel::Animiraj()
{
    PosaljiKut();
    Refresh(false);
    kut+=glm::radians(1.0f);
}

void GL_Panel::PromijeniVelicinu(wxSizeEvent& event)
{
    wxString sadrzaj;
    if(!inicijalizirano || oglKontekst==nullptr || !IsShownOnScreen())
        return;
    wxSize velicina = GetClientSize();
    //sadrzaj << "Promijenjena velicina, x: " << velicina.x << ", y: " << velicina.y << "\n";
    oglKontekst->SetCurrent(*this);
    oglKontekst->PostaviViewport(velicina);
    //upisiUKonzolu(sadrzaj);
    Refresh(false);
    PosaljiKut();
}

wxEvtHandler* GL_Panel::DohvatiRukovateljProzora()
{
    return prozorEvtHandler;
}


void GL_Panel::obradiPoruku(wxCommandEvent& event)
{
    PorukaPaneluPodaci* pp;
    if((pp = (PorukaPaneluPodaci *)(event.GetClientData()))==nullptr)
        return;
    if(pp->t == PorukaPaneluPodaci::tip::Test)
    {
        wxString sadrzaj(pp->tekst);
        sadrzaj << "O.K.\n";
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    }
    else if(pp->t == PorukaPaneluPodaci::tip::Pomicanje)
    {
        Animiraj();
        PosaljiKut();
    }
    else if(pp->t == PorukaPaneluPodaci::tip::PromijeniKut)
    {
        kut=glm::radians(pp->kut);
        Animiraj();
        kut-=glm::radians(1.0f);
    }
    delete pp;
}
void GL_Panel::PosaljiKut()
{
    wxCommandEvent* promjenaKuta;
    if(glm::degrees(kut)>360.0f)
        kut-=glm::radians(360.0f);
    if(prozorEvtHandler != nullptr)
    {
        PorukaProzoruPodaci *pp = new PorukaProzoruPodaci;
        pp->t=PorukaProzoruPodaci::tip::Kut;
        pp->kut=glm::degrees(kut);
        promjenaKuta = new wxCommandEvent(EVT_PORUKA_PROZORU);
        promjenaKuta->SetClientData((void *)pp);
        wxQueueEvent(prozorEvtHandler,promjenaKuta);
    }
}

/* radi samo u tijeku osvjezavanja */
bool GL_Panel::IspisiInfo()
{
    wxString sadrzaj;
    if (inicijalizirano == false || oglKontekst == nullptr)
        return false;
    oglKontekst->SetCurrent(*this);
    sadrzaj << "OpenGL verzija: " << reinterpret_cast<const char *>(glGetString(GL_VERSION)) << "\n";
    sadrzaj << wxT("Proizvođač: ") << reinterpret_cast<const char *>(glGetString(GL_VENDOR)) << "\n";
    sadrzaj << wxT("Renderer: ") << reinterpret_cast<const char *>(glGetString(GL_RENDERER)) << "\n";
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    return true;
}

