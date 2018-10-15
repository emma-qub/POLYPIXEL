#ifndef OBJECTMODEL_HXX
#define OBJECTMODEL_HXX

#include <QStandardItemModel>

#include "Objects/Deviations/Mirror.hxx"
#include "Objects/Deviations/Portal.hxx"
#include "Objects/Mutables/Countdown.hxx"
#include "Objects/Mutables/Disposable.hxx"
#include "Objects/Mutables/Switch.hxx"
#include "Objects/Mutables/Transfer.hxx"
#include "Objects/Obstacles/OneWay.hxx"
#include "Objects/Obstacles/Tape.hxx"

#include <QStandardItemModel>

class ObjectModel: public QStandardItemModel {

public:
  enum ModelType {
    eTapeModel,
    eMirrorModel,
    eOneWayModel,
    ePortalModel
  };

  ObjectModel(const QString& ObjectName, QObject* p_parent = nullptr);
  ~ObjectModel() override;

  QList<Object*> GetObjectsList() const { return m_objectsList; }
  virtual void SetObject(int p_index, Object* p_object) = 0;

protected:
  inline void AddObject(Object* p_object) { m_objectsList << p_object; }

  QStandardItem* m_objectsItem;

private:
  QList<Object*> m_objectsList;
};

class MirrorModel: public ObjectModel {

public:
  MirrorModel(QObject* p_parent = nullptr);
  ~MirrorModel() override;

  void AddMirror(Mirror const& p_mirror);
  inline QList<Mirror*> GetMirrorsList() const { return m_mirrorsList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<Mirror*> m_mirrorsList;
};



class PortalModel: public ObjectModel {

public:
  PortalModel(QObject* p_parent = nullptr);
  ~PortalModel() override;

  void AddPortal(Portal const& p_portal);
  inline QList<Portal*> GetPortalsList() const { return m_portalsList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<Portal*> m_portalsList;
};



class CountdownModel: public ObjectModel {

public:
  CountdownModel(QObject* p_parent = nullptr);
  ~CountdownModel() override;

  void AddCountdown(Countdown const& p_countdown);
  inline QList<Countdown*> GetCountdownsList() const { return m_countdownsList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<Countdown*> m_countdownsList;
};



class DisposableModel: public ObjectModel {

public:
  DisposableModel(QObject* p_parent = nullptr);
  ~DisposableModel() override;

  void AddDisposable(Disposable const& p_disposable);
  inline QList<Disposable*> GetDisposablesList() const { return m_disposablesList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<Disposable*> m_disposablesList;
};



class SwitchModel: public ObjectModel {

public:
  SwitchModel(QObject* p_parent = nullptr);
  ~SwitchModel() override;

  void AddSwitch(Switch const& p_switch);
  inline QList<Switch*> GetSwitchesList() const { return m_switchesList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<Switch*> m_switchesList;
};



class TransferModel: public ObjectModel {

public:
  TransferModel(QObject* p_parent = nullptr);
  ~TransferModel() override;

  void AddTransfer(Transfer const& p_transfer);
  inline QList<Transfer*> GetTransfersList() const { return m_transfersList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<Transfer*> m_transfersList;
};



class OneWayModel: public ObjectModel {

public:
  OneWayModel(QObject* p_parent = nullptr);
  ~OneWayModel() override;

  void AddOneWay(OneWay const& p_oneWay);
  inline QList<OneWay*> GetOneWaysList() const { return m_oneWaysList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<OneWay*> m_oneWaysList;
};



class TapeModel: public ObjectModel {

public:
  TapeModel(QObject* p_parent = nullptr);
  ~TapeModel() override;

  void AddTape(Tape const& p_tape);
  inline QList<Tape*> GetTapesList() const { return m_tapesList; }
  void SetObject(int p_index, Object* p_object) override;

private:
  QList<Tape*> m_tapesList;
};


#endif
