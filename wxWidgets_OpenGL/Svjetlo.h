#ifndef SVJETLO_H
#define SVJETLO_H


class Svjetlo
{
    public:
        Svjetlo(glm::vec3 polozaj=glm::vec3(0.0,0.0,0.0), GLfloat intenzitet = 1.0f, glm::vec3 boja=glm::vec3(1.0f,1.0f,1.0f));
        virtual ~Svjetlo();
        virtual bool PoveziSaLokacijama(GLuint shaderProgram, wxString imePozicije, wxString imeBoje);
        virtual void Aktiviraj();
        virtual void PostaviPolozaj(GLfloat x, GLfloat y, GLfloat z);
        virtual void PostaviBoju(GLfloat r, GLfloat g, GLfloat b);
        virtual void PostaviIntenzitet(GLfloat i);
    protected:

    private:
        GLuint polozajID, bojaID;
        glm::vec3 polozaj;
        glm::vec3 boja;
        GLfloat intenzitet;

};

#endif // SVJETLO_H
