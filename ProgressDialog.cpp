// ProgressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "ProgressDialog.h"

ProgressFactory* ProgressFactory::_instance = nullptr;
std::wstring formatter(ProgressStatus& status) {
  MyProgressStatus& st = static_cast<MyProgressStatus&>(status);
  TCHAR msg[200];
  wsprintf(msg, _T("Processing module %s\r\nRecord %d"), st.module, st.rec);
  return msg;
}



// CProgressDialog message handlers
void ProgressThreadMapper::startProcess() {
  _thread = _beginthreadex(0,0,runMethod, (LPVOID) this,0,0);
  
}

unsigned  ProgressThreadMapper::runMethod(LPVOID pParam) {
  ProgressThreadMapper* p = (ProgressThreadMapper*)pParam;
  p->_real.startProcess();
  return 0;
}

GUIProgressController::GUIProgressController(ProgressModel* const model, ProgressView* const view)
: ProgressController(createModel(*model), view)
{
  view->setFormmater(formatter);
}