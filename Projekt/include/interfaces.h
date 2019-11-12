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
#endif

#include "../GUIGlavniProzor.h"
#include <iostream>

/* Ako se koriste prekompilirani headeri (pch) iskopirati wx_pch.h iz template foldera Code::Blocksa u include projekta */

wxDECLARE_EVENT(EVT_POSALJI_PORUKU, wxCommandEvent);

struct PorukaPodaci
{
    enum tip:int{Status, Konzola};
    int t;
    wxString sadrzaj;
};

class IGrafikaGDI
{
    public:
    virtual ~IGrafikaGDI(){};
    virtual bool PoveziSaProzorom(GlavniProzor* prozor)=0;
    virtual void CrtajTest(wxAntialiasMode aa=wxANTIALIAS_DEFAULT)=0;
    virtual void BitmapTest()=0;
    virtual bool Blit(wxPaintEvent& event)=0;

};



#endif // INTERFACES_H
