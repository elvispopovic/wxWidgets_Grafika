#ifndef SHADER_H
#define SHADER_H


class Shader
{
    public:
        Shader(wxGLCanvas *canvas);
        virtual ~Shader();
        bool Kreiraj(wxString stazaVertex, wxString stazaFragment);
        bool KoristiProgram();
        void PrestaniKoristiti();
        GLuint DohvatiProgram();
    protected:

    private:
        bool kreirano;
        GLuint shaderProgram;
        wxEvtHandler *prozorEvtHandler;
        wxString UcitajTekstualnuDatoteku(wxString staza);
        bool ProvjeriShader(GLuint shader, wxString staza);
        bool ProvjeriProgram(GLuint program);
};

#endif // SHADER_H
