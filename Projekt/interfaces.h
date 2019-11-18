#ifndef INTERFACES_H
#define INTERFACES_H

#ifndef WX_PRECOMP
    #include <wx/app.h>
    #include <wx/wx.h>
    #include <wx/graphics.h>
    #include <wx/dcclient.h>
    #include <wx/dcgraph.h>
    #include <wx/dcbuffer.h>
    #include <wx/rawbmp.h>
    #include <iostream>
    #include <thread>
    #include <future>
#endif

#include "GUIGlavniProzor.h"


/* Ako se koriste prekompilirani headeri (pch) iskopirati wx_pch.h iz template foldera Code::Blocksa u include projekta */

wxDECLARE_EVENT(EVT_PORUKA_PROZORU, wxCommandEvent);

struct PorukaPodaci
{
    enum tip:int{Status, Konzola, Animacija};
    int t;
    wxString sadrzaj;
    unsigned char pomak;
};

class IGrafikaGDI
{
    public:
    virtual ~IGrafikaGDI(){};
    virtual bool PoveziSaProzorom(GlavniProzor* prozor)=0;
    virtual void CrtajTest(wxAntialiasMode aa=wxANTIALIAS_DEFAULT)=0;
    virtual void BitmapTest(unsigned char pomak)=0;
    virtual bool Blit(wxPaintEvent& event)=0;

};

class IModel
{
    public:
    virtual ~IModel(){};
    virtual void PoveziSaProzorom(GlavniProzor* prozor)=0;
    virtual void Pokreni()=0;
    virtual void Zaustavi()=0;
    virtual void upisiUKonzolu(wxString sadrzaj)=0;
    virtual void pomakniAnimaciju(unsigned char pomak)=0;
    virtual void radna(int id, std::shared_future<void> futur)=0;
};


#endif // INTERFACES_H
