/***************************************************************
 * Name:      wx_pch.h
 * Purpose:   Header to create Pre-Compiled Header (PCH)
 * Author:    EPC ()
 * Created:   2019-12-06
 * Copyright: EPC ()
 * License:
 **************************************************************/

#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED

// basic wxWidgets headers
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef WX_PRECOMP
    #include <cstring>
    #define GLM_FORCE_RADIANS
    #include <glm/glm.hpp>
    #include <glm/gtc/type_ptr.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtx/euler_angles.hpp>

    #include <GL/glew.h> //GLEW mora biti prvi
    #include <wx/glcanvas.h>
    #include <wx/textfile.h>

    #include <ctime>
    #include <thread>
    #include <future>

    #include "Strukture.h"
#endif // WX_PRECOMP

#endif // WX_PCH_H_INCLUDED
