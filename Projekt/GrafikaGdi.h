#ifndef GRAFIKAGDI_H
#define GRAFIKAGDI_H

#include "interfaces.h"

/* Klasa GrafikeGDI */
/* GlavniProzor nema sucelje */
class GrafikaGDI : public IGrafikaGDI
{
    public:
        GrafikaGDI();
        ~GrafikaGDI();
        bool PoveziSaProzorom(GlavniProzor* prozor);
        void CrtajTest(wxAntialiasMode aa=wxANTIALIAS_DEFAULT);
        void BitmapTest(unsigned char pomak);
        bool Blit(wxPaintEvent& event);

    protected:
        wxGraphicsRenderer *renderer;
        wxBitmap *bitmap;
        wxMemoryDC *memDC;
        void upisiUKonzolu(wxString sadrzaj);
        void upisiStatus(wxString sadrzaj);

    private:
        wxEvtHandler *prozorEvtHandler;

};

#endif // GRAFIKAGDI_H
