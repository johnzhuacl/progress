#pragma once
#include "Progress.h"

// CProgressDialog dialog
class MyProgressStatus :public ProgressStatus {
public:
  int rec = 0;
  TCHAR* module = nullptr;
};

class ProgressThreadMapper: public ProgressModel {
protected:
  uintptr_t _thread;
  ProgressModel& _real;
  bool userCancelled;
public:
   ProgressThreadMapper(ProgressModel& model):_real(model), userCancelled(false) {
  }
   virtual ~ProgressThreadMapper() {
   }
public:
  virtual ProgressStatus& getStatus() {
    return _real.getStatus();
  };
  virtual void singalUserCancel() {
    userCancelled = true;
    return _real.singalUserCancel();
  }
  virtual void startProcess();
  virtual int getResult() {
    if (userCancelled)
      return 99;
    return _real.getResult();
  }
  virtual void setController(ProgressController* controller) {
    _real.setController(controller);
  }
  static  unsigned __stdcall runMethod(LPVOID pParam);
};


class GUIProgressController : public ProgressController {
protected:
  ProgressThreadMapper* wrapper;
public:
  GUIProgressController(ProgressModel* const model, ProgressView* const view);
  virtual ~GUIProgressController() {
    delete wrapper;
  }

protected:
  virtual ProgressModel* createModel(ProgressModel& model)  {
    wrapper = new ProgressThreadMapper(model);
    return wrapper;
  }
};
class DummyProgressModel : public ProgressModel {
protected:
  MyProgressStatus status;
  public:
    virtual ~DummyProgressModel() {

    }
    DummyProgressModel() {
      status.module = _T("first");
    }
    virtual ProgressStatus& getStatus() {
      return status;
    }
   
    virtual void startProcess() {
      while (!_cancelled) {
        Sleep(67);
        status.rec++;
        if (status.rec == 10) {
          status.module = _T("second");
        }
        if (status.rec == 20) {
          status.module = _T("third");
        }
        if (status.rec >= 30)
          break;
      }
      done();
    }
    virtual int getResult() {
      if (_cancelled) {
        return CANCELED;
      }
      return OK;
    }
};

class GUIProgressFactory :public ProgressFactory {
public:
  virtual ~GUIProgressFactory() {
    _instance = nullptr;
  }
  GUIProgressFactory() {
    setInstance(this);
  }
  virtual std::unique_ptr<ProgressView> getView(int id);
  virtual std::unique_ptr<ProgressController> getController(int id, ProgressModel* model, ProgressView* view) {
    return std::make_unique<GUIProgressController>(model, view);
  }
};