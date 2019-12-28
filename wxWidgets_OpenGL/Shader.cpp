#include "Shader.h"
#include "GL_Panel.h"

Shader::Shader(wxGLCanvas *canvas)
{
    shaderProgram = 0;
    kreirano = false;
    prozorEvtHandler = nullptr;
    GL_Panel* panel = dynamic_cast<GL_Panel*>(canvas);
    if(panel)
        prozorEvtHandler = panel->DohvatiRukovateljProzora();

}

Shader::~Shader()
{
    //dtor
}

bool Shader::Kreiraj(wxString stazaVertex, wxString stazaFragment)
{
    const GLchar *vertexText;
    const GLchar *fragmentText;
    wxString sadrzaj, vShader, fShader;

    if(kreirano == true)
        return true;

    vShader = UcitajTekstualnuDatoteku(stazaVertex);
    fShader = UcitajTekstualnuDatoteku(stazaFragment);
    if(vShader=="" || fShader=="")
        return false;
    vertexText = vShader.c_str().AsChar();
    fragmentText = fShader.c_str().AsChar();


    GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &vertexText, 0);
    glCompileShader(shaderVertex); // kompiliranje vertex shadera
    if(ProvjeriShader(shaderVertex, stazaVertex)==false)
    {
        glDeleteShader(shaderVertex);
        return false;
    }

    GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &fragmentText, 0);
    glCompileShader(shaderFragment); // kompiliranje vertex shadera
    if(ProvjeriShader(shaderFragment, stazaFragment) == false)
    {
        glDeleteShader(shaderVertex);
        glDeleteShader(shaderFragment);
        return false;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shaderVertex);
    glAttachShader(shaderProgram, shaderFragment);

    glLinkProgram(shaderProgram);
    glDetachShader(shaderProgram, shaderVertex);
    glDetachShader(shaderProgram, shaderFragment);

    if(ProvjeriProgram(shaderProgram)==false)
    {
        glDeleteShader(shaderVertex);
        glDeleteShader(shaderFragment);
        glDeleteProgram(shaderProgram);
        return false;
    }
    glDeleteShader(shaderVertex);
	glDeleteShader(shaderFragment);

	kreirano = true;
    return true;
}

bool Shader::KoristiProgram()
{
    if(kreirano == false)
        return false;
    glUseProgram(shaderProgram);
    return true;
}

void Shader::PrestaniKoristiti()
{
    glUseProgram(0);
}



GLuint Shader::DohvatiProgram()
{
    return shaderProgram;
}

bool Shader::ProvjeriShader(GLuint shader, wxString staza)
{
    wxString sadrzaj;
    GLsizei length = 0;
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer); // cita shader log
    if (length > 0)
    {
        sadrzaj << "Shader ID: " << shader << " (" << staza << ")\n" << "Status: " << buffer << "\n"; // ispisuje se log
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
        return false;
    }
    sadrzaj << "Shader ID: " << shader << " (" << staza << ")" << wxT(" je uspješno kompiliran.\n");
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    return true;
}

bool Shader::ProvjeriProgram(GLuint program)
{
    wxString sadrzaj;
    GLsizei length = 0;
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer); //
    if (length > 0) //
    {
        sadrzaj << "Program ID: " << program << "\n" << "Status linkera: " << buffer << "\n";
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    }

    glValidateProgram(program); // provjera ispravnosti shadera
    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    sadrzaj.clear();
    if (status == GL_FALSE) // ako postoji greska
    {
        sadrzaj << wxT("Greška prilikom validacije programa ") << program << "\n"; // ispisuje se
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
        return false;
    }
    sadrzaj << "Program ID: " << program << wxT(" je uspješno kompiliran.\n");
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    return true;
}

wxString Shader::UcitajTekstualnuDatoteku(wxString staza)
{
    wxString sadrzaj;
	wxTextFile datoteka(staza);
	if(!datoteka.Exists())
    {
        wxString poruka(wxT("Nije pronađena datoteka "));
        poruka << staza << "\n";
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, poruka);
        return sadrzaj;
    }

    datoteka.Open(staza);
	sadrzaj << datoteka.GetFirstLine() << "\n";
	while(!datoteka.Eof())
        sadrzaj << datoteka.GetNextLine() << "\n";
    return sadrzaj;
}
