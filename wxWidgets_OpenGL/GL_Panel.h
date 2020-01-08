#ifndef GL_PANEL_H
#define GL_PANEL_H

#include "GL_Kontekst.h"

wxDECLARE_EVENT(EVT_PORUKA_PROZORU, wxCommandEvent);
wxDECLARE_EVENT(EVT_PORUKA_PANELU, wxCommandEvent);

class GL_Panel : public wxGLCanvas
{
    public:
        GL_Panel(wxWindow *parent, const wxGLAttributes& atributi);
        virtual ~GL_Panel();
        bool Inicijaliziraj();
        void Osvjezi(wxPaintEvent& event);
        void Animiraj();
        void PromijeniVelicinu(wxSizeEvent& event);
        wxEvtHandler* DohvatiRukovateljProzora();
        bool IspisiInfo();
    protected:
        void obradiPoruku(wxCommandEvent& event);
    private:
        wxEvtHandler *prozorEvtHandler;
        GL_Kontekst*  oglKontekst;
        bool inicijalizirano;
        GLfloat kut;
        int tijelo;
        void PosaljiKut();

};

#endif // GL_PANEL_H
