#pragma once

#include "Progress.h"
// CDialogTest dialog

class CDialogTest : public CDialogEx, public ProgressView
{
	DECLARE_DYNAMIC(CDialogTest)

public:
	CDialogTest(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogTest();
  void virtual onFinish();
  virtual int ShowView();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnBnClickedCancel();
};

class GUIProgressView : public ProgressView {
  CDialogTest dialog;
public:
  GUIProgressView() {};
  virtual ~GUIProgressView() {};
  void virtual setController(ProgressController* controller) {
    dialog.setController(controller);
  }
  void virtual setModel(ProgressModel* model) {
    dialog.setModel(model);
  }
  void virtual setFormmater(formatter_func formatt) {
    dialog.setFormmater(formatt);
  };
  void virtual handleCancel() {
    dialog.handleCancel();
  }
  void virtual onFinish() {
    dialog.onFinish();
  
  }
  virtual int ShowView() {
    return dialog.ShowView();

  }
};