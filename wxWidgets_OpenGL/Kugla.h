#ifndef KUGLA_H
#define KUGLA_H

#include "Shader.h"
#include "Svjetlo.h"
#include "Tekstura.h"

class Kugla
{
    public:
        Kugla(wxGLCanvas *canvas, int podjela);
        virtual ~Kugla();
        bool Inicijaliziraj(Shader* shader);
        void Render(glm::mat4 View, glm::mat4 Projection, GLfloat kut, glm::vec3 pomak, glm::vec3 skaliranje);

    protected:

    private:
        wxGLCanvas* canvas;
        bool inicijalizirano;
        int podjela;
        GLuint VertexArrayID[1];
        GLfloat *vrhovi, *UVovi;
        GLfloat *normale, *tangente, *bitangente;
        GLushort *tstrip, *tfan[2];

        GLuint MVP_ID, V_ID, M_ID, MV3x3_ID;
        GLuint vertexbuffer, uvbuffer, normbuffer, tangentbuffer, bitangentbuffer, indexbuffer[3];
        void kreirajGeometriju();
};

#endif // KUGLA_H
