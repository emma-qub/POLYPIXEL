#ifndef OBJECTMODEL_HXX
#define OBJECTMODEL_HXX

#include <QStandardItemModel>

#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Deviations/Portal.hxx"
#include "Core/Objects/Mutables/Countdown.hxx"
#include "Core/Objects/Mutables/Disposable.hxx"
#include "Core/Objects/Mutables/Switch.hxx"
#include "Core/Objects/Mutables/Transfer.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Obstacles/Tape.hxx"

#include <QStandardItemModel>

class ObjectModel: public QStandardItemModel {

public:
  enum States {
    eSelected,
    eEnabled,
    eDisabled,
    eHighlightUp,
    eHighlightDown
  };

  enum ItemRoles {
    eStateRole = Qt::UserRole+1
  };

  enum ModelType {
    eTapeModel,
    eMirrorModel,
    eOneWayModel,
    ePortalModel
  };

  ObjectModel(const QString& ObjectName, QObject* p_parent = nullptr);
  ~ObjectModel() override;

  virtual void SetObject(int p_index, Object* p_object) = 0;

  inline QList<Object*> GetObjectsList() const { return m_objectsList; }
  inline QStandardItem* GetItemFromObject(Object* p_object) const { return m_objectItemMap[p_object]; }

protected:
  inline void AddObject(Object* p_object) { m_objectsList << p_object; }
  inline void RegisterObject(Object* p_object, QStandardItem* p_item) { m_objectItemMap[p_object] = p_item; }

  QStandardItem* m_objectsItem;

private:
  QList<Object*> m_objectsList;
  QMap<Object*, QStandardItem*> m_objectItemMap;
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
