/***************************************************************
 * Name:      OpenGL_iz_nuleApp.h
 * Purpose:   Defines Application Class
 * Author:    EPC ()
 * Created:   2019-12-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifndef OPENGL_IZ_NULEAPP_H
#define OPENGL_IZ_NULEAPP_H

#include <wx/app.h>
#include "Model.h"

class Aplikacija : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();
    private:
        wxEvtHandler *prozorEvtHandler;
        Model* model;
        bool pokrenuto;
};

#endif // OPENGL_IZ_NULEAPP_H
