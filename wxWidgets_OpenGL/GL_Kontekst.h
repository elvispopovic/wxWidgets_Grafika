#ifndef GL_KONTEKST_H
#define GL_KONTEKST_H

#include "Shader.h"
#include "Tekstura.h"
#include "Svjetlo.h"
#include "Kocka.h"
#include "Kugla.h"


class GL_Kontekst : public wxGLContext
{
    public:
        GL_Kontekst(wxGLCanvas *canvas);
        virtual ~GL_Kontekst();
        bool Inicijaliziraj();
        virtual void PostaviViewport(wxSize velicina);
        virtual void Render(GLfloat kut, int tip);

    protected:

    private:
        bool inicijalizirano;
        Shader* shader;
        Kocka *kocka;
        Kugla *lopta;

        glm::mat4 View, Projection;
        Tekstura *kocka_tekstura, *kocka_normal_mapa, *kocka_specular_mapa;
        Tekstura *lopta_tekstura, *lopta_normal_mapa, *lopta_specular_mapa;
        Svjetlo *glavnoSvjetlo, *dodatnoSvjetlo1, *dodatnoSvjetlo2;

        wxGLCanvas* canvas;
        wxEvtHandler* prozorEvtHandler;

};

#endif // GL_KONTEKST_H
