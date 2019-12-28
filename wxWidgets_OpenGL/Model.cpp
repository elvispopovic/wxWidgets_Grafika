#include "Model.h"
#include "Prozor.h"

wxDEFINE_EVENT(EVT_PORUKA_MODELU, wxCommandEvent);

Model::Model() : pokrenuto(false)
{
    prozorEvtHandler = nullptr;
    Bind(EVT_PORUKA_MODELU, &Model::obradiPoruku, this);
}

Model::~Model()
{
    if(pokrenuto)
        Zaustavi();
    Unbind(EVT_PORUKA_MODELU, &Model::obradiPoruku, this);
}

void Model::PoveziSaProzorom(GlavniProzor* prozor)
{
    /* omogucava slanje poruke prozoru */
    prozorEvtHandler = (wxEvtHandler*)prozor;
}

void Model::obradiPoruku(wxCommandEvent& event)
{
    PorukaModeluPodaci* pp;
    if((pp = (PorukaModeluPodaci *)(event.GetClientData()))==nullptr)
        return;
    if (pp->t == PorukaModeluPodaci::tip::PokreniZaustavi)
    {
        if(!pokrenuto)
            Pokreni(33);
        else
            Zaustavi();
    }
    delete pp;
}

void Model::Pokreni(int milisekundi)
{
    if(pokrenuto)
        return;
    dretveniPromise = new std::promise<void>;
    std::shared_future<void> futur = dretveniPromise->get_future();
    /* lokalne varijable se moraju proslijediti, a atributi klase i funkcije se dohvacaju sa this */
    animacijskaDretva = new std::thread(&Model::radna, this, 1, futur, milisekundi);
    AzurirajGumb("pokreni");
    pokrenuto = true;
}

void Model::Zaustavi()
{
    if(!pokrenuto)
        return;
    /* postavljanje promisa, sve blokirano futurom se otključava */
    dretveniPromise->set_value();
    animacijskaDretva->join();
    delete animacijskaDretva;
    delete dretveniPromise;
    AzurirajGumb("zaustavi");
    pokrenuto = false;
}

void Model::AzurirajGumb(wxString sadrzaj)
{
    wxCommandEvent* pokretanjeZaustavljanje;
    if(prozorEvtHandler == nullptr)
        return;
    PorukaProzoruPodaci *pp = new PorukaProzoruPodaci;
    pp->t=PorukaProzoruPodaci::tip::PokreniZaustavi;
    pp->sadrzaj = sadrzaj;
    pokretanjeZaustavljanje = new wxCommandEvent(EVT_PORUKA_PROZORU);
    pokretanjeZaustavljanje->SetClientData((void *)pp);
    wxQueueEvent(prozorEvtHandler,pokretanjeZaustavljanje);

}

void Model::Animiraj()
{
    wxCommandEvent* animacija;
    if(prozorEvtHandler == nullptr)
        return;
    PorukaProzoruPodaci *pp = new PorukaProzoruPodaci;
    pp->t=PorukaProzoruPodaci::tip::Animacija;
    animacija = new wxCommandEvent(EVT_PORUKA_PROZORU);
    animacija->SetClientData((void *)pp);
    wxQueueEvent(prozorEvtHandler,animacija);
}

void Model::radna(int id, std::shared_future<void> futur, int milisekundi)
{
    wxString sadrzaj;
    sadrzaj << "Pokrenuta je animacijska dretva ID: "<< id <<"\n";
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    while (futur.wait_for(std::chrono::milliseconds(milisekundi)) == std::future_status::timeout)
    {
        Animiraj();
    }
    sadrzaj.clear();
    sadrzaj << wxT("Završava animacijska dretva ID: ")<< id <<".\n";
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);

}
