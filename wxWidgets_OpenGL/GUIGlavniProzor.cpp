///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 21 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIGlavniProzor.h"

 ///////////////////////////////////////////////////////////////////////////

GlavniProzor::GlavniProzor( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Alt+F4"), wxT("Zatvori aplikaciju"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );

	mbar->Append( fileMenu, wxT("&File") );

	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Prikaži info"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );

	mbar->Append( helpMenu, wxT("&Help") );

	this->SetMenuBar( mbar );

	wxBoxSizer* bSizerGlavni;
	bSizerGlavni = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizerPomocni;
	bSizerPomocni = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerGrafika_Kontrole;
	bSizerGrafika_Kontrole = new wxBoxSizer( wxHORIZONTAL );

	sizerCrtanje = new wxBoxSizer( wxVERTICAL );


	bSizerGrafika_Kontrole->Add( sizerCrtanje, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerKontrole;
	bSizerKontrole = new wxBoxSizer( wxVERTICAL );

	bSizerKontrole->SetMinSize( wxSize( 150,-1 ) );
	panelKontrole = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizerKontrole->Add( panelKontrole, 1, 0, 5 );

	gumbPomakni = new wxButton( this, wxID_ANY, wxT("Pomakni"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerKontrole->Add( gumbPomakni, 0, wxALL, 5 );

	gumbPokreni = new wxButton( this, wxID_ANY, wxT("Pokreni"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerKontrole->Add( gumbPokreni, 0, wxALL, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerKontrole->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	brojacKuta = new wxSpinCtrlDouble( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 0, 5 );
	brojacKuta->SetDigits( 1 );
	brojacKuta->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
	brojacKuta->SetForegroundColour( wxColour( 255, 128, 0 ) );
	brojacKuta->SetBackgroundColour( wxColour( 0, 0, 0 ) );

	bSizerKontrole->Add( brojacKuta, 0, wxALL, 5 );


	bSizerGrafika_Kontrole->Add( bSizerKontrole, 0, 0, 5 );


	bSizerPomocni->Add( bSizerGrafika_Kontrole, 2, wxEXPAND, 5 );

	konzola = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CHARWRAP|wxTE_MULTILINE );
	konzola->SetFont( wxFont( 10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Lucida Console") ) );
	konzola->SetForegroundColour( wxColour( 255, 153, 0 ) );
	konzola->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	konzola->SetMinSize( wxSize( -1,150 ) );

	bSizerPomocni->Add( konzola, 0, wxEXPAND, 5 );


	bSizerGlavni->Add( bSizerPomocni, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerGlavni );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GlavniProzor::OnClose ) );
	fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GlavniProzor::OnQuit ), this, menuFileQuit->GetId());
	helpMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GlavniProzor::OnAbout ), this, menuHelpAbout->GetId());
	gumbPomakni->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( GlavniProzor::GumbPomakni ), NULL, this );
	gumbPokreni->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( GlavniProzor::GumbPokreniZaustavi ), NULL, this );
	brojacKuta->Connect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( GlavniProzor::PomakKuta ), NULL, this );
}

GlavniProzor::~GlavniProzor()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GlavniProzor::OnClose ) );
	gumbPomakni->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( GlavniProzor::GumbPomakni ), NULL, this );
	gumbPokreni->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( GlavniProzor::GumbPokreniZaustavi ), NULL, this );
	brojacKuta->Disconnect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( GlavniProzor::PomakKuta ), NULL, this );

}
