#include "ObjectModel.hxx"



ObjectModel::ObjectModel(QString const& ObjectName, QObject* p_parent):
  QStandardItemModel(p_parent),
  m_objectsItem(new QStandardItem(ObjectName)) {

  appendRow(m_objectsItem);
}

ObjectModel::~ObjectModel() {
  for (auto* object: m_objectsList) {
    delete object;
  }
}





MirrorModel::MirrorModel(QObject* p_parent):
  ObjectModel("Mirrors", p_parent) {

  setColumnCount(4);
}

MirrorModel::~MirrorModel() = default;

void MirrorModel::AddMirror(Mirror const& p_mirror) {
  auto mirror = new Mirror(p_mirror);
  m_mirrorsList << mirror;
  AddObject(mirror);

  auto mirrorItem = new QStandardItem(tr("%1 #%2").arg(p_mirror.GetName().c_str()).arg(m_mirrorsList.size()));
  auto line = p_mirror.GetLine();
  mirrorItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(line.GetA().GetX(), 'f', 0))
    << new QStandardItem(QString::number(line.GetA().GetY(), 'f', 0))
    << new QStandardItem(QString::number(line.GetB().GetX(), 'f', 0))
    << new QStandardItem(QString::number(line.GetB().GetY(), 'f', 0)));
  m_objectsItem->appendRow(QList<QStandardItem*>() << mirrorItem << new QStandardItem << new QStandardItem << new QStandardItem);
}

void MirrorModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eMirror);
  auto newMirror = static_cast<Mirror*>(p_object);

  //Update Mirror
  auto mirror = m_mirrorsList.at(p_index);
  mirror->SetLine(newMirror->GetLine());

  // Update Mirror Item
  auto mirrorItem = m_objectsItem->child(p_index, 0);
  auto line = mirror->GetLine();
  mirrorItem->child(0, 0)->setText(QString::number(line.GetA().GetX(), 'f', 0));
  mirrorItem->child(0, 1)->setText(QString::number(line.GetA().GetY(), 'f', 0));
  mirrorItem->child(0, 2)->setText(QString::number(line.GetB().GetX(), 'f', 0));
  mirrorItem->child(0, 3)->setText(QString::number(line.GetB().GetY(), 'f', 0));
}



PortalModel::PortalModel(QObject* p_parent):
  ObjectModel("Portals", p_parent) {

  setColumnCount(4);
}

PortalModel::~PortalModel() = default;

void PortalModel::AddPortal(Portal const& p_portal) {
  auto portal = new Portal(p_portal);
  m_portalsList << portal;
  AddObject(portal);

  auto entrance = p_portal.GetIn();
  auto entranceItem = new QStandardItem(tr("Entrance"));
  entranceItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(entrance.GetA().GetX(), 'f', 0))
    << new QStandardItem(QString::number(entrance.GetA().GetY(), 'f', 0))
    << new QStandardItem(QString::number(entrance.GetB().GetX(), 'f', 0))
    << new QStandardItem(QString::number(entrance.GetB().GetY(), 'f', 0)));

  auto entranceNormal = p_portal.GetNormalIn();
  auto entranceNormalItem = new QStandardItem(tr("Entrance Normal"));
  entranceNormalItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(entranceNormal.GetX(), 'f', 3))
    << new QStandardItem(QString::number(entranceNormal.GetY(), 'f', 3))
    << new QStandardItem
    << new QStandardItem);

  auto exit = p_portal.GetOut();
  auto exitItem = new QStandardItem(tr("Exit"));
  exitItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(exit.GetA().GetX(), 'f', 0))
    << new QStandardItem(QString::number(exit.GetA().GetY(), 'f', 0))
    << new QStandardItem(QString::number(exit.GetB().GetX(), 'f', 0))
    << new QStandardItem(QString::number(exit.GetB().GetY(), 'f', 0)));

  auto exitNormal = p_portal.GetNormalOut();
  auto exitNormalItem = new QStandardItem(tr("Exit Normal"));
  exitNormalItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(exitNormal.GetX(), 'f', 3))
    << new QStandardItem(QString::number(exitNormal.GetY(), 'f', 3))
    << new QStandardItem
    << new QStandardItem);

  auto portalItem = new QStandardItem(tr("%1 #%2").arg(p_portal.GetName().c_str()).arg(m_portalsList.size()));
  portalItem->appendRow(QList<QStandardItem*>() << entranceItem << new QStandardItem << new QStandardItem << new QStandardItem);
  portalItem->appendRow(QList<QStandardItem*>() << entranceNormalItem << new QStandardItem << new QStandardItem << new QStandardItem);
  portalItem->appendRow(QList<QStandardItem*>() << exitItem << new QStandardItem << new QStandardItem << new QStandardItem);
  portalItem->appendRow(QList<QStandardItem*>() << exitNormalItem << new QStandardItem << new QStandardItem << new QStandardItem);

  m_objectsItem->appendRow(QList<QStandardItem*>() << portalItem << new QStandardItem << new QStandardItem << new QStandardItem);
}

void PortalModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::ePortal);
  auto newPortal = static_cast<Portal*>(p_object);

  // Update Portal
  auto portal = m_portalsList.at(p_index);
  portal->SetIn(newPortal->GetIn());
  portal->SetOut(newPortal->GetOut());

  // Update Portal Item
  auto portalItem = m_objectsItem->child(p_index, 0);
  auto inLine = portal->GetIn();
  auto inItem = portalItem->child(0, 0);
  inItem->child(0, 0)->setText(QString::number(inLine.GetA().GetX(), 'f', 0));
  inItem->child(0, 1)->setText(QString::number(inLine.GetA().GetY(), 'f', 0));
  inItem->child(0, 2)->setText(QString::number(inLine.GetB().GetX(), 'f', 0));
  inItem->child(0, 3)->setText(QString::number(inLine.GetB().GetY(), 'f', 0));
  auto normalIn = portal->GetNormalIn();
  auto normalInItem = portalItem->child(1, 0);
  normalInItem->child(0, 0)->setText(QString::number(normalIn.GetX(), 'f', 0));
  normalInItem->child(0, 1)->setText(QString::number(normalIn.GetY(), 'f', 0));
  auto outLine = portal->GetOut();
  auto outItem = portalItem->child(2, 0);
  outItem->child(0, 0)->setText(QString::number(outLine.GetA().GetX(), 'f', 0));
  outItem->child(0, 1)->setText(QString::number(outLine.GetA().GetY(), 'f', 0));
  outItem->child(0, 2)->setText(QString::number(outLine.GetB().GetX(), 'f', 0));
  outItem->child(0, 3)->setText(QString::number(outLine.GetB().GetY(), 'f', 0));
  auto normalOut = portal->GetNormalOut();
  auto normalOutItem = portalItem->child(3, 0);
  normalOutItem->child(0, 0)->setText(QString::number(normalOut.GetX(), 'f', 0));
  normalOutItem->child(0, 1)->setText(QString::number(normalOut.GetY(), 'f', 0));
}



CountdownModel::CountdownModel(QObject* p_parent):
  ObjectModel("Countdowns", p_parent) {

  setColumnCount(3);
}

CountdownModel::~CountdownModel() = default;

void CountdownModel::AddCountdown(Countdown const& p_countdown) {
  auto countdown = new Countdown(p_countdown);
  m_countdownsList << countdown;
  AddObject(countdown);

  auto counter = p_countdown.GetCounter();
  auto position = p_countdown.GetPosition();

  auto countdownItem = new QStandardItem(tr("%1 #%2").arg(p_countdown.GetName().c_str()).arg(m_countdownsList.size()));
  countdownItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(counter))
    << new QStandardItem(QString::number(position.GetX(), 'f', 0))
    << new QStandardItem(QString::number(position.GetY(), 'f', 0)));

  m_objectsItem->appendRow(QList<QStandardItem*>() << countdownItem << new QStandardItem << new QStandardItem);
}

void CountdownModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eCountdown);
//  auto newCountdown = static_cast<Countdown*>(p_object);

//  auto countdown = m_countdownsList.at(p_index);
//  countdown->SetPosition(newCountdown->GetPosition());

  /// UPDATE STANDARD ITEMS
}



DisposableModel::DisposableModel(QObject* p_parent):
  ObjectModel("Disposables", p_parent) {

  setColumnCount(2);
}

DisposableModel::~DisposableModel() = default;

void DisposableModel::AddDisposable(Disposable const& p_disposable) {
  auto disposable = new Disposable(p_disposable);
  m_disposablesList << disposable;
  AddObject(disposable);

  auto position = p_disposable.GetPosition();

  auto disposableItem = new QStandardItem(tr("%1 #%2").arg(p_disposable.GetName().c_str()).arg(m_disposablesList.size()));
  disposableItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position.GetX(), 'f', 0))
    << new QStandardItem(QString::number(position.GetY(), 'f', 0)));

  m_objectsItem->appendRow(QList<QStandardItem*>() << disposableItem << new QStandardItem);
}

void DisposableModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eDisposable);
//  auto newDisposable = static_cast<Disposable*>(p_object);

//  auto disposable = m_disposablesList.at(p_index);
//  disposable->SetPosition(newDsposable->GetPosition());

  /// UPDATE STANDARD ITEMS
}



SwitchModel::SwitchModel(QObject* p_parent):
  ObjectModel("Switches", p_parent) {

  setColumnCount(2);
}

SwitchModel::~SwitchModel() = default;

void SwitchModel::AddSwitch(Switch const& p_switch) {
  auto switche = new Switch(p_switch);
  m_switchesList << switche;
  AddObject(switche);

  auto position = p_switch.GetPosition();
  auto switchItem = new QStandardItem(tr("%1 #%2").arg(p_switch.GetName().c_str()).arg(m_switchesList.size()));
  switchItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position.GetX(), 'f', 0))
    << new QStandardItem(QString::number(position.GetY(), 'f', 0)));

  m_objectsItem->appendRow(QList<QStandardItem*>() << switchItem << new QStandardItem);
}

void SwitchModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eSwitch);
//  auto newSwitch = static_cast<Switch*>(p_object);

//  auto switche = m_switchesList.at(p_index);
//  switche->SetPosition(newSwitch->GetPosition());

  /// UPDATE STANDARD ITEMS
}



TransferModel::TransferModel(QObject* p_parent):
  ObjectModel("Transfers", p_parent) {

  setColumnCount(2);
}

TransferModel::~TransferModel() = default;

void TransferModel::AddTransfer(Transfer const& p_transfer) {
  auto transfer = new Transfer(p_transfer);
  m_transfersList << transfer;
  AddObject(transfer);

  auto position1 = p_transfer.GetPosition();
  auto position1Item = new QStandardItem(tr("Position #1"));
  position1Item->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position1.GetX(), 'f', 0))
    << new QStandardItem(QString::number(position1.GetY(), 'f', 0)));

  auto position2 = p_transfer.GetPosition2();
  auto position2Item = new QStandardItem(tr("Position #2"));
  position2Item->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position2.GetX(), 'f', 0))
    << new QStandardItem(QString::number(position2.GetY(), 'f', 0)));

  auto transferItem = new QStandardItem(tr("%1 #%2").arg(p_transfer.GetName().c_str()).arg(m_transfersList.size()));
  transferItem->appendRow(QList<QStandardItem*>() << position1Item << new QStandardItem);
  transferItem->appendRow(QList<QStandardItem*>() << position2Item << new QStandardItem);

  m_objectsItem->appendRow(QList<QStandardItem*>() << transferItem << new QStandardItem);
}

void TransferModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eTransfer);
//  auto newTransfer = static_cast<Transfer*>(p_object);

//  auto transfer = m_transfersList.at(p_index);
//  transfer->SetPosition(newTransfer->GetPosition());
//  transfer->SetPosition2(newTransfer->GetPosition2());

  /// UPDATE STANDARD ITEMS
}



OneWayModel::OneWayModel(QObject* p_parent):
  ObjectModel("One Ways", p_parent) {

  setColumnCount(4);
}

OneWayModel::~OneWayModel() = default;

void OneWayModel::AddOneWay(OneWay const& p_oneWay) {
  auto oneWay = new OneWay(p_oneWay);
  m_oneWaysList << oneWay;
  AddObject(oneWay);

  auto line = p_oneWay.GetLine();
  auto lineItem = new QStandardItem(tr("Line"));
  lineItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(line.GetA().GetX(), 'f', 0))
    << new QStandardItem(QString::number(line.GetA().GetY(), 'f', 0))
    << new QStandardItem(QString::number(line.GetB().GetX(), 'f', 0))
    << new QStandardItem(QString::number(line.GetB().GetY(), 'f', 0)));

  auto direction = p_oneWay.GetDirection();
  auto directionItem = new QStandardItem(tr("Direction"));
  directionItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(direction.GetX(), 'f', 3))
    << new QStandardItem(QString::number(direction.GetY(), 'f', 3))
    << new QStandardItem
    << new QStandardItem);

  auto oneWayItem = new QStandardItem(tr("%1 #%2").arg(p_oneWay.GetName().c_str()).arg(m_oneWaysList.size()));
  oneWayItem->appendRow(QList<QStandardItem*>() << lineItem << new QStandardItem << new QStandardItem << new QStandardItem);
  oneWayItem->appendRow(QList<QStandardItem*>() << directionItem << new QStandardItem << new QStandardItem << new QStandardItem);

  m_objectsItem->appendRow(QList<QStandardItem*>() << oneWayItem << new QStandardItem << new QStandardItem << new QStandardItem);
}

void OneWayModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eOneWay);
  auto newOneWay = static_cast<OneWay*>(p_object);

  // Update OneWay
  auto oneWay = m_oneWaysList.at(p_index);
  oneWay->SetLine(newOneWay->GetLine());

  // Update OneWay Item
  auto oneWayItem = m_objectsItem->child(p_index, 0);
  auto lineItem = oneWayItem->child(0, 0);
  auto line = oneWay->GetLine();
  lineItem->child(0, 0)->setText(QString::number(line.GetA().GetX(), 'f', 0));
  lineItem->child(0, 1)->setText(QString::number(line.GetA().GetY(), 'f', 0));
  lineItem->child(0, 2)->setText(QString::number(line.GetB().GetX(), 'f', 0));
  lineItem->child(0, 3)->setText(QString::number(line.GetB().GetY(), 'f', 0));
  auto directionItem = oneWayItem->child(1, 0);
  auto direction = oneWay->GetDirection();
  directionItem->child(0, 0)->setText(QString::number(direction.GetX(), 'f', 3));
  directionItem->child(0, 1)->setText(QString::number(direction.GetY(), 'f', 3));
}



TapeModel::TapeModel(QObject* p_parent):
  ObjectModel("Tapes", p_parent) {

  setColumnCount(4);
}

TapeModel::~TapeModel() = default;

void TapeModel::AddTape(Tape const& p_tape) {
  auto tape = new Tape(p_tape);
  m_tapesList << tape;
  AddObject(tape);

  auto left = p_tape.GetX();
  auto top = p_tape.GetY();
  auto right = left + p_tape.GetW();
  auto bottom = top + p_tape.GetH();
  auto tapeItem = new QStandardItem(tr("%1 #%2").arg(p_tape.GetName().c_str()).arg(m_tapesList.size()));
  tapeItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(left, 'f', 0))
    << new QStandardItem(QString::number(top, 'f', 0))
    << new QStandardItem(QString::number(right, 'f', 0))
    << new QStandardItem(QString::number(bottom, 'f', 0)));

  m_objectsItem->appendRow(QList<QStandardItem*>() << tapeItem << new QStandardItem << new QStandardItem << new QStandardItem);
}

void TapeModel::SetObject(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eTape);
  auto newTape = static_cast<Tape*>(p_object);

  // Update Tape
  auto tape = m_tapesList.at(p_index);
  tape->SetX(newTape->GetX());
  tape->SetY(newTape->GetY());
  tape->SetW(newTape->GetW());
  tape->SetH(newTape->GetH());

  // Update Tape Item
  auto tapeItem = m_objectsItem->child(p_index, 0);
  tapeItem->child(0, 0)->setText(QString::number(tape->GetX(), 'f', 0));
  tapeItem->child(0, 1)->setText(QString::number(tape->GetY(), 'f', 0));
  tapeItem->child(0, 2)->setText(QString::number(tape->GetX()+tape->GetW(), 'f', 0));
  tapeItem->child(0, 3)->setText(QString::number(tape->GetY()+tape->GetH(), 'f', 0));
}
