#include "Kugla.h"

Kugla::Kugla(wxGLCanvas *canvas, int podjela) : canvas(canvas)
{
    if(podjela<10)
        podjela=10;
    this->podjela = podjela;
    int u = (podjela-1)*(2*podjela+1)+2;
    int v = (2*(2*podjela+1)+2)*(podjela-4)+(2*(2*podjela+1)+1)*2;
    int w = 2*podjela+2;

    vrhovi = new GLfloat[u*3];
    UVovi = new GLfloat[u*2];
    normale = new GLfloat[u*3];
    tangente = new GLfloat[u*3];
    bitangente = new GLfloat[u*3];
    tstrip = new GLushort[v];
    tfan[0] = new GLushort[w];
    tfan[1] = new GLushort[w];
    inicijalizirano = false;
    kreirajGeometriju();
}

Kugla::~Kugla()
{
    delete[] tfan[1];
    delete[] tfan[0];
    delete[] tstrip;
    delete[] bitangente;
    delete[] tangente;
    delete[] normale;
    delete[] UVovi;
    delete[] vrhovi;
    if(inicijalizirano == true)
    {
        glDeleteVertexArrays(1, VertexArrayID);
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &uvbuffer);
        glDeleteBuffers(1, &normbuffer);
        glDeleteBuffers(1, &tangentbuffer);
        glDeleteBuffers(1, &bitangentbuffer);
        glDeleteBuffers(3, indexbuffer);
    }

}

bool Kugla::Inicijaliziraj(Shader* shader)
{
    if(inicijalizirano == true)
        return true;
    GLuint program = shader->DohvatiProgram();

    int u = (podjela-1)*(2*podjela+1)+2;
    int v = (2*(2*podjela+1)+2)*(podjela-4)+(2*(2*podjela+1)+1)*2;
    int w = 2*podjela+2;

    MVP_ID = glGetUniformLocation(program, "MVP");
    V_ID = glGetUniformLocation(program, "V");
    M_ID = glGetUniformLocation(program, "M");
    MV3x3_ID = glGetUniformLocation(program, "MV3x3");

    glGenVertexArrays(1, VertexArrayID);

    glGenBuffers(1, &vertexbuffer); //inicijalizacija
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); //aktivacija
	glBufferData(GL_ARRAY_BUFFER, u*3*sizeof(GLfloat), vrhovi, GL_STATIC_DRAW); //koordinate kugle

    glGenBuffers(1, &uvbuffer); //inicijalizacija
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer); //aktivacija
	glBufferData(GL_ARRAY_BUFFER, u*2*sizeof(GLfloat), UVovi, GL_STATIC_DRAW); //uv koordinate

	glGenBuffers(1, &normbuffer); //inicijalizacija
	glBindBuffer(GL_ARRAY_BUFFER, normbuffer); //aktivacija
	glBufferData(GL_ARRAY_BUFFER, u*3*sizeof(GLfloat), normale, GL_STATIC_DRAW); //koordinate kugle

	glGenBuffers(1, &tangentbuffer); //inicijalizacija
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer); //aktivacija
	glBufferData(GL_ARRAY_BUFFER, u*3*sizeof(GLfloat), tangente, GL_STATIC_DRAW); //koordinate kugle

	glGenBuffers(1, &bitangentbuffer); //inicijalizacija
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer); //aktivacija
	glBufferData(GL_ARRAY_BUFFER, u*3*sizeof(GLfloat), bitangente, GL_STATIC_DRAW); //koordinate kugle

	glGenBuffers(3, indexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexbuffer[0]); //za triangle fan sjevernog pola
	glBufferData(GL_ARRAY_BUFFER, w*sizeof(GLushort), tfan[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, indexbuffer[1]); //za triangle fan juznog pola
	glBufferData(GL_ARRAY_BUFFER, w*sizeof(GLushort), tfan[1], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, indexbuffer[2]); //za triangle strip pojasa
	glBufferData(GL_ARRAY_BUFFER, v*sizeof(GLushort), tstrip, GL_STATIC_DRAW);

	inicijalizirano = true;
    return true;
}

void Kugla::Render(glm::mat4 View, glm::mat4 Projection, GLfloat kut, glm::vec3 pomak, glm::vec3 skaliranje)
{
    if(inicijalizirano == false)
        return;

    int v = (2*(2*podjela+1)+2)*(podjela-4)+(2*(2*podjela+1)+1)*2;
    int w = 2*podjela+2;

    glm::mat4 RotationMatrix = glm::eulerAngleYXZ(kut, 0.0f, 0.0f); //rotacija objekta
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1), pomak); //pomak objekta
	glm::mat4 ScalingMatrix = glm::scale(glm::mat4(1), skaliranje);
    glm::mat4 Model = TranslationMatrix * RotationMatrix * ScalingMatrix;
    glm::mat4 MV = View * Model;
    glm::mat3 MV3x3 = glm::mat3(MV);
    glm::mat4 MVP        = Projection * MV;


    glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(M_ID, 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(V_ID, 1, GL_FALSE, &View[0][0]);
    glUniformMatrix3fv(MV3x3_ID, 1, GL_FALSE, &MV3x3[0][0]);

    glBindVertexArray(VertexArrayID[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // 0 odgovara layoutu u shaderu
        3,                  // velicina, 3 koordinate
        GL_FLOAT,           // tip
        GL_FALSE,           // normalizirano
        0,                  // korak
        (void*)0            // pomak polja
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        1,                                // 1 odgovara layoutu u shaderu
        2,                                // velicina, 2 koordinate
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalizirano
        0,                                // korak
        (void*)0                          // pomak polja
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


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer[0]); //aktiviramo indekse sjevernog triangle fana
	glDrawElements(GL_TRIANGLE_FAN, w, GL_UNSIGNED_SHORT, (void*)0); //crtamo triangle fan
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer[1]); //aktiviramo indekse juznog triangle fana
	glDrawElements(GL_TRIANGLE_FAN, w, GL_UNSIGNED_SHORT, (void*)0); //crtamo triangle fan
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer[2]); //aktiviramo indekse triangle stripa
	glDrawElements(GL_TRIANGLE_STRIP,v, GL_UNSIGNED_SHORT, (void*)0); //crtamo triangle strip

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

void Kugla::kreirajGeometriju()
{
    GLfloat *ptVrhovi=vrhovi;
    GLfloat *ptUVovi=UVovi;
    GLfloat *ptNormale = normale;
    GLfloat *ptTangente = tangente;
    GLfloat *ptBitangente = bitangente;
    GLushort *ptStrip = tstrip;
    GLushort *ptFan;
    glm::vec3 normala, tangenta, bitangenta;
    GLfloat R = 1.5;

    int i, j;
    int w = 2*podjela+2;

    // sjeverni pol
    ptFan=tfan[0];
    (*ptVrhovi++)=0.0f; (*ptVrhovi++)=1.0f; (*ptVrhovi++)=0.0f;
    (*ptUVovi++)=0.5f; (*ptUVovi++)=0.01;
    (*ptNormale++)=0.0; (*ptNormale++)=1.0; (*ptNormale++)=0.0;
    (*ptTangente++)=1.0f; (*ptTangente++)=0.0f; (*ptTangente++)=0.0f;
    (*ptBitangente++)=0.0f; (*ptBitangente++)=0.0f; (*ptBitangente++)=1.0f;
    for(int i=0; i<w; i++)
        *(ptFan++)=i;

    // sferni pojas

    for(j=1; j<podjela; j++)
    {
        GLfloat phi = glm::pi<GLfloat>()/podjela*(GLfloat)j;
		GLfloat r = glm::sin(phi);
		GLfloat uvy=phi/glm::pi<GLfloat>();

		for(i=0; i<(w-1); i++)
        {
            normala.x = r*glm::sin(2.0f*glm::pi<GLfloat>()/(2*podjela)*(GLfloat)i);
            normala.y = glm::cos(phi);
            normala.z = r*glm::cos(2.0f*glm::pi<GLfloat>()/(2*podjela)*(GLfloat)i);
            normala = glm::normalize(normala);

            (*ptVrhovi++) = R * normala.x;
            (*ptVrhovi++) = R * normala.y;
            (*ptVrhovi++) = R * normala.z;

            (*ptUVovi++)=(GLfloat)i/(2*podjela);
			(*ptUVovi++)=uvy;
			(*ptNormale++) = normala.x; (*ptNormale++) = normala.y; (*ptNormale++) = normala.z;
			tangenta = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f),normala));
			(*ptTangente++) = tangenta.x; (*ptTangente++) = tangenta.y; (*ptTangente++) = tangenta.z;
			bitangenta = glm::normalize(glm::cross(normala, tangenta));
			(*ptBitangente++) = bitangenta.x; (*ptBitangente++) = bitangenta.y; (*ptBitangente++) = bitangenta.z;
        }
    }

    for(j=0; j<(podjela-2); j++)
    {
		if(j>0)
        {
            (*ptStrip++)=j*(2*podjela+1)+1; //degenerirani
        }
		for(i=0; i<(w-1); i++)
		{
			(*ptStrip++)=j*(2*podjela+1)+i+1;
			(*ptStrip++)=(j+1)*(2*podjela+1)+i+1;
		}
		if(j<(podjela-3))
		{
            (*ptStrip++)=(j+1)*(2*podjela+1)+1; //degenerirani
		}
	}
    // juzni pol
    ptFan=tfan[1];
    (*ptVrhovi++)=0.0f; (*ptVrhovi++)=-1.0f; (*ptVrhovi++)=0.0f;
    (*ptUVovi++)=0.5f; (*ptUVovi++)=0.99f;
    (*ptNormale++)=0.0; (*ptNormale++)=-1.0; (*ptNormale++)=0.0;
    (*ptTangente++)=1.0f; (*ptTangente++)=0.0f; (*ptTangente++)=0.0f;
    (*ptBitangente++)=0.0f; (*ptBitangente++)=0.0f; (*ptBitangente++)=-1.0f;

    int u = (podjela-1)*(w-1)+2;
    for(i=u-1; i>(u-w); i--)
        *(ptFan++)=i;
}
