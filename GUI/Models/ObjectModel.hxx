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

class MirrorModel: public QStandardItemModel {

public:
  MirrorModel(QObject* p_parent = nullptr);
  ~MirrorModel() override;

  void AddMirror(Mirror const& p_mirror);
  inline QList<Object*> GetMirrorsList() const { return m_mirrorsList; }

private:
  QStandardItem* m_mirrorsItem;
  QList<Object*> m_mirrorsList;
};



class PortalModel: public QStandardItemModel {

public:
  PortalModel(QObject* p_parent = nullptr);
  ~PortalModel() override;

  void AddPortal(Portal const& p_portal);
  inline QList<Object*> GetPortalsList() const { return m_portalsList; }

private:
  QStandardItem* m_portalsItem;
  QList<Object*> m_portalsList;
};



class CountdownModel: public QStandardItemModel {

public:
  CountdownModel(QObject* p_parent = nullptr);
  ~CountdownModel() override;

  void AddCountdown(Countdown const& p_countdown);
  inline QList<Object*> GetCountdownsList() const { return m_countdownsList; }

private:
  QStandardItem* m_countdownsItem;
  QList<Object*> m_countdownsList;
};



class DisposableModel: public QStandardItemModel {

public:
  DisposableModel(QObject* p_parent = nullptr);
  ~DisposableModel() override;

  void AddDisposable(Disposable const& p_disposable);
  inline QList<Object*> GetDisposablesList() const { return m_disposablesList; }

private:
  QStandardItem* m_disposablesItem;
  QList<Object*> m_disposablesList;
};



class SwitchModel: public QStandardItemModel {

public:
  SwitchModel(QObject* p_parent = nullptr);
  ~SwitchModel() override;

  void AddSwitch(Switch const& p_switch);
  inline QList<Object*> GetSwitchesList() const { return m_switchesList; }

private:
  QStandardItem* m_switchesItem;
  QList<Object*> m_switchesList;
};



class TransferModel: public QStandardItemModel {

public:
  TransferModel(QObject* p_parent = nullptr);
  ~TransferModel() override;

  void AddTransfer(Transfer const& p_transfer);
  inline QList<Object*> GetTransfersList() const { return m_transfersList; }

private:
  QStandardItem* m_transfersItem;
  QList<Object*> m_transfersList;
};



class OneWayModel: public QStandardItemModel {

public:
  OneWayModel(QObject* p_parent = nullptr);
  ~OneWayModel() override;

  void AddOneWay(OneWay const& p_oneWay);
  inline QList<Object*> GetOneWaysList() const { return m_oneWaysList; }

private:
  QStandardItem* m_oneWaysItem;
  QList<Object*> m_oneWaysList;
};



class TapeModel: public QStandardItemModel {

public:
  TapeModel(QObject* p_parent = nullptr);
  ~TapeModel() override;

  void AddTape(Tape const& p_tape);
  inline QList<Object*> GetTapesList() const { return m_tapesList; }

private:
  QStandardItem* m_tapesItem;
  QList<Object*> m_tapesList;
};


#endif
