#ifndef GRAFIKAGDI_H
#define GRAFIKAGDI_H

#include "interfaces.h"

class GrafikaGDI : public IGrafikaGDI
{
    public:
        GrafikaGDI();
        ~GrafikaGDI();
        bool PoveziSaProzorom(GlavniProzor* prozor);
        void CrtajTest(wxAntialiasMode aa=wxANTIALIAS_DEFAULT);
        void BitmapTest();
        bool Blit(wxPaintEvent& event);

    protected:
        wxGraphicsRenderer *renderer;
        wxBitmap *bitmap;
        wxMemoryDC *memDC;
        void upisiUKonzolu(wxString sadrzaj);
        void upisiStatus(wxString sadrzaj);

    private:
        GlavniProzor* prozor;
        wxEvtHandler *prozorEvtHandler;

};

#endif // GRAFIKAGDI_H
