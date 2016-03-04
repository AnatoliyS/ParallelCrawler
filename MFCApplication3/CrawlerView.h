
// CrawlerView.h : header file
//

#pragma once
#include "afxcmn.h"

namespace crawler {

	// CrawlerView dialog
	class CrawlerView : public CDialogEx
	{
		// Construction
	public:
		CrawlerView(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
	protected:
		HICON m_hIcon;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	private:
		CListCtrl mTaskList;
	};

}