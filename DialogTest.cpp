// DialogTest.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "ProgressDialog.h"
#include "DialogTest.h"
#include "afxdialogex.h"


// CDialogTest dialog
#define PROGRESS_TIMER  1000
IMPLEMENT_DYNAMIC(CDialogTest, CDialogEx)

CDialogTest::CDialogTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent), ProgressView()
{

}

CDialogTest::~CDialogTest()
{
}

void CDialogTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogTest, CDialogEx)
  ON_WM_CREATE()
  ON_WM_TIMER()
  ON_BN_CLICKED(ID_CANCEL, &CDialogTest::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogTest message handlers


int CDialogTest::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  SetTimer(PROGRESS_TIMER, 100, NULL);
  _controller->onViewReady();

  if (CDialogEx::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  Add your specialized creation code here

  return 0;
}

void CDialogTest::onFinish()
{
  ::PostMessage(this->GetSafeHwnd(), WM_CLOSE, NULL, NULL);

}
int CDialogTest::ShowView()
{
  return DoModal();
}

void CDialogTest::OnTimer(UINT_PTR nIDEvent)
{
  CWnd *module = GetDlgItem(IDC_COMP);
  MyProgressStatus status = (MyProgressStatus&)(_model->getStatus());

  CWnd *label = GetDlgItem(ID_DATA);
  label->SetWindowText(_formatter(status).c_str());
  CDialogEx::OnTimer(nIDEvent);
}


void CDialogTest::OnBnClickedCancel()
{
  this->handleCancel();

}

std::unique_ptr<ProgressView> GUIProgressFactory::getView(int id) {
  return std::make_unique<GUIProgressView>();
}