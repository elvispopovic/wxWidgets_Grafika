#ifndef GL_KONTEKST_H
#define GL_KONTEKST_H

#include "Shader.h"
#include "Tekstura.h"
#include "Svjetlo.h"


class GL_Kontekst : public wxGLContext
{
    public:
        GL_Kontekst(wxGLCanvas *canvas);
        virtual ~GL_Kontekst();
        bool Inicijaliziraj();
        virtual void PostaviViewport(wxSize velicina);
        virtual void Render(wxSize velicina, GLfloat kut);

    protected:

    private:
        static unsigned short vrhovi_stranica_podaci[6*4];
        static GLfloat vrhovi_podaci[14*3];
        static GLfloat uv_podaci[14*2];
        bool inicijalizirano;
        Shader* shader;
        GLuint VertexArrayID[1];
        GLuint vertexbuffer, uvbuffer, normbuffer, tangentbuffer, bitangentbuffer, elementbuffer;
        GLuint MVP_ID, V_ID, M_ID, MV3x3_ID;
        glm::mat4 View, Projection;

        Tekstura *tekstura, *normal_mapa, *specular_mapa;

        Svjetlo *glavnoSvjetlo, *dodatnoSvjetlo1, *dodatnoSvjetlo2;

        GLfloat *vrhovi, *uvovi, *normale, *tangente, *bitangente;
        unsigned short *indeksi;

        wxGLCanvas* canvas;
        wxEvtHandler* prozorEvtHandler;
        void izracunajVrhove();

};

#endif // GL_KONTEKST_H
