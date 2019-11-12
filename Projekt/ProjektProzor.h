/***************************************************************
 * Name:      Projekt2Main.h
 * Purpose:   Defines Application Frame
 * Author:    EPC ()
 * Created:   2019-11-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifndef PROJEKT2MAIN_H
#define PROJEKT2MAIN_H

#include "interfaces.h"

class GlavniProzor1 : public GlavniProzor
{
    public:
        GlavniProzor1(wxFrame *frame, IGrafikaGDI* grafika);
        ~GlavniProzor1();
    private:
        IGrafikaGDI* grafika;
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void PanelCrtanje( wxPaintEvent& event );
        virtual void OnAbout(wxCommandEvent& event);
        virtual void obradiPoruku(wxCommandEvent& event);
};

#endif // PROJEKT2MAIN_H
