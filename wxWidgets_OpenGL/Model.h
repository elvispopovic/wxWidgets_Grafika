#ifndef MODEL_H
#define MODEL_H

#include "GUIGlavniProzor.h"

wxDECLARE_EVENT(EVT_PORUKA_MODELU, wxCommandEvent);
wxDECLARE_EVENT(EVT_PORUKA_PANELU, wxCommandEvent);

class Model : public wxFrame
{
    public:
        Model();
        virtual ~Model();
        void PoveziSaProzorom(GlavniProzor* prozor);
        void Pokreni(int milisekundi);
        void Zaustavi();

        void Animiraj();
    protected:
        void obradiPoruku(wxCommandEvent& event);
    private:
        wxEvtHandler *prozorEvtHandler;
        bool pokrenuto;
        std::promise<void>* dretveniPromise;
        std::thread* animacijskaDretva;
        void radna(int id, std::shared_future<void> futur, int milisekundi);
        void AzurirajGumb(wxString sadrzaj);
};

#endif // MODEL_H
