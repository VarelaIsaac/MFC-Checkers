#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFC Checkers.h"
#include "MainFrm.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCCheckersApp

BEGIN_MESSAGE_MAP(CMFCCheckersApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CMFCCheckersApp::OnAppAbout)
END_MESSAGE_MAP()

CMFCCheckersApp::CMFCCheckersApp() noexcept {
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
    System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
    SetAppID(_T("MFCCheckers.AppID.NoVersion"));
}

CMFCCheckersApp theApp;

BOOL CMFCCheckersApp::InitInstance() {
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    if (!AfxOleInit()) {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();
    EnableTaskbarInteraction(FALSE);
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    CFrameWnd* pFrame = new CMainFrame;
    if (!pFrame)
        return FALSE;
    m_pMainWnd = pFrame;
    pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr, nullptr);

    CChildView* pView = new CChildView;
    if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), pFrame, AFX_IDW_PANE_FIRST, NULL))
        return FALSE;

    pFrame->SetWindowText(_T("Checkers Game"));
    pFrame->ShowWindow(SW_SHOWMAXIMIZED);
    pFrame->UpdateWindow();
    return TRUE;
}

int CMFCCheckersApp::ExitInstance() {
    AfxOleTerm(FALSE);
    return CWinApp::ExitInstance();
}

class CAboutDlg : public CDialogEx {
public:
    CAboutDlg() noexcept;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void CMFCCheckersApp::OnAppAbout() {
    CAboutDlg aboutDlg;
}
