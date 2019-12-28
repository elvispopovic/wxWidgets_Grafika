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

bool Svjetlo::PoveziSaLokacijama(GLuint shaderProgram, wxString imePozicije, wxString imeBoje)
{
    polozajID = glGetUniformLocation(shaderProgram, imePozicije);
    bojaID = glGetUniformLocation(shaderProgram, imeBoje);
    return true;
}

void Svjetlo::Aktiviraj()
{
    glUniform3f(polozajID, polozaj.x, polozaj.y, polozaj.z);
    glUniform4f(bojaID, boja.x, boja.y, boja.z, intenzitet);
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
