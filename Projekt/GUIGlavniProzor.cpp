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

	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("&Pokreni") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem3 );

	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu3, wxID_ANY, wxString( wxT("&Zaustavi") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem4 );

	mbar->Append( m_menu3, wxT("&Upravljanje") );

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

	panelCrtanje = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	panelCrtanje->SetBackgroundColour( wxColour( 0, 0, 0 ) );

	bSizerGrafika_Kontrole->Add( panelCrtanje, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerKontrole;
	bSizerKontrole = new wxBoxSizer( wxVERTICAL );

	bSizerKontrole->SetMinSize( wxSize( 150,-1 ) );
	panelKontrole = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizerKontrole->Add( panelKontrole, 1, 0, 5 );

	but_pokreni = new wxButton( this, wxID_ANY, wxT("Pokreni"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerKontrole->Add( but_pokreni, 0, wxALL|wxEXPAND, 5 );

	but_zaustavi = new wxButton( this, wxID_ANY, wxT("Zaustavi"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerKontrole->Add( but_zaustavi, 0, wxALL|wxEXPAND, 5 );


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
	m_menu3->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GlavniProzor::PokreniDretve ), this, m_menuItem3->GetId());
	m_menu3->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GlavniProzor::ZaustaviDretve ), this, m_menuItem4->GetId());
	helpMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GlavniProzor::OnAbout ), this, menuHelpAbout->GetId());
	panelCrtanje->Connect( wxEVT_PAINT, wxPaintEventHandler( GlavniProzor::PanelCrtanje ), NULL, this );
	but_pokreni->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlavniProzor::PokreniDretve ), NULL, this );
	but_zaustavi->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlavniProzor::ZaustaviDretve ), NULL, this );
}

GlavniProzor::~GlavniProzor()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GlavniProzor::OnClose ) );
	panelCrtanje->Disconnect( wxEVT_PAINT, wxPaintEventHandler( GlavniProzor::PanelCrtanje ), NULL, this );
	but_pokreni->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlavniProzor::PokreniDretve ), NULL, this );
	but_zaustavi->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GlavniProzor::ZaustaviDretve ), NULL, this );

}
