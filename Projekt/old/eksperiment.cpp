#include "dretva.h"

Dretva::Dretva(GlavniProzor* prozor)
{
    this->prozor = prozor;
    prozorEvtHandler = (wxEvtHandler*)prozor;
    dretva = nullptr;
}

Dretva::~Dretva()
{
    exitSignal -> set_value();
    if(dretva)
    {
        dretva -> join();
        delete dretva;
    }
    delete exitSignal;
}

void Dretva::radna()
{
    wxString upis;
    upis << "Pokrenuta je dretva " << id << "\n";
    upisiUKonzolu(upis);
    while (futureObj.wait_for(std::chrono::milliseconds(1000)) == std::future_status::timeout)
    {
        upis.clear();
        upis << "Dretva " << id << " radi...\n";
        upisiUKonzolu(upis);
    }
    upis.clear();
    upis << "Dretva " << id << " završava\n";
    upisiUKonzolu(upis);
}

bool Dretva::Pokreni(int id)
{
    wxString upis;
    if(dretva)
        return false;
    exitSignal = new std::promise<void>;
    futureObj = exitSignal -> get_future();
    this->id = id;
    upis << "Pokreæe se dretva " << id << "\n";
    upisiUKonzolu(upis);
    dretva = new std::thread([=] { radna(); });
    return true;
}

bool Dretva::Zaustavi()
{
    wxString upis;
    if(dretva == nullptr)
        return false;
    exitSignal -> set_value();
    dretva -> join();
    upis << "Dretva " << id << " je završila.\n";
    upisiUKonzolu(upis);
    delete dretva;
    delete exitSignal;
    dretva = nullptr;
    return true;
}

void Dretva::upisiUKonzolu(wxString sadrzaj)
{
    wxCommandEvent* konzolaEvent;
    if(prozorEvtHandler == nullptr)
        return;
    PorukaPodaci *pp = new PorukaPodaci;
    pp->t=PorukaPodaci::tip::Konzola;

    pp->sadrzaj = sadrzaj;
    konzolaEvent = new wxCommandEvent(EVT_POSALJI_PORUKU);
    konzolaEvent->SetClientData((void *)pp);
    wxQueueEvent(prozorEvtHandler,konzolaEvent);
}

