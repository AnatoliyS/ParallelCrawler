
// CrawlerView.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "CrawlerView.h"
#include "afxdialogex.h"

namespace crawler {

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


	// CrawlerView dialog



	CrawlerView::CrawlerView(CWnd* pParent /*=NULL*/)
		: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
	{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	}

	void CrawlerView::DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_LIST1, mTaskList);
	}

	BEGIN_MESSAGE_MAP(CrawlerView, CDialogEx)
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
	END_MESSAGE_MAP()


	// CrawlerView message handlers

	BOOL CrawlerView::OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO: Add extra initialization here
		UpdateData(TRUE);
		mTaskList.InsertColumn(0, L"URL", LVCFMT_LEFT, 200);
		mTaskList.InsertColumn(1, L"Status", LVCFMT_LEFT, 200);
		int num = mTaskList.InsertItem(0, L"sdfdsf");
		mTaskList.SetItemText(num, 1, L"Done");
		UpdateData(FALSE);

		return TRUE;  // return TRUE  unless you set the focus to a control
	}

	// If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.

	void CrawlerView::OnPaint()
	{
		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CDialogEx::OnPaint();
		}
	}

	// The system calls this function to obtain the cursor to display while the user drags
	//  the minimized window.
	HCURSOR CrawlerView::OnQueryDragIcon()
	{
		return static_cast<HCURSOR>(m_hIcon);
	}

}