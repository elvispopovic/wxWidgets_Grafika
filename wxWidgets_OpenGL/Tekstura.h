#ifndef TEKSTURA_H
#define TEKSTURA_H


class Tekstura
{
    public:
        Tekstura(wxGLCanvas *canvas);
        virtual ~Tekstura();
        bool Kreiraj(wxString staza);
        virtual GLuint PoveziSaLokacijom(GLuint shaderProgram, wxString ime);
        virtual bool Aktiviraj(GLuint teksturnaJedinica);
        virtual void Deaktiviraj();
        virtual GLuint DohvatiTeksturu();
    protected:

    private:
        GLuint shaderProgram, tekstura, teksturaId, teksturnaJedinica;
        bool inicijalizirano;
        wxEvtHandler *prozorEvtHandler;
};

#endif // TEKSTURA_H
