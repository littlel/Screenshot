
// screen_testDlg.h : ͷ�ļ�
//

#pragma once
#include "MakePNG.h"





// Cscreen_testDlg �Ի���
class Cscreen_testDlg : public CDialogEx
{
// ����
public:
	Cscreen_testDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCREEN_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
