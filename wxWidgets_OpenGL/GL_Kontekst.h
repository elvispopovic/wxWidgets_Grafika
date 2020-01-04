#ifndef GL_KONTEKST_H
#define GL_KONTEKST_H

#include "Shader.h"
#include "Tekstura.h"
#include "Svjetlo.h"
#include "Kocka.h"


class GL_Kontekst : public wxGLContext
{
    public:
        GL_Kontekst(wxGLCanvas *canvas);
        virtual ~GL_Kontekst();
        bool Inicijaliziraj();
        virtual void PostaviViewport(wxSize velicina);
        virtual void Render(GLfloat kut);

    protected:

    private:
        bool inicijalizirano;
        Shader* shader;
        Kocka *kocka;
        GLuint VertexArrayID[1];
        glm::mat4 View, Projection;
        Svjetlo *glavnoSvjetlo, *dodatnoSvjetlo1, *dodatnoSvjetlo2;

        wxGLCanvas* canvas;
        wxEvtHandler* prozorEvtHandler;

};

#endif // GL_KONTEKST_H
