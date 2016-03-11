#include "stdafx.h"
#include "App.h"
#include "CrawlerView.h"
#include "Crawler.h"
#include "CrawlerController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace crawler;

BEGIN_MESSAGE_MAP(App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

App::App() {
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

App::~App() {}

// The one and only App object
App theApp;

// Crawler initialization
void App::initCrawler() {
	crawler = make_shared<Crawler>();
	view = make_shared<CrawlerView>(this->GetMainWnd());
	controller = make_unique<CrawlerController>(crawler, view);

	// Show view
	m_pMainWnd = view.get();
	BOOL ret = view->Create(IDD_MFCAPPLICATION3_DIALOG, this->GetMainWnd());
	if (ret)
		view->ShowWindow(SW_SHOW);
}

// App initialization
BOOL App::InitInstance() {
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("Crawler"));

	initCrawler();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
}

