
// screen_testDlg.h : 头文件
//

#pragma once
#include "MakePNG.h"





// Cscreen_testDlg 对话框
class Cscreen_testDlg : public CDialogEx
{
// 构造
public:
	Cscreen_testDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREEN_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	static CString ScreenShot(void);
public:
	afx_msg void OnBnClickedButton1();

   static unsigned _stdcall save_pic(LPVOID lp);

   afx_msg void OnBnClickedButton2();
};
