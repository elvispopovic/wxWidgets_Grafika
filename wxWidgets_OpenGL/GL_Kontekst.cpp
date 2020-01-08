#include "GL_Kontekst.h"
#include "GL_Panel.h"

GL_Kontekst::GL_Kontekst(wxGLCanvas *canvas) : wxGLContext(canvas, NULL)
{
    this->canvas = canvas;
    inicijalizirano = false;
    shader = nullptr;
    kocka_tekstura = kocka_normal_mapa = kocka_specular_mapa = nullptr;
    lopta_tekstura = lopta_normal_mapa = lopta_specular_mapa = nullptr;
    glavnoSvjetlo=dodatnoSvjetlo1=dodatnoSvjetlo2 = nullptr;
    GL_Panel* panel = dynamic_cast<GL_Panel*>(canvas);
    if(panel)
        prozorEvtHandler = panel->DohvatiRukovateljProzora();
    kocka = new Kocka(canvas);
    lopta = new Kugla(canvas, 30);

}

GL_Kontekst::~GL_Kontekst()
{
    delete lopta;
    delete kocka;
    if(shader != nullptr)
        delete shader;
    if(kocka_tekstura != nullptr)
    {
        kocka_tekstura->Deaktiviraj();
        delete kocka_tekstura;
    }

    if(kocka_normal_mapa != nullptr)
    {
        kocka_normal_mapa->Deaktiviraj();
        delete kocka_normal_mapa;
    }

    if(kocka_specular_mapa != nullptr)
    {
        kocka_specular_mapa->Deaktiviraj();
        delete kocka_specular_mapa;
    }

    if(lopta_tekstura != nullptr)
    {
        lopta_tekstura->Deaktiviraj();
        delete lopta_tekstura;
    }

    if(lopta_normal_mapa != nullptr)
    {
        lopta_normal_mapa->Deaktiviraj();
        delete lopta_normal_mapa;
    }

    if(lopta_specular_mapa != nullptr)
    {
        lopta_specular_mapa->Deaktiviraj();
        delete lopta_specular_mapa;
    }
    if(glavnoSvjetlo != nullptr)
        delete glavnoSvjetlo;
    if(dodatnoSvjetlo1 != nullptr)
        delete dodatnoSvjetlo1;
    if(dodatnoSvjetlo2 != nullptr)
        delete dodatnoSvjetlo2;

}

/* moze se inicijalizirati tek nakon pokretanja GLEW-a */
bool GL_Kontekst::Inicijaliziraj()
{
    wxString sadrzaj;
    GLenum error;
    if(inicijalizirano == true)
        return true;
    if((error = glewInit())!=GLEW_OK)
    {
        sadrzaj << wxT("Greška prilikom inicijalizacije GLEW-a: ") << glewGetErrorString(error) << "\n";
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
        return false;
    }
    else
    {
        sadrzaj << wxT("Inicijaliziran GLEW: ") << glewGetErrorString(error) << "\n";
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    shader = new Shader(canvas);
    if(shader->Kreiraj("ScenaVertex.glsl", "ScenaFragment.glsl")==false)
        return false;

    GLuint program = shader->DohvatiProgram();
    kocka_tekstura = new Tekstura(canvas);
    kocka_tekstura->Kreiraj("kocka_difuzna.dds");
    kocka_tekstura->PoveziSaLokacijom(program, "difuzni_sampler");

    kocka_normal_mapa = new Tekstura(canvas);
    kocka_normal_mapa->Kreiraj("kocka_normal.dds");
    kocka_normal_mapa->PoveziSaLokacijom(program, "normal_sampler");

    kocka_specular_mapa = new Tekstura(canvas);
    kocka_specular_mapa->Kreiraj("kocka_specular.dds");
    kocka_specular_mapa->PoveziSaLokacijom(program, "spekularni_sampler");

    lopta_tekstura = new Tekstura(canvas);
    lopta_tekstura->Kreiraj("euroball_difuzna.dds");
    lopta_tekstura->PoveziSaLokacijom(program, "difuzni_sampler");

    lopta_normal_mapa = new Tekstura(canvas);
    lopta_normal_mapa->Kreiraj("euroball_normal.dds");
    lopta_normal_mapa->PoveziSaLokacijom(program, "normal_sampler");

    lopta_specular_mapa = new Tekstura(canvas);
    lopta_specular_mapa->Kreiraj("euroball_specular.dds");
    lopta_specular_mapa->PoveziSaLokacijom(program, "spekularni_sampler");

    glavnoSvjetlo = new Svjetlo(glm::vec3(0.0,6.0,3.0), 4.0, glm::vec3(1.0,1.0,1.0));
    dodatnoSvjetlo1 = new Svjetlo(glm::vec3(-3.0,0.0,2.0), 2.0, glm::vec3(1.0,1.0,0.5));
    dodatnoSvjetlo2 = new Svjetlo(glm::vec3(3.0,0.0,2.0), 2.0, glm::vec3(0.5,1.0,1.0));

    glavnoSvjetlo->PoveziSaLokacijomSvjetla(program,"svjetlo",0);
    dodatnoSvjetlo1->PoveziSaLokacijomSvjetla(program,"svjetlo",1);
    dodatnoSvjetlo2->PoveziSaLokacijomSvjetla(program,"svjetlo",2);

    kocka->Inicijaliziraj(shader);
    lopta->Inicijaliziraj(shader);
    return true;
}

void GL_Kontekst::PostaviViewport(wxSize velicina)
{
    wxString sadrzaj;
    GLfloat omjer;
    if (velicina.x < 1) velicina.x = 1;
    if (velicina.y < 1) velicina.y = 1;
    glViewport(0, 0, (GLsizei)velicina.x, (GLsizei)velicina.y);
    if(velicina.y==0)
        omjer = GLfloat(velicina.x);
	else
        omjer=GLfloat(velicina.x)/GLfloat(velicina.y);

    Projection = glm::perspective(glm::radians(45.0f), omjer, 0.1f, 100.0f);
    View       = glm::lookAt(
                        glm::vec3(0,3,4), // Pozicija kamere u svjetskom sustavu
                        glm::vec3(0,0,0), // tocka prema kuda kamera gleda
                        glm::vec3(0,1,0)  // vektor smjera gore kamere
                        );

    sadrzaj << wxT("Viewport: ") << velicina.x << " x " << velicina.y << "\n";
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
}

void GL_Kontekst::Render(GLfloat kut, int tip)
{
    glClearColor((GLfloat)0.0, (GLfloat)0.07, (GLfloat)0.1, (GLfloat)1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(shader == nullptr)
        return;
    shader->KoristiProgram();

    glavnoSvjetlo->Aktiviraj();
    dodatnoSvjetlo1->Aktiviraj();
    dodatnoSvjetlo2->Aktiviraj();

    if(tip == 0 && kocka != nullptr)
    {
        kocka_tekstura->Aktiviraj(0);
        kocka_normal_mapa->Aktiviraj(1);
        kocka_specular_mapa->Aktiviraj(2);
        kocka->Render(View, Projection, kut, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else if(lopta != nullptr)
    {
        lopta_tekstura->Aktiviraj(0);
        lopta_normal_mapa->Aktiviraj(1);
        lopta_specular_mapa->Aktiviraj(2);
        lopta->Render(View, Projection, kut, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    }
}


