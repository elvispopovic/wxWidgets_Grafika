#ifndef MODEL_H
#define MODEL_H

#include "interfaces.h"
#include "ProjektProzor.h"

/* Klasa modela */
/* GlavniProzor nema sucelje */
class Model : public IModel
{
    public:
        Model(int brojDretvi);
        ~Model();
        void PoveziSaProzorom(GlavniProzor* prozor);
        void Pokreni();
        void Zaustavi();

    protected:
        void upisiUKonzolu(wxString sadrzaj);
        void pomakniAnimaciju(unsigned char pomak);

    private:
        int brojDretvi;
        bool pokrenuto;

        std::promise<void>* dretveniPromise;
        std::thread** dretve;

        GlavniProzor* prozor;
        wxEvtHandler *prozorEvtHandler;
        void radna(int id, std::shared_future<void> futur);
};

#endif // MODEL_H
