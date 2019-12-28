#include "GL_Kontekst.h"
#include "GL_Panel.h"

//36 vrhova
unsigned short GL_Kontekst::vrhovi_stranica_podaci[] = {
    0,1,2,3, 4,5,6,7, 0,3,8,9, 3,2,10,11, 2,1,12,13, 1,0,4,7
};

/*
Vrhovi su redundantni da odgovaraju vrhovima UV mape
Kriz kocke ima 14 vrhova
*/
GLfloat GL_Kontekst::vrhovi_podaci[] = {
  -1.0f,  -1.0f,   1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f
};

/* vrhovi u UV mapi */
GLfloat GL_Kontekst::uv_podaci[] = {
   0.5f,  1.0-0.75f,
   0.5f,  1.0-0.5f,
   0.75f, 1.0-0.5f,
   0.75f, 1.0-0.75f,

   0.25f, 1.0-0.75f,
   0.0f,  1.0-0.75f,
   0.0f,  1.0-0.5f,
   0.25f, 1.0-0.5f,

   0.75f, 1.0-1.0f,
   0.5f,  1.0-1.0f,
   1.0f,  1.0-0.5f,
   1.0f,  1.0-0.75f,

   0.5f,  1.0-0.25f,
   0.75f, 1.0-0.25f
};

GL_Kontekst::GL_Kontekst(wxGLCanvas *canvas) : wxGLContext(canvas, NULL)
{
    this->canvas = canvas;
    inicijalizirano = false;
    shader = nullptr;
    tekstura = normal_mapa = specular_mapa = nullptr;
    GL_Panel* panel = dynamic_cast<GL_Panel*>(canvas);
    if(panel)
        prozorEvtHandler = panel->DohvatiRukovateljProzora();
    vrhovi = new GLfloat[3*24];
    uvovi = new GLfloat[2*24];
    normale = new GLfloat[3*24];
    tangente = new GLfloat[3*24];
    bitangente = new GLfloat[3*24];
    indeksi = new unsigned short[36];
}

GL_Kontekst::~GL_Kontekst()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteVertexArrays(1, VertexArrayID);
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
    delete glavnoSvjetlo;
    delete dodatnoSvjetlo1;
    delete dodatnoSvjetlo2;
    delete[] vrhovi;
    delete[] uvovi;
    delete[] normale;
    delete[] tangente;
    delete[] bitangente;
    delete[] indeksi;
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
    MVP_ID = glGetUniformLocation(program, "MVP");
    V_ID = glGetUniformLocation(program, "V");
    M_ID = glGetUniformLocation(program, "M");
    MV3x3_ID = glGetUniformLocation(program, "MV3x3");

    //na pocetku
    glGenVertexArrays(1, VertexArrayID);
    glBindVertexArray(VertexArrayID[0]);
    //ucitavanje DDS teksture
    tekstura = new Tekstura(canvas);
    tekstura->Kreiraj("tekstrura_kocka.dds");
    tekstura->PoveziSaLokacijom(shader->DohvatiProgram(), "difuzni_sampler");

    normal_mapa = new Tekstura(canvas);
    normal_mapa->Kreiraj("normal_mapa.dds");
    normal_mapa->PoveziSaLokacijom(shader->DohvatiProgram(), "normal_sampler");

    specular_mapa = new Tekstura(canvas);
    specular_mapa->Kreiraj("specular_mapa.dds");
    specular_mapa->PoveziSaLokacijom(shader->DohvatiProgram(), "spekularni_sampler");

    glavnoSvjetlo = new Svjetlo(glm::vec3(4.0,4.0,3.0), 4.0, glm::vec3(1.0,1.0,1.0));
    dodatnoSvjetlo1 = new Svjetlo(glm::vec3(0.0,0.0,3.0), 1.0, glm::vec3(1.0,1.0,0.0));
    dodatnoSvjetlo2 = new Svjetlo(glm::vec3(2.0,0.0,-2.0), 1.0, glm::vec3(0.0,1.0,1.0));
    //glavnoSvjetlo->PoveziSaLokacijama(shader->DohvatiProgram(),"SvjetloPolozaj_svjetske","SvjetloBojaIntenzitet");
    glavnoSvjetlo->PoveziSaLokacijomSvjetla(shader->DohvatiProgram(),"svjetlo",0);
    dodatnoSvjetlo1->PoveziSaLokacijomSvjetla(shader->DohvatiProgram(),"svjetlo",1);
    dodatnoSvjetlo2->PoveziSaLokacijomSvjetla(shader->DohvatiProgram(),"svjetlo",2);
    //izracunavamo vrhove, uvove, normale, tangente i bitangente iz sirovih ulaznih podataka
    izracunajVrhove();

    //koordinate
    glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*36*sizeof(GLfloat), vrhovi, GL_STATIC_DRAW);

	//uv koordinate
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, 2*36*sizeof(GLfloat), uvovi, GL_STATIC_DRAW);

	//normale
	glGenBuffers(1, &normbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*36*sizeof(GLfloat), normale, GL_STATIC_DRAW);
	//tangente
	glGenBuffers(1, &tangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*36*sizeof(GLfloat), tangente, GL_STATIC_DRAW);
	//bitangente
	glGenBuffers(1, &bitangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*36*sizeof(GLfloat), bitangente, GL_STATIC_DRAW);

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned short), indeksi , GL_STATIC_DRAW);

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

void GL_Kontekst::Render(wxSize velicina, GLfloat kut)
{
    glClearColor((GLfloat)0.0, (GLfloat)0.07, (GLfloat)0.1, (GLfloat)1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 RotationMatrix = glm::eulerAngleYXZ(kut, 0.0f, 0.0f); //rotacija objekta
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f,0.0f,0.0f)); //pomak objekta
	glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 Model = TranslationMatrix * RotationMatrix * ScalingMatrix;
    glm::mat4 MV = View * Model;
    glm::mat3 MV3x3 = glm::mat3(MV);
    glm::mat4 MVP        = Projection * MV;

    shader->KoristiProgram();
    glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(V_ID, 1, GL_FALSE, &View[0][0]);
    glUniformMatrix3fv(MV3x3_ID, 1, GL_FALSE, &MV3x3[0][0]);

    tekstura->Aktiviraj(0);
    normal_mapa->Aktiviraj(1);
    specular_mapa->Aktiviraj(2);
    glavnoSvjetlo->Aktiviraj();
    dodatnoSvjetlo1->Aktiviraj();
    dodatnoSvjetlo2->Aktiviraj();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        2,                                // size : U+V => 2
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normbuffer); //aktiviramo buffer normala
	glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer); //aktiviramo buffer tangenti
	glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer); //aktiviramo buffer bitangenti
	glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    //glDrawArrays(GL_TRIANGLES, 0, 12*3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer); //aktiviramo buffer indeksa
	//crtamo gornju i donju stranicu
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

}

void GL_Kontekst::izracunajVrhove()
{

    static const unsigned short trokuti[6] = {0,1,2,0,2,3};

    GLfloat *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;
    unsigned short* ptr6;
    int i, j, k;
    int off1, off2;
    wxString sadrzaj;
    ptr1 = vrhovi;
    ptr2 = uvovi;
    ptr3 = normale;
    ptr4 = tangente;
    ptr5 = bitangente;
    ptr6 = indeksi;

    for(k=0; k<6; k++) //broj stranica
    {
        off1 = k*4;
        //vrhovi i uv-ovi
        for(j=0; j<4; j++)
        {
            off2 = vrhovi_stranica_podaci[off1+j]*3;
            for(i=0; i<3; i++)
                *(ptr1++) = vrhovi_podaci[off2+i];
            off2 = vrhovi_stranica_podaci[off1+j]*2;
            for(i=0; i<2; i++)
                *(ptr2++) = uv_podaci[off2+i];
        }
        //indeksi
        for(j=0; j<6; j++)
            *(ptr6++) = off1+trokuti[j];

        // do tuda smo dosli... stavljeno 4 umjesto 6
        off2 = k*12; //4*3
        glm::vec3 v1 = glm::vec3(vrhovi[off2+3], vrhovi[off2+4], vrhovi[off2+5])-glm::vec3(vrhovi[off2], vrhovi[off2+1], vrhovi[off2+2]);
        glm::vec3 v2 = glm::vec3(vrhovi[off2+9], vrhovi[off2+10], vrhovi[off2+11])-glm::vec3(vrhovi[off2], vrhovi[off2+1], vrhovi[off2+2]);
        glm::vec3 normala = glm::normalize(glm::cross(v1,v2));

        off2 = k*8; //4*2
        glm::vec2 uv1 = glm::vec2(uvovi[off2+2], uvovi[off2+3])-glm::vec2(uvovi[off2], uvovi[off2+1]);
        glm::vec2 uv2 = glm::vec2(uvovi[off2+6], uvovi[off2+7])-glm::vec2(uvovi[off2], uvovi[off2+1]);
        float r=1.0f/(uv1.x*uv2.y-uv1.y*uv2.x); //nazivnik
		glm::vec3 tangenta = glm::normalize((v1*uv2.y - v2*uv1.y)*r);
		glm::vec3 bitangenta = glm::normalize((v2*uv1.x - v1*uv2.x)*r);

		for(j=0; j<4; j++)
        {
            *(ptr3++) = normala.x;
            *(ptr3++) = normala.y;
            *(ptr3++) = normala.z;
            *(ptr4++) = tangenta.x;
            *(ptr4++) = tangenta.y;
            *(ptr4++) = tangenta.z;
            *(ptr5++) = bitangenta.x;
            *(ptr5++) = bitangenta.y;
            *(ptr5++) = bitangenta.z;
        }
    }
}

