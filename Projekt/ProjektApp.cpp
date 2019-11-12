/***************************************************************
 * Name:      Projekt2App.cpp
 * Purpose:   Code for Application Class
 * Author:    EPC ()
 * Created:   2019-11-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "ProjektApp.h"

IMPLEMENT_APP(ProjektApp);

bool ProjektApp::OnInit()
{
    grafika = new GrafikaGDI();
    GlavniProzor1* prozor = new GlavniProzor1(0L, grafika);
    prozor->SetIcon(wxICON(aaaa)); // To Set App Icon
    prozor->Show();
    grafika->PoveziSaProzorom(prozor);

    grafika->BitmapTest();

    return true;
}

int ProjektApp::OnExit()
{
    delete grafika;
    return 0;
}
