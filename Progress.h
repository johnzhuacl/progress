#pragma once
#include <thread>

class ProgressListener {
  virtual ~ProgressListener() {};
  virtual void onStatusChange() = 0;
};
class ProgressStatus {

};
class ProgressController;

class ProgressModel {

protected:
  ProgressController * _controller;
  BOOL _cancelled = false;
public:
  const unsigned CANCELED = 99;
  const unsigned OK = 0;
  virtual ProgressStatus& getStatus() = 0;
  virtual void singalUserCancel() {
    _cancelled = true;
  }
  virtual void startProcess() = 0;
  virtual int getResult() = 0;
  virtual void setController(ProgressController* controller) {
    _controller = controller;
  }
  virtual void done();
};
typedef std::wstring (*formatter_func)(ProgressStatus& status) ;
class ProgressView {
protected:
  ProgressController* _controller;
  ProgressModel* _model;
  formatter_func _formatter;
public:
  ProgressView(ProgressController* controller, ProgressModel* model) : _controller(controller), _model(model)
  {
  }
  ProgressView() 
  {
  }
  void virtual setController(ProgressController* controller) {
    _controller = controller;
  }
  void virtual setModel(ProgressModel* model) {
    _model = model;
  }
  void virtual setFormmater(formatter_func formatt) {
    _formatter = formatt;
  };
  void virtual handleCancel();
  void virtual onFinish();
  virtual int ShowView();
};

class ProgressController {
protected:
  ProgressModel* const _model;
  ProgressView * const _view;
public:
  ProgressController(ProgressModel* const model, ProgressView* const view) 
    : _model(model), _view(view)
  {
    _view->setController(this);
    _view->setModel(_model);
    _model->setController(this);
  }
  virtual ~ProgressController() {
    
  }
 
  int virtual run() {
    _view->ShowView();
    return _model->getResult();
  }

  virtual void onCancel() {
    _model->singalUserCancel();
  }
  virtual void onViewReady() {
    _model->startProcess();
  }
  virtual void onModelDone() {
    _view->onFinish();
  }
};

class ProgressFactory {

protected:
  static ProgressFactory* _instance;
  
  void setInstance(ProgressFactory* instance) {
    _instance = instance;
  }

public:
  virtual ~ProgressFactory() {

  }
  static ProgressFactory* instance() {
    return _instance;
  }
  virtual std::unique_ptr<ProgressView> getView(int id) = 0;
  virtual std::unique_ptr<ProgressController> getController(int id, ProgressModel* model, ProgressView* view) = 0;
};
class EngineProgressFactory: public ProgressFactory {

public:
  EngineProgressFactory() {
    _instance = this;
  }
  virtual ~EngineProgressFactory() {
    _instance = nullptr;
  }
  virtual std::unique_ptr<ProgressView> getView(int id) {
    return std::make_unique<ProgressView>();
  }
  virtual std::unique_ptr<ProgressController> getController(int id, ProgressModel* model, ProgressView* view) {
    return std::make_unique<ProgressController>(model, view);
  }
};