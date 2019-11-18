/***************************************************************
 * Name:      Projekt2App.h
 * Purpose:   Defines Application Class
 * Author:    EPC ()
 * Created:   2019-11-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifndef PROJEKT2APP_H
#define PROJEKT2APP_H

#include "interfaces.h"
#include "Model.h"
#include "ProjektProzor.h"

class ProjektApp : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();

    private:
        GlavniProzor1* prozor;
        IGrafikaGDI* grafika;
        Model* model;
};

#endif // PROJEKT2APP_H
