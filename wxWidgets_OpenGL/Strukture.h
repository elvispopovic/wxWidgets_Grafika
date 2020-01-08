#ifndef STRUKTURE_H_INCLUDED
#define STRUKTURE_H_INCLUDED

struct PorukaProzoruPodaci
{
    enum tip:int{Status, Konzola, PokreniZaustavi, Animacija, Kut};
    int t;
    double kut;
    wxString sadrzaj;
    unsigned char pomak;
};

struct PorukaModeluPodaci
{
    enum tip:int{PokreniZaustavi, Test};
    int t;
    wxString sadrzaj;
};


wxDECLARE_EVENT(EVT_PORUKA_PROZORU, wxCommandEvent);

class PorukaPaneluPodaci
{
    public:
        enum tip:int{Parametri, Pomicanje, PokreniZaustavi, PromijeniKut, PromijeniTijelo, Test};
        int t;
        int i;
        double kut;
        wxString tekst;
        static void UpisiUKonzolu(wxEvtHandler* rukovatelj, wxString sadrzaj)
        {
            wxCommandEvent* konzolaEvent;
            if(rukovatelj == nullptr)
                return;
            PorukaProzoruPodaci *pp = new PorukaProzoruPodaci;
            pp->t=PorukaProzoruPodaci::tip::Konzola;
            pp->sadrzaj = sadrzaj;
            konzolaEvent = new wxCommandEvent(EVT_PORUKA_PROZORU);
            konzolaEvent->SetClientData((void *)pp);
            wxQueueEvent(rukovatelj,konzolaEvent);
        }

        static bool ProvjeriGresku(wxEvtHandler* rukovatelj)
        {
            wxString sadrzaj;
            GLenum posljednjaGreska = GL_NO_ERROR;
            for ( ;; )
            {
                GLenum greska = glGetError();
                if(greska == GL_NO_ERROR)
                    return false;
                if ( greska == posljednjaGreska )
                {
                    sadrzaj << wxT("Stanje posljednje greške ne može se resetirati.\n");
                    UpisiUKonzolu(rukovatelj, sadrzaj);
                    return true;
                }
                posljednjaGreska = greska;
                sadrzaj << wxT("Open GL greška: ") << greska << "\n";
                UpisiUKonzolu(rukovatelj, sadrzaj);
            }
            return true;
        }

        static void IspisiMatricu(wxEvtHandler* rukovatelj, glm::mat4 matrica)
        {
            for(int j=0; j<4; j++)
            {
                wxString s;
                for(int i=0; i<4; i++)
                {
                    s << matrica[j][i] << " ";
                }
                s << "\n";
                UpisiUKonzolu(rukovatelj, s);
            }
        }
};

#endif // STRUKTURE_H_INCLUDED
