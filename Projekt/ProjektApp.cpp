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
#include "GrafikaGdi.h"
#include "model.h"

IMPLEMENT_APP(ProjektApp);

bool ProjektApp::OnInit()
{
    grafika = new GrafikaGDI();
    model = new Model(4);
    prozor = new GlavniProzor1(0L, grafika, model);
    prozor->SetIcon(wxICON(aaaa)); // To Set App Icon
    prozor->Show();

    grafika->PoveziSaProzorom(prozor);
    grafika->BitmapTest(0);
    model-> PoveziSaProzorom(prozor);
    //model -> Pokreni();
    return true;
}


int ProjektApp::OnExit()
{
    if(model != nullptr)
        delete model;
    if(grafika != nullptr)
        delete grafika;
    /* prozor se ne briše. Njega uništava destroy prilikom poziva delete ili quit i ne smije se ovdje staviti delete */
    return 0;
}
