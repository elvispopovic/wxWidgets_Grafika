#ifndef KOCKA_H
#define KOCKA_H

#include "Shader.h"
#include "Svjetlo.h"
#include "Tekstura.h"

class Kocka
{
    public:
        Kocka();
        virtual ~Kocka();
        bool Inicijaliziraj(wxGLCanvas *canvas, GLuint VertexArrayID, Shader* shader);
        void Render(glm::mat4 View, glm::mat4 Projection, GLfloat kut, glm::vec3 pomak, glm::vec3 skaliranje);

    protected:

    private:
        static unsigned short vrhovi_stranica_podaci[6*4];
        static GLfloat vrhovi_podaci[14*3];
        static GLfloat uv_podaci[14*2];

        GLuint VertexArrayID; /* samo jedan element polja */
        GLuint MVP_ID, V_ID, M_ID, MV3x3_ID;
        GLuint vertexbuffer, uvbuffer, normbuffer, tangentbuffer, bitangentbuffer, elementbuffer;



        GLfloat *vrhovi, *uvovi, *normale, *tangente, *bitangente;
        unsigned short *indeksi;

        void izracunajVrhove();
};

#endif // KOCKA_H
