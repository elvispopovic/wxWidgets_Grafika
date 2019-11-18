///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 21 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1000
#define idMenuAbout 1001

///////////////////////////////////////////////////////////////////////////////
/// Class GlavniProzor
///////////////////////////////////////////////////////////////////////////////
class GlavniProzor : public wxFrame
{
	private:

	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* m_menu3;
		wxMenu* helpMenu;
		wxPanel* panelCrtanje;
		wxPanel* panelKontrole;
		wxButton* but_pokreni;
		wxButton* but_zaustavi;
		wxTextCtrl* konzola;
		wxStatusBar* statusBar;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void PokreniDretve( wxCommandEvent& event ) { event.Skip(); }
		virtual void ZaustaviDretve( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void PanelCrtanje( wxPaintEvent& event ) { event.Skip(); }


	public:

		GlavniProzor( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Grafika"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,700 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~GlavniProzor();

};

