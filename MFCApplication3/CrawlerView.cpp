// CrawlerView.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "CrawlerView.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



namespace crawler {

	BEGIN_MESSAGE_MAP(CrawlerView, CDialogEx)
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		ON_MESSAGE(WM_UPDATE_CONTROL, &CrawlerView::OnUpdateControl)
		ON_BN_CLICKED(IDC_BUTTON1, &CrawlerView::OnExitButtonClicked)
		ON_BN_CLICKED(IDC_BUTTON2, &CrawlerView::OnStartButtonClicked)
		ON_BN_CLICKED(IDC_BUTTON3, &CrawlerView::OnStopButtonClicked)
	END_MESSAGE_MAP()

	// CrawlerView dialog
	CrawlerView::CrawlerView(CWnd* pParent /*=NULL*/)
		: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
		, mURL(_T("http://bbc.co.uk"))
		, mMaxThreads(5)
		, mMaxPages(50)
		, mPattern(_T("Ukraine"))
	{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	}

	void CrawlerView::setOnStartButtonListener(OnStartButtonListener _listener) {
		onStartListener = _listener;
	}

	void CrawlerView::setOnStopButtonListener(OnStopButtonListener _listener) {
		onStopListener = _listener;
	}

	HWND CrawlerView::getHandle() {
		return handle;
	}

	BOOL CrawlerView::Create(UINT nID, CWnd * pWnd)
	{
		return CDialogEx::Create(nID, pWnd);
	}

	void CrawlerView::addTask(const Task* task)
	{
		UpdateData(TRUE);
		CString url = CString(task->getURL().c_str());
		CString status = CString(task->getStatus().c_str());
		int count = mTaskList.GetItemCount();
		int num = mTaskList.InsertItem(count, url);
		mTaskList.SetItemText(num, 1, status);
		mTaskList.EnsureVisible(num, TRUE);
		UpdateData(FALSE);
	}

	void CrawlerView::DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_LIST1, mTaskList);
		DDX_Text(pDX, IDC_EDIT1, mURL);
		DDV_MaxChars(pDX, mURL, 250);
		DDX_Text(pDX, IDC_EDIT3, mMaxThreads);
		DDV_MinMaxInt(pDX, mMaxThreads, 0, 20);
		DDX_Text(pDX, IDC_EDIT2, mMaxPages);
		DDV_MinMaxInt(pDX, mMaxPages, 0, 1000000000);
		DDX_Text(pDX, IDC_EDIT4, mPattern);
		DDV_MaxChars(pDX, mPattern, 255);
	}	

	LRESULT CrawlerView::OnUpdateControl(WPARAM wParam, LPARAM lParam)
	{
		Task* t = reinterpret_cast<Task*>(lParam);
		addTask(t);
		delete t;
		return 0;
	}


	BOOL CrawlerView::OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		handle = GetSafeHwnd();

		// TODO: Add extra initialization here
		UpdateData(TRUE);
		mTaskList.InsertColumn(0, L"URL", LVCFMT_LEFT, 500);
		mTaskList.InsertColumn(1, L"Status", LVCFMT_LEFT, 200);
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

	void CrawlerView::OnExitButtonClicked()
	{
		DestroyWindow();
	}

	void CrawlerView::OnStartButtonClicked()
	{
		UpdateData(TRUE);
		mTaskList.DeleteAllItems();
		UpdateData(FALSE);

		onStartListener();
	}

	void CrawlerView::OnStopButtonClicked()
	{
		onStopListener();
	}
}


