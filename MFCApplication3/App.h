// MFCApplication3.h : main header file for the PROJECT_NAME application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "CrawlerController.h"

using namespace crawler;

class App : public CWinApp {
public:
	App();
	~App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
private:
	DECLARE_MESSAGE_MAP()
	void initCrawler();

	// MVC pattern for crawler 
	shared_ptr<CrawlerView> view;
	shared_ptr<Crawler> crawler;
	unique_ptr<CrawlerController> controller;
};

extern App theApp;