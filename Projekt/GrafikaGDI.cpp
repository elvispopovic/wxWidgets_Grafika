
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "GrafikaGdi.h"

/* zbog korištenja UNICODE-a, stranica u kompileru mora biti podešena na UTF-8 */

GrafikaGDI::GrafikaGDI()
{
    renderer = wxGraphicsRenderer::GetGDIPlusRenderer();
    bitmap=new wxBitmap(wxSystemSettings::GetMetric ( wxSYS_SCREEN_X ) ,wxSystemSettings::GetMetric ( wxSYS_SCREEN_Y ), 32);
    memDC = new wxMemoryDC(*bitmap);
    memDC -> SetBackground(*wxBLACK_BRUSH);
    memDC -> Clear();
    CrtajTest();
    wxInitAllImageHandlers();
}

GrafikaGDI::~GrafikaGDI()
{
    memDC->SelectObject(wxNullBitmap);
    delete bitmap;
    delete memDC;
}

/* ova poveznica omoguæava slanje poruka za status bar prozora */
bool GrafikaGDI::PoveziSaProzorom(GlavniProzor* prozor)
{
    int v1,v2,v3;
    prozorEvtHandler = (wxEvtHandler*)prozor;
    wxString upis;

    upis = renderer->GetName();
    renderer->GetVersion(&v1,&v2,&v3);
    upis << " " << v1 << "." << v2 << "." << v3;
    upisiStatus(upis);

    upis.Clear();
    upis << wxT("Unicode test: ČčĆćŽžŠšĐđ\n");
    upisiUKonzolu(upis);

    return true;
}



/* crta testni crtez u bitmapu preko memorijskog konteksta */
void GrafikaGDI::CrtajTest(wxAntialiasMode aa)
{
    memDC->SelectObject(*bitmap); //u raw pristupu se deselektira bitmapa pa ju treba selektirati
    wxGraphicsContext* gc = wxGraphicsContext::Create( *memDC );
    if(gc)
    {
        gc->SetPen( *wxRED_PEN );
        gc->SetAntialiasMode( aa );
        wxGraphicsPath path = gc->CreatePath();
        path.AddCircle( 50.0, 50.0, 50.0 );
        path.MoveToPoint(0.0, 50.0);
        path.AddLineToPoint(100.0, 50.0);
        path.MoveToPoint(50.0, 0.0);
        path.AddLineToPoint(50.0, 100.0 );
        path.CloseSubpath();
        path.AddRectangle(25.0, 25.0, 50.0, 50.0);
        gc->StrokePath(path);
        delete gc;
    }
}

void GrafikaGDI::BitmapTest(unsigned char pomak)
{
    static unsigned char p = 0;
    int w, h, s;
    wxString upis;
    memDC->SelectObject(wxNullBitmap);

    /* za 24 bita NativePixelData, a za 32 bita AlphaPixeldata */
    /* PixelData tj. raw pristup će zaključati pitmapu sve dok ne izađe iz dosega */
    wxAlphaPixelData npd(*bitmap);
    w = npd.GetWidth();
    h = npd.GetHeight();
    s = npd.GetRowStride();
    if(pomak==0)
        upis << wxT("Bitmapa - širina: ") << w << ", visina: " << h << wxT(", širina retka: ") << s << "\n";
    //else
    //    upis << wxT("Bitmapa, crtanja - pomak: ") << (int)p << "\n";
    upisiUKonzolu(upis);


    wxAlphaPixelData::Iterator it(npd);

    it.Offset(npd, 100, 100);

    for ( int y = 0; y < 255; ++y )
    {
        wxAlphaPixelData::Iterator pocetakReda = it;

        for ( int x = 0; x < 255; ++x, ++it )
        {
            it.Red() = x+p;
            it.Green() = 255-x-p;
            it.Blue() = y+p;
        }

        it = pocetakReda;
        it.OffsetY(npd, 1);
    }
    p+=pomak;
}

/* Kopira sadrzaj iz bitmape i njezinog konteksta u kontekst panela.
    Paint event mora biti od panela za crtanje */
bool GrafikaGDI::Blit(wxPaintEvent& event)
{
    int w, h;
    wxPanel *panel;
    panel = wxDynamicCast(event.GetEventObject(),wxPanel);
    if(memDC != nullptr && panel != nullptr)
    {
        /* ponovno selektiramo bitmapu u kontekst */
        memDC->SelectObject(*bitmap);
        panel->GetSize(&w, &h);
        wxAutoBufferedPaintDC hdc(panel);
        hdc.Blit(0,0,w,h,memDC,0,0);
        return true;
    }

    return false;
}

void GrafikaGDI::upisiUKonzolu(wxString sadrzaj)
{
    wxCommandEvent* konzolaEvent;
    if(prozorEvtHandler == nullptr)
        return;
    PorukaPodaci *pp = new PorukaPodaci;
    pp->t=PorukaPodaci::tip::Konzola;

    pp->sadrzaj = sadrzaj;
    konzolaEvent = new wxCommandEvent(EVT_PORUKA_PROZORU);
    konzolaEvent->SetClientData((void *)pp);
    wxQueueEvent(prozorEvtHandler,konzolaEvent);
}

void GrafikaGDI::upisiStatus(wxString sadrzaj)
{
    wxCommandEvent* statusEvent;
    if(prozorEvtHandler == nullptr)
        return;
    PorukaPodaci *pp = new PorukaPodaci;
    pp->t=PorukaPodaci::tip::Status;

    pp->sadrzaj = sadrzaj;
    statusEvent = new wxCommandEvent(EVT_PORUKA_PROZORU);
    statusEvent->SetClientData((void *)pp);
    wxQueueEvent(prozorEvtHandler,statusEvent);
}
