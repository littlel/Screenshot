
// screen_testDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "screen_test.h"
#include "screen_testDlg.h"
#include "afxdialogex.h"

#include <iostream>   
#include <string>   
using namespace std; 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//  
// ȫ�ֱ�����ȫ�ֺ�������  
//  
HHOOK hhookMs = NULL;  
LRESULT CALLBACK LowLevelMouseProc (INT nCode, WPARAM wParam, LPARAM lParam);  
BOOL UninstallKbHook();  
BOOL InstallKbHook(); 

 HANDLE shot_event;


// Cscreen_testDlg �Ի���


Cscreen_testDlg::Cscreen_testDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cscreen_testDlg::IDD, pParent)
{
	shot_event = CreateEvent(NULL,false,false,NULL);
	_beginthreadex(nullptr,NULL,save_pic,this,NULL,nullptr);
	InstallKbHook();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cscreen_testDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cscreen_testDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cscreen_testDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cscreen_testDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cscreen_testDlg ��Ϣ�������

BOOL Cscreen_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cscreen_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cscreen_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


wstring GetAppPathW()  
{  
	wchar_t szExePath[MAX_PATH] = {0};  
	GetModuleFileNameW(NULL, szExePath, MAX_PATH);  
	wchar_t *pstr = wcsrchr(szExePath, '\\');  
	memset(pstr + 1, 0, 2);  
	wstring strAppPath(szExePath);  
	return strAppPath;  
}  

CString Cscreen_testDlg::ScreenShot(void)
{
	CWnd *pDesktop = GetDesktopWindow();    
	CDC *pDC = pDesktop->GetDC();    
	CRect rect;    
	//��ȡ���ڵĴ�С    
	pDesktop->GetClientRect(&rect);  

	//���浽���ļ���  
	CString strFileName(GetAppPathW().c_str());  
	strFileName += _T("ScreenShot\\");  
	CreateDirectory((LPCTSTR)strFileName,NULL);  
	CTime t = CTime::GetCurrentTime();  
	CString tt = t.Format("%Y%m%d_%H%M%S");  
	strFileName += tt;  
	strFileName += _T(".PNG");  
	//����ΪPNG  
	CMakePNG MakePNG;  
	MakePNG.MakePNG(pDC->m_hDC,rect,strFileName);  
	pDesktop->ReleaseDC(pDC);  
	return strFileName;  
}

void Cscreen_testDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString ttt = ScreenShot();
	AfxMessageBox(ttt);

}



//����¼�����
LRESULT CALLBACK LowLevelMouseProc (INT nCode, WPARAM wParam, LPARAM lParam)  
{  
	MSLLHOOKSTRUCT *pkbhs = (MSLLHOOKSTRUCT *)lParam;  
	char strMsg[100] = {0};  

	switch (nCode)  
	{  
	case HC_ACTION:  
		{  
			//����ƶ�  
			if (wParam == WM_MOUSEMOVE)   
			{  
				sprintf(strMsg, "WM_MOUSEMOVE: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);  
				//OutputDebugString(strMsg);  
			}  

			//������  
			if(wParam == WM_LBUTTONDOWN)  
			{  
				sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);  
				//OutputDebugString(strMsg);  
			}  
			//����һ�
			if(wParam == WM_RBUTTONDOWN)
			{
				sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);  
				//OutputDebugString(strMsg);  
			}
			//�����¼�  
			if (wParam == WM_MOUSEWHEEL)  
			{  
				sprintf(strMsg, "WM_MOUSEWHEEL: %d\n", HIWORD(pkbhs->mouseData));  
				//OutputDebugString(strMsg);  
			}  
			SetEvent(shot_event);
		}  
	default:  
		break;  
	}  
	return CallNextHookEx (NULL, nCode, wParam, lParam);  
}  

BOOL InstallKbHook( )  
{  
	if (hhookMs )  
		UninstallKbHook();  

	hhookMs = SetWindowsHookEx(WH_MOUSE_LL,   
		(HOOKPROC)LowLevelMouseProc, AfxGetApp()->m_hInstance, NULL);  

	return(hhookMs != NULL);  

}  

BOOL UninstallKbHook()  
{  
	BOOL fOk = FALSE;  
	if (hhookMs ) {  
		fOk = UnhookWindowsHookEx(hhookMs );  
		hhookMs = NULL;  
	}  

	return(fOk);  
}  

void Cscreen_testDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UninstallKbHook();
}


unsigned _stdcall Cscreen_testDlg::save_pic(LPVOID lp)
{
	Cscreen_testDlg *pthis = (Cscreen_testDlg*)lp;

	while (1)
	{
		int iret  = WaitForSingleObject(shot_event,INFINITE);
		if(iret != WAIT_OBJECT_0)
		{
			char log[256] = {0};
			sprintf_s(log, 256, "WaitForSingleObject fail 2!%d", GetLastError());
		}
		else
		{
			ScreenShot();
			Sleep(2);
			continue;
		}
	}
	return 0;
}