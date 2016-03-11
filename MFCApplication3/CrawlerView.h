// CrawlerView.h : header file
//

#pragma once
#include "afxcmn.h"
#include "Task.h"

#define WM_UPDATE_CONTROL WM_APP + 0x10

namespace crawler {
	using OnStartButtonListener = function<void()>;
	using OnStopButtonListener = function<void()>;

	class CrawlerView : public CDialogEx {	
	public:
		CrawlerView(CWnd* pParent = NULL);	// standard constructor
		BOOL Create(UINT nID, CWnd * pWnd); // to create dialog

		// Set events listeners
		void setOnStartButtonListener(OnStartButtonListener);
		void setOnStopButtonListener(OnStopButtonListener);

		// Get window handle to recieve messages
		HWND getHandle();
		
		// Values binded to controls.
		CString mURL;		// initail URL
		CString mPattern;   // a word we search for
		int mMaxThreads;    // number of threads
		int mMaxPages;      // max URLs that we can visit

		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
		#endif

	protected:
		// Helper function to add task to mTaskList and deletes memory.
		// See CrawlerController, crawler->setOnTaskFinishedListener for details
		void addTask(const Task* task);

		// UI handlers
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support														// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnStartButtonClicked();
		afx_msg void OnStopButtonClicked();
		afx_msg void OnExitButtonClicked();
		DECLARE_MESSAGE_MAP()

		HICON m_hIcon;

		// Handle of window to recieve messages
		HWND handle;

		// List of URLs (Tasks)
		CListCtrl mTaskList;

		// Updates tasks list
		LRESULT OnUpdateControl(WPARAM wParam, LPARAM lParam);

		// Functions that are executed when buttons pressed
		OnStartButtonListener onStartListener;
		OnStopButtonListener onStopListener;
	};

}