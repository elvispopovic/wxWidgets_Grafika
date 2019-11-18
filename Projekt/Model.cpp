#include "Model.h"

Model::Model(int brojDretvi) : brojDretvi(brojDretvi),pokrenuto(false)
{
    if(brojDretvi > 0)
        dretve = new std::thread*[brojDretvi];
}

Model::~Model()
{
    wxString sadrzaj;
    if(pokrenuto)
    {
        sadrzaj << "Zaustavljam pokrenute dretve...\n";
        upisiUKonzolu(sadrzaj);
        Zaustavi();
    }
    if(brojDretvi > 0)
        delete[] dretve;
}

void Model::PoveziSaProzorom(GlavniProzor* prozor)
{
    this->prozor = prozor;
    prozorEvtHandler = (wxEvtHandler*)prozor;
}

/* radna metoda za dretvu */
/* za više dretvi future object mora biti shared */

void Model::radna(int id, std::shared_future<void> futur)
{
    wxString sadrzaj;
    sadrzaj << "Pokrenuta je dretva "<< id <<"\n";
    this->upisiUKonzolu(sadrzaj);

    /* zahtijev za futurom blokira dretvu dok se ne postavi promis. Ovdje je wait koji se sam odblokira nakon 1000 ms */
    /* ako se sam odblokirao, petlja se nastavlja. Ako je odblokiran promisom, petlja staje. */
    while (futur.wait_for(std::chrono::milliseconds(2000)) == std::future_status::timeout)
    {
        pomakniAnimaciju(1);
        sadrzaj.clear();
        sadrzaj << "Dretva "<< id <<" radi...\n";
        this->upisiUKonzolu(sadrzaj);
    }
    sadrzaj.clear();
    sadrzaj << wxT("Završava dretva ")<< id <<"\n";
    this->upisiUKonzolu(sadrzaj);
}

/* pokretanje dretvi */
/* ako se prosljeđuje kakva referenca u dretvu, treba ju proslijediti kao std::ref(v) */
void Model::Pokreni()
{
    if(pokrenuto)
        return;
    upisiUKonzolu(wxT("Pokrećem sve dretve...\n"));
    /* par promise futur - kada se postavi promise, odblokira se svatko tko je tražio vrijednost futura */
    dretveniPromise = new std::promise<void>;
    std::shared_future<void> futur = dretveniPromise->get_future();
    /* lokalne varijable se moraju proslijediti, a atributi klase i funkcije se dohvacaju sa this */
    for (int i=0; i<brojDretvi; i++)
        dretve[i] = new std::thread(&Model::radna, this, i, futur);
    pokrenuto = true;
}

void Model::Zaustavi()
{
    if(!pokrenuto)
        return;
    upisiUKonzolu(wxT("Zaustavljam sve dretve...\n"));
    /* postavljanje promisa, sve blokirano futurom se otključava */
    dretveniPromise->set_value();
    for(int i=0; i<brojDretvi; i++)
    {
        dretve[i]->join();
        delete dretve[i];
    }
    delete dretveniPromise;
    pokrenuto = false;
}

void Model::upisiUKonzolu(wxString sadrzaj)
{
    wxCommandEvent* konzolaEvent;
    PorukaPodaci *pp = new PorukaPodaci;
    pp->t=PorukaPodaci::tip::Konzola;
    pp->sadrzaj = sadrzaj;
    konzolaEvent = new wxCommandEvent(EVT_PORUKA_PROZORU);
    konzolaEvent->SetClientData((void *)pp);
    wxQueueEvent(prozorEvtHandler,konzolaEvent);
}

void Model::pomakniAnimaciju(unsigned char pomak)
{
    wxCommandEvent* animacijaEvent;
    PorukaPodaci *pp = new PorukaPodaci;
    pp->t=PorukaPodaci::tip::Animacija;
    pp->pomak = pomak;
    animacijaEvent = new wxCommandEvent(EVT_PORUKA_PROZORU);
    animacijaEvent->SetClientData((void *)pp);
    wxQueueEvent(prozorEvtHandler,animacijaEvent);
}
