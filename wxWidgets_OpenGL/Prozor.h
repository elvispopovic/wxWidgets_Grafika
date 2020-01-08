/***************************************************************
 * Name:      OpenGL_iz_nuleMain.h
 * Purpose:   Defines Application Frame
 * Author:    EPC ()
 * Created:   2019-12-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifndef OPENGL_IZ_NULEMAIN_H
#define OPENGL_IZ_NULEMAIN_H



#include "Aplikacija.h"
#include "GL_Panel.h"
#include "Model.h"


#include "GUIGlavniProzor.h"

wxDECLARE_EVENT(EVT_PORUKA_PROZORU, wxCommandEvent);
wxDECLARE_EVENT(EVT_PORUKA_PANELU, wxCommandEvent);
wxDECLARE_EVENT(EVT_PORUKA_MODELU, wxCommandEvent);

class Prozor: public GlavniProzor
{
    public:
        Prozor(wxFrame *prozor, Model* model);
        ~Prozor();
        GL_Panel* DohvatiGLPanel();
    protected:
        void GumbPomakni( wxMouseEvent& event);
        void GumbPokreniZaustavi( wxMouseEvent& event );
        void PomakKuta( wxSpinDoubleEvent& event );
        void TijeloPromijenjeno( wxCommandEvent& event );
        void obradiPoruku(wxCommandEvent& event);
    private:
        GL_Panel* glPanel;
        wxEvtHandler *panelEvtHandler;
        wxEvtHandler *modelEvtHandler;
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);

};

#endif // OPENGL_IZ_NULEMAIN_H
