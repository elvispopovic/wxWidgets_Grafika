/***************************************************************
 * Name:      wx_pch.h
 * Purpose:   Header to create Pre-Compiled Header (PCH)
 * Author:    EPC ()
 * Created:   2019-11-12
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

#ifdef WX_GCH
#include <wx_pch.h>
#else
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef WX_PRECOMP
    // put here all your rarely-changing header files
    #include <wx/app.h>
    #include <wx/wx.h>
    #include <wx/graphics.h>
    #include <wx/dcclient.h>
    #include <wx/dcgraph.h>
    #include <wx/dcbuffer.h>
    #include <wx/rawbmp.h>
    #include <iostream>
    #include <ctime>
    #include <thread>
    #include <future>
#endif // WX_PRECOMP
#endif // WX_GCH

#endif // WX_PCH_H_INCLUDED
