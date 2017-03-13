
// screen_testDlg.cpp : 实现文件
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
// 全局变量和全局函数定义  
//  
HHOOK hhookMs = NULL;  
LRESULT CALLBACK LowLevelMouseProc (INT nCode, WPARAM wParam, LPARAM lParam);  
BOOL UninstallKbHook();  
BOOL InstallKbHook(); 

 HANDLE shot_event;


// Cscreen_testDlg 对话框


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


// Cscreen_testDlg 消息处理程序

BOOL Cscreen_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cscreen_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	//获取窗口的大小    
	pDesktop->GetClientRect(&rect);  

	//保存到的文件名  
	CString strFileName(GetAppPathW().c_str());  
	strFileName += _T("ScreenShot\\");  
	CreateDirectory((LPCTSTR)strFileName,NULL);  
	CTime t = CTime::GetCurrentTime();  
	CString tt = t.Format("%Y%m%d_%H%M%S");  
	strFileName += tt;  
	strFileName += _T(".PNG");  
	//保存为PNG  
	CMakePNG MakePNG;  
	MakePNG.MakePNG(pDC->m_hDC,rect,strFileName);  
	pDesktop->ReleaseDC(pDC);  
	return strFileName;  
}

void Cscreen_testDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ttt = ScreenShot();
	AfxMessageBox(ttt);

}



//鼠标事件钩子
LRESULT CALLBACK LowLevelMouseProc (INT nCode, WPARAM wParam, LPARAM lParam)  
{  
	MSLLHOOKSTRUCT *pkbhs = (MSLLHOOKSTRUCT *)lParam;  
	char strMsg[100] = {0};  

	switch (nCode)  
	{  
	case HC_ACTION:  
		{  
			//鼠标移动  
			if (wParam == WM_MOUSEMOVE)   
			{  
				sprintf(strMsg, "WM_MOUSEMOVE: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);  
				//OutputDebugString(strMsg);  
			}  

			//鼠标左击  
			if(wParam == WM_LBUTTONDOWN)  
			{  
				sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);  
				//OutputDebugString(strMsg);  
			}  
			//鼠标右击
			if(wParam == WM_RBUTTONDOWN)
			{
				sprintf(strMsg, "WM_LBUTTONDOWN: x= %d, y= %d\n", pkbhs->pt.x, pkbhs->pt.y);  
				//OutputDebugString(strMsg);  
			}
			//滚轮事件  
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
	// TODO: 在此添加控件通知处理程序代码
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