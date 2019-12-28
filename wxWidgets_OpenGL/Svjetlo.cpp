#include "Svjetlo.h"

Svjetlo::Svjetlo(glm::vec3 polozaj, GLfloat intenzitet, glm::vec3 boja)
{
    this->polozaj = polozaj;
    this->intenzitet = intenzitet;
    this->boja = boja;
}

Svjetlo::~Svjetlo()
{
    //dtor
}

void Svjetlo::PoveziSaLokacijomSvjetla(GLuint shaderProgram, const char* imePozicije, int indeks)
{
    wxString pozicija;
    pozicija << imePozicije << "[" << indeks << "]";
    polozajID = glGetUniformLocation(shaderProgram, pozicija+".polozaj");
    bojaID = glGetUniformLocation(shaderProgram, pozicija+".boja");
    intenzitetID = glGetUniformLocation(shaderProgram, pozicija+".intenzitet");
}

void Svjetlo::Aktiviraj()
{
    glUniform3f(polozajID, polozaj.x, polozaj.y, polozaj.z);
    glUniform3f(bojaID, boja.x, boja.y, boja.z);
    glUniform1f(intenzitetID, intenzitet);

}

void Svjetlo::PostaviPolozaj(GLfloat x, GLfloat y, GLfloat z)
{
    polozaj.x = x;
    polozaj.y = y;
    polozaj.z = z;
}

void Svjetlo::PostaviBoju(GLfloat r, GLfloat g, GLfloat b)
{
    boja.x = r;
    boja.y = g;
    boja.z = b;
}

void Svjetlo::PostaviIntenzitet(GLfloat i)
{
    intenzitet = i;
}
