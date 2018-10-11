#include "ObjectModel.hxx"



MirrorModel::MirrorModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_mirrorsItem(new QStandardItem("Mirrors")) {

  appendRow(m_mirrorsItem);
}

MirrorModel::~MirrorModel() {
  for (auto* object: m_mirrorsList) {
    delete object;
  }
}

void MirrorModel::AddMirror(Mirror const& p_mirror) {
  m_mirrorsList << new Mirror(p_mirror);

  auto mirrorItem = new QStandardItem(tr("%1 #%2").arg(p_mirror.GetName().c_str()).arg(m_mirrorsList.size()));
  auto mirrorLine = p_mirror.GetMirrorLine();
  mirrorItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(mirrorLine.GetA().GetX()))
    << new QStandardItem(QString::number(mirrorLine.GetA().GetY()))
    << new QStandardItem(QString::number(mirrorLine.GetB().GetX()))
    << new QStandardItem(QString::number(mirrorLine.GetB().GetY())));
  m_mirrorsItem->appendRow(mirrorItem);
}



PortalModel::PortalModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_portalsItem(new QStandardItem("Portals")) {

  appendRow(m_portalsItem);
}

PortalModel::~PortalModel() {
  for (auto* object: m_portalsList) {
    delete object;
  }
}

void PortalModel::AddPortal(Portal const& p_portal) {
  m_portalsList << new Portal(p_portal);

  auto entrance = p_portal.GetIn();
  auto entranceItem = new QStandardItem(tr("Entrance"));
  entranceItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(entrance.GetA().GetX()))
    << new QStandardItem(QString::number(entrance.GetA().GetY()))
    << new QStandardItem(QString::number(entrance.GetB().GetX()))
    << new QStandardItem(QString::number(entrance.GetB().GetY())));

  auto entranceNormal = p_portal.GetNormalIn();
  auto entranceNormalItem = new QStandardItem(tr("Entrance Normal"));
  entranceNormalItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(entranceNormal.GetX()))
    << new QStandardItem(QString::number(entranceNormal.GetY())));

  auto exit = p_portal.GetOut();
  auto exitItem = new QStandardItem(tr("Exit"));
  exitItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(exit.GetA().GetX()))
    << new QStandardItem(QString::number(exit.GetA().GetY()))
    << new QStandardItem(QString::number(exit.GetB().GetX()))
    << new QStandardItem(QString::number(exit.GetB().GetY())));

  auto exitNormal = p_portal.GetNormalOut();
  auto exitNormalItem = new QStandardItem(tr("Exit Normal"));
  exitNormalItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(exitNormal.GetX()))
    << new QStandardItem(QString::number(exitNormal.GetY())));

  auto portalItem = new QStandardItem(tr("%1 #%2").arg(p_portal.GetName().c_str()).arg(m_portalsList.size()));
  portalItem->appendRow(entranceItem);
  portalItem->appendRow(entranceNormalItem);
  portalItem->appendRow(exitItem);
  portalItem->appendRow(exitNormalItem);

  m_portalsItem->appendRow(portalItem);
}



CountdownModel::CountdownModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_countdownsItem(new QStandardItem("Countdowns")) {

  appendRow(m_countdownsItem);
}

CountdownModel::~CountdownModel() {
  for (auto* object: m_countdownsList) {
    delete object;
  }
}

void CountdownModel::AddCountdown(Countdown const& p_countdown) {
  m_countdownsList << new Countdown(p_countdown);

  auto counter = p_countdown.GetCounter();
  auto position = p_countdown.GetPosition();

  auto countdownItem = new QStandardItem(tr("%1 #%2").arg(p_countdown.GetName().c_str()).arg(m_countdownsList.size()));
  countdownItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(counter))
    << new QStandardItem(QString::number(position.GetX()))
    << new QStandardItem(QString::number(position.GetY())));

  m_countdownsItem->appendRow(countdownItem);
}



DisposableModel::DisposableModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_disposablesItem(new QStandardItem("Disposables")) {

  appendRow(m_disposablesItem);
}

DisposableModel::~DisposableModel() {
  for (auto* object: m_disposablesList) {
    delete object;
  }
}

void DisposableModel::AddDisposable(Disposable const& p_disposable) {
  m_disposablesList << new Disposable(p_disposable);

  auto position = p_disposable.GetPosition();

  auto disposableItem = new QStandardItem(tr("%1 #%2").arg(p_disposable.GetName().c_str()).arg(m_disposablesList.size()));
  disposableItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position.GetX()))
    << new QStandardItem(QString::number(position.GetY())));

  m_disposablesItem->appendRow(disposableItem);
}



SwitchModel::SwitchModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_switchesItem(new QStandardItem("Switches")) {

  appendRow(m_switchesItem);
}

SwitchModel::~SwitchModel() {
  for (auto* object: m_switchesList) {
    delete object;
  }
}

void SwitchModel::AddSwitch(Switch const& p_switch) {
  m_switchesList << new Switch(p_switch);

  auto position = p_switch.GetPosition();
  auto switchItem = new QStandardItem(tr("%1 #%2").arg(p_switch.GetName().c_str()).arg(m_switchesList.size()));
  switchItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position.GetX()))
    << new QStandardItem(QString::number(position.GetY())));

  m_switchesItem->appendRow(switchItem);
}



TransferModel::TransferModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_transfersItem(new QStandardItem("Transfers")) {

  appendRow(m_transfersItem);
}

TransferModel::~TransferModel() {
  for (auto* object: m_transfersList) {
    delete object;
  }
}

void TransferModel::AddTransfer(Transfer const& p_transfer) {
  m_transfersList << new Transfer(p_transfer);

  auto position1 = p_transfer.GetPosition();
  auto position1Item = new QStandardItem(tr("Position #1"));
  position1Item->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position1.GetX()))
    << new QStandardItem(QString::number(position1.GetY())));

  auto position2 = p_transfer.GetPosition2();
  auto position2Item = new QStandardItem(tr("Position #2"));
  position2Item->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(position2.GetX()))
    << new QStandardItem(QString::number(position2.GetY())));

  auto transferItem = new QStandardItem(tr("%1 #%2").arg(p_transfer.GetName().c_str()).arg(m_transfersList.size()));
  transferItem->appendRow(position1Item);
  transferItem->appendRow(position2Item);

  m_transfersItem->appendRow(transferItem);
}



OneWayModel::OneWayModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_oneWaysItem(new QStandardItem("One Ways")) {

  appendRow(m_oneWaysItem);
}

OneWayModel::~OneWayModel() {
  for (auto* object: m_oneWaysList) {
    delete object;
  }
}

void OneWayModel::AddOneWay(OneWay const& p_oneWay) {
  m_oneWaysList << new OneWay(p_oneWay);

  auto line = p_oneWay.GetLine();
  auto lineItem = new QStandardItem(tr("Line"));
  lineItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(line.GetA().GetX()))
    << new QStandardItem(QString::number(line.GetA().GetY()))
    << new QStandardItem(QString::number(line.GetB().GetX()))
    << new QStandardItem(QString::number(line.GetB().GetY())));

  auto direction = p_oneWay.GetDirection();
  auto directionItem = new QStandardItem(tr("Direction"));
  directionItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(direction.GetX()))
    << new QStandardItem(QString::number(direction.GetY())));

  auto oneWayItem = new QStandardItem(tr("%1 #%2").arg(p_oneWay.GetName().c_str()).arg(m_oneWaysList.size()));
  oneWayItem->appendRow(lineItem);
  oneWayItem->appendRow(directionItem);

  m_oneWaysItem->appendRow(oneWayItem);
}



TapeModel::TapeModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_tapesItem(new QStandardItem("Tapes")) {

  appendRow(m_tapesItem);
}

TapeModel::~TapeModel() {
  for (auto* object: m_tapesList) {
    delete object;
  }
}

void TapeModel::AddTape(Tape const& p_tape) {
  m_tapesList << new Tape(p_tape);

  auto left = p_tape.getX();
  auto top = p_tape.getY();
  auto right = left + p_tape.getW();
  auto bottom = top + p_tape.getH();
  auto tapeItem = new QStandardItem(tr("%1 #%2").arg(p_tape.GetName().c_str()).arg(m_tapesList.size()));
  tapeItem->appendRow(QList<QStandardItem*>()
    << new QStandardItem(QString::number(left))
    << new QStandardItem(QString::number(top))
    << new QStandardItem(QString::number(right))
    << new QStandardItem(QString::number(bottom)));

  m_tapesItem->appendRow(tapeItem);
}
