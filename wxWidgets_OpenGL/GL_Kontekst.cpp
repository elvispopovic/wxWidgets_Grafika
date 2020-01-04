#include "GL_Kontekst.h"
#include "GL_Panel.h"

GL_Kontekst::GL_Kontekst(wxGLCanvas *canvas) : wxGLContext(canvas, NULL)
{
    this->canvas = canvas;
    inicijalizirano = false;
    shader = nullptr;
    tekstura = normal_mapa = specular_mapa = nullptr;
    glavnoSvjetlo=dodatnoSvjetlo1=dodatnoSvjetlo2 = nullptr;
    GL_Panel* panel = dynamic_cast<GL_Panel*>(canvas);
    if(panel)
        prozorEvtHandler = panel->DohvatiRukovateljProzora();
    kocka = new Kocka();
}

GL_Kontekst::~GL_Kontekst()
{
    delete kocka;
    if(shader != nullptr)
        delete shader;
    if(tekstura != nullptr)
    {
        tekstura->Deaktiviraj();
        delete tekstura;
    }

    if(normal_mapa != nullptr)
    {
        normal_mapa->Deaktiviraj();
        delete normal_mapa;
    }

    if(specular_mapa != nullptr)
    {
        specular_mapa->Deaktiviraj();
        delete specular_mapa;
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
    tekstura = new Tekstura(canvas);
    tekstura->Kreiraj("tekstrura_kocka.dds");
    tekstura->PoveziSaLokacijom(program, "difuzni_sampler");

    normal_mapa = new Tekstura(canvas);
    normal_mapa->Kreiraj("normal_mapa.dds");
    normal_mapa->PoveziSaLokacijom(program, "normal_sampler");

    specular_mapa = new Tekstura(canvas);
    specular_mapa->Kreiraj("specular_mapa.dds");
    specular_mapa->PoveziSaLokacijom(program, "spekularni_sampler");


    glavnoSvjetlo = new Svjetlo(glm::vec3(4.0,4.0,3.0), 4.0, glm::vec3(1.0,1.0,1.0));
    dodatnoSvjetlo1 = new Svjetlo(glm::vec3(0.0,0.0,3.0), 1.0, glm::vec3(1.0,1.0,0.0));
    dodatnoSvjetlo2 = new Svjetlo(glm::vec3(2.0,0.0,-2.0), 1.0, glm::vec3(0.0,1.0,1.0));

    glavnoSvjetlo->PoveziSaLokacijomSvjetla(program,"svjetlo",0);
    dodatnoSvjetlo1->PoveziSaLokacijomSvjetla(program,"svjetlo",1);
    dodatnoSvjetlo2->PoveziSaLokacijomSvjetla(program,"svjetlo",2);

    glGenVertexArrays(1, VertexArrayID);
    kocka->Inicijaliziraj(canvas, VertexArrayID[0], shader);
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
                        glm::vec3(4,3,3), // Camera is at (4,3,-3), in World Space
                        glm::vec3(0,0,0), // and looks at the origin
                        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                        );

    sadrzaj << wxT("Viewport: ") << velicina.x << " x " << velicina.y << "\n";
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, sadrzaj);
}

void GL_Kontekst::Render(GLfloat kut)
{
    glClearColor((GLfloat)0.0, (GLfloat)0.07, (GLfloat)0.1, (GLfloat)1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(shader == nullptr)
        return;
    shader->KoristiProgram();
    tekstura->Aktiviraj(0);
    normal_mapa->Aktiviraj(1);
    specular_mapa->Aktiviraj(2);

    glavnoSvjetlo->Aktiviraj();
    dodatnoSvjetlo1->Aktiviraj();
    dodatnoSvjetlo2->Aktiviraj();
    if(kocka != nullptr)
        kocka->Render(View, Projection, kut, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}


