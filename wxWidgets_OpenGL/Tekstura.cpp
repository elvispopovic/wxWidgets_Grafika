#include "Tekstura.h"
#include "GL_Panel.h"

Tekstura::Tekstura(wxGLCanvas *canvas)
{
    inicijalizirano = false;
    tekstura = teksturaId = 0;
    GL_Panel* panel = dynamic_cast<GL_Panel*>(canvas);
    if(panel)
        prozorEvtHandler = panel->DohvatiRukovateljProzora();

}

Tekstura::~Tekstura()
{
    //dtor
}


GLuint Tekstura::PoveziSaLokacijom(GLuint shaderProgram, wxString ime)
{
    if(inicijalizirano == false)
        return (GLuint)0;
    this->shaderProgram = shaderProgram;
    teksturaId  = glGetUniformLocation(shaderProgram, ime);
    return teksturaId;
}

bool Tekstura::Aktiviraj(GLuint teksturnaJedinica)
{
    if(inicijalizirano == false)
        return false;
    this->teksturnaJedinica = teksturnaJedinica;
    glActiveTexture(GL_TEXTURE0+teksturnaJedinica);
	glBindTexture(GL_TEXTURE_2D, tekstura);
	glUniform1i(teksturaId, teksturnaJedinica);
	return true;
}

void Tekstura::Deaktiviraj()
{
    glBindTexture(GL_TEXTURE_2D, teksturnaJedinica);
    glActiveTexture(GL_TEXTURE0);
}

GLuint Tekstura::DohvatiTeksturu()
{
    return teksturaId;
}

bool Tekstura::Kreiraj(wxString staza)
{
    unsigned int visina, sirina, linVelicina, mipMapBroj, fourCC, format, velicinaBloka;
    unsigned int vis_tmp, sir_tmp, vel_tmp, off_tmp;
    long velicina;
    wxString ispis;
    char header[128];
    unsigned char* buffer;
    if(prozorEvtHandler == nullptr)
    {
        return false;
    }
	if(!wxFile::Exists(staza))
    {
        wxString poruka(wxT("Nije pronađena datoteka "));
        poruka << staza << "\n";
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, poruka);
        return false;
    }
    wxFile datoteka(staza);
    datoteka.SeekEnd();
    velicina = datoteka.Tell();
    datoteka.Seek(0);

    datoteka.Read(header, 128);
    if(std::strncmp(header,"DDS ",4))
    {
        wxString poruka("Datoteka");
        poruka << staza << " nije DDS.\n";
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, poruka);
        return false;
    }

    visina = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
	sirina = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
	linVelicina = (header[20]) | (header[21] << 8) | (header[22] << 16) | (header[23] << 24);
	mipMapBroj = (header[28]) | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);
	fourCC = (header[84]) | (header[85] << 8) | (header[86] << 16) | (header[87] << 24);

	ispis.clear();
	if(header[84] == 'D')
    {
        ispis << "DDS format: ";
		switch(header[87])
		{
			case '1': // DXT1
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                ispis << "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT\n";
				velicinaBloka = 8;
				break;
			case '3': // DXT3
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				ispis << "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT\n";
				velicinaBloka = 16;
				break;
			case '5': // DXT5
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				ispis << "GL_COMPRESSED_RGBA_S3TC_DXT4_EXT\n";
				velicinaBloka = 16;
				break;
			case '0': // DX10
				// unsupported, else will error
				// as it adds sizeof(struct DDS_HEADER_DXT10) between pixels
				// so, buffer = malloc((file_size - 128) - sizeof(struct DDS_HEADER_DXT10));
			default: PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, wxT("Format nije podržan.\n")); return -1;
		}
	} else // BC4U/BC4S/ATI2/BC55/R8G8_B8G8/G8R8_G8B8/UYVY-packed/YUY2-packed unsupported
		return false;
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, ispis);
    ispis.clear();
    ispis << wxT("Učitavanje DDS datoteke ") << staza << " (" << (velicina/1024) << " KB)\n";
    ispis << wxT("Širina: ") << sirina << ", visina: " << visina << wxT(", lin. veličina: ") << linVelicina << ", mip map: " << mipMapBroj << ", four CC: " << fourCC << "\n";
    PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, ispis);
    buffer = new unsigned char[velicina-128];
    if(buffer==nullptr)
    {
        wxString poruka(wxT("Ne mogu alocirati memoriju za učitavanje DDS datoteke.\n"));
        PorukaPaneluPodaci::UpisiUKonzolu(prozorEvtHandler, ispis);
        return -1;
    }
    datoteka.Read(buffer, velicina);
    glGenTextures(1, &tekstura);
	if(tekstura == 0)
        return false;
    glBindTexture(GL_TEXTURE_2D, tekstura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapBroj-1); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    vis_tmp = visina; sir_tmp = sirina;
    vel_tmp = 0; off_tmp = 0;
    for (unsigned int i=0; i<mipMapBroj; i++)
    {
        if(vis_tmp == 0 || sir_tmp == 0)
        {
            mipMapBroj --;
            continue;
        }
        vel_tmp = ((sir_tmp+3)/4) * ((vis_tmp+3)/4) * velicinaBloka;
        glCompressedTexImage2D(GL_TEXTURE_2D, i, format, sir_tmp, vis_tmp, 0, vel_tmp, buffer + off_tmp);
        off_tmp += vel_tmp;
        sir_tmp /= 2;
        vis_tmp /= 2;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapBroj-1);
    glBindTexture(GL_TEXTURE_2D, 0);

    PorukaPaneluPodaci::ProvjeriGresku(prozorEvtHandler);

    delete[] buffer;
    inicijalizirano = true;
    return true;
}
