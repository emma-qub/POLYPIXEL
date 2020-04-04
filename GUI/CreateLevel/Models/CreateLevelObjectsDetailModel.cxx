#include "CreateLevelObjectsDetailModel.hxx"

#include "Core/Objects/Object.hxx"
#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Deviations/Portal.hxx"

CreateLevelObjectsDetailModel::CreateLevelObjectsDetailModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_tapeX1Item(nullptr),
  m_tapeY1Item(nullptr),
  m_tapeX2Item(nullptr),
  m_tapeY2Item(nullptr),
  m_tapeWItem(nullptr),
  m_tapeHItem(nullptr),
  m_mirrorX1Item(nullptr),
  m_mirrorY1Item(nullptr),
  m_mirrorX2Item(nullptr),
  m_mirrorY2Item(nullptr),
  m_mirrorNXItem(nullptr),
  m_mirrorNYItem(nullptr),
  m_oneWayX1Item(nullptr),
  m_oneWayY1Item(nullptr),
  m_oneWayX2Item(nullptr),
  m_oneWayY2Item(nullptr),
  m_oneWayNXItem(nullptr),
  m_oneWayNYItem(nullptr),
  m_portalInX1Item(nullptr),
  m_portalInY1Item(nullptr),
  m_portalInX2Item(nullptr),
  m_portalInY2Item(nullptr),
  m_portalInNXItem(nullptr),
  m_portalInNYItem(nullptr),
  m_portalOutX1Item(nullptr),
  m_portalOutY1Item(nullptr),
  m_portalOutX2Item(nullptr),
  m_portalOutY2Item(nullptr),
  m_portalOutNXItem(nullptr),
  m_portalOutNYItem(nullptr),
  m_object(nullptr)
{
}

CreateLevelObjectsDetailModel::~CreateLevelObjectsDetailModel() = default;

void CreateLevelObjectsDetailModel::ClearObject() {
  clear();
  m_object = nullptr;
}

void CreateLevelObjectsDetailModel::ResetCurrentObject(Object* p_object) {
  m_object = p_object;
  clear();

  if (p_object == nullptr)
    return;

  setColumnCount(2);
  auto objectType = p_object->GetObjectType();

  switch (objectType) {
  case Object::eTape:{
    auto tapeObject = static_cast<Tape*>(p_object);

    auto dimensionsItem = new QStandardItem("Dimensions");
    m_tapeWItem = new QStandardItem(QString("w: %1").arg(QString::number(tapeObject->GetW(), 'f', 0)));
    m_tapeHItem = new QStandardItem(QString("h: %1").arg(QString::number(tapeObject->GetH(), 'f', 0)));
    dimensionsItem->appendRow({m_tapeWItem, m_tapeHItem});
    appendRow(dimensionsItem);

    auto topLeftItem = new QStandardItem("Top left");
    m_tapeX1Item = new QStandardItem(QString("x: %1").arg(QString::number(tapeObject->GetX1(), 'f', 0)));
    m_tapeY1Item = new QStandardItem(QString("y: %1").arg(QString::number(tapeObject->GetY1(), 'f', 0)));
    topLeftItem->appendRow({m_tapeX1Item, m_tapeY1Item});
    appendRow(topLeftItem);

    auto bottomRightItem = new QStandardItem("Bottom right");
    m_tapeX2Item = new QStandardItem(QString("x: %1").arg(QString::number(tapeObject->GetX2(), 'f', 0)));
    m_tapeY2Item = new QStandardItem(QString("y: %1").arg(QString::number(tapeObject->GetY2(), 'f', 0)));
    bottomRightItem->appendRow({m_tapeX2Item, m_tapeY2Item});
    appendRow(bottomRightItem);

    break;
  } case Object::eMirror:{
    auto mirrorObject = static_cast<Mirror*>(p_object);

    auto startItem = new QStandardItem("Start");
    m_mirrorX1Item = new QStandardItem(QString("x: %1").arg(QString::number(mirrorObject->GetX1(), 'f', 0)));
    m_mirrorY1Item = new QStandardItem(QString("y: %1").arg(QString::number(mirrorObject->GetY1(), 'f', 0)));
    startItem->appendRow({m_mirrorX1Item, m_mirrorY1Item});
    appendRow(startItem);

    auto endItem = new QStandardItem("End");
    m_mirrorX2Item = new QStandardItem(QString("x: %1").arg(QString::number(mirrorObject->GetX2(), 'f', 0)));
    m_mirrorY2Item = new QStandardItem(QString("y: %1").arg(QString::number(mirrorObject->GetY2(), 'f', 0)));
    endItem->appendRow({m_mirrorX2Item, m_mirrorY2Item});
    appendRow(endItem);

    auto normalItem = new QStandardItem("Normal");
    m_mirrorNXItem = new QStandardItem(QString("x: %1").arg(QString::number(mirrorObject->GetLine().GetNormal().GetX(), 'f', 3)));
    m_mirrorNYItem = new QStandardItem(QString("y: %1").arg(QString::number(mirrorObject->GetLine().GetNormal().GetY(), 'f', 3)));
    normalItem->appendRow({m_mirrorNXItem, m_mirrorNYItem});
    appendRow(normalItem);

    break;
  } case Object::eOneWay:{
    auto oneWayObject = static_cast<OneWay*>(p_object);

    auto startItem = new QStandardItem("Start");
    m_oneWayX1Item = new QStandardItem(QString("x: %1").arg(QString::number(oneWayObject->GetX1(), 'f', 0)));
    m_oneWayY1Item = new QStandardItem(QString("y: %1").arg(QString::number(oneWayObject->GetY1(), 'f', 0)));
    startItem->appendRow({m_oneWayX1Item, m_oneWayY1Item});
    appendRow(startItem);

    auto endItem = new QStandardItem("End");
    m_oneWayX2Item = new QStandardItem(QString("x: %1").arg(QString::number(oneWayObject->GetX2(), 'f', 0)));
    m_oneWayY2Item = new QStandardItem(QString("y: %1").arg(QString::number(oneWayObject->GetY2(), 'f', 0)));
    endItem->appendRow({m_oneWayX2Item, m_oneWayY2Item});
    appendRow(endItem);

    auto normalItem = new QStandardItem("Normal");
    m_oneWayNXItem = new QStandardItem(QString("x: %1").arg(QString::number(oneWayObject->GetNX(), 'f', 3)));
    m_oneWayNYItem = new QStandardItem(QString("y: %1").arg(QString::number(oneWayObject->GetNY(), 'f', 3)));
    normalItem->appendRow({m_oneWayNXItem, m_oneWayNYItem});
    appendRow(normalItem);

    break;
  } case Object::ePortal:{
    auto portalObject = static_cast<Portal*>(p_object);

    // Yellow
    auto inItem = new QStandardItem("Yellow");
    appendRow(inItem);

    auto startInItem = new QStandardItem("Yellow Start");
    m_portalInX1Item = new QStandardItem(QString("x: %1").arg(QString::number(portalObject->GetIn().GetA().GetX(), 'f', 0)));
    m_portalInY1Item = new QStandardItem(QString("y: %1").arg(QString::number(portalObject->GetIn().GetA().GetY(), 'f', 0)));
    startInItem->appendRow({m_portalInX1Item, m_portalInY1Item});
    inItem->appendRow(startInItem);

    auto endInItem = new QStandardItem("Yellow End");
    m_portalInX2Item = new QStandardItem(QString("x: %1").arg(QString::number(portalObject->GetIn().GetB().GetX(), 'f', 0)));
    m_portalInY2Item = new QStandardItem(QString("y: %1").arg(QString::number(portalObject->GetIn().GetB().GetY(), 'f', 0)));
    endInItem->appendRow({m_portalInX2Item, m_portalInY2Item});
    inItem->appendRow(endInItem);

    auto normalInItem = new QStandardItem("Yellow Normal");
    m_portalInNXItem = new QStandardItem(QString("x: %1").arg(QString::number(portalObject->GetIn().GetNormal().GetX(), 'f', 3)));
    m_portalInNYItem = new QStandardItem(QString("y: %1").arg(QString::number(portalObject->GetIn().GetNormal().GetY(), 'f', 3)));
    normalInItem->appendRow({m_portalInNXItem, m_portalInNYItem});
    inItem->appendRow(normalInItem);

    // Blue
    auto outItem = new QStandardItem("Blue");
    appendRow(outItem);

    auto startOutItem = new QStandardItem("Blue Start");
    m_portalOutX1Item = new QStandardItem(QString("x: %1").arg(QString::number(portalObject->GetOut().GetA().GetX(), 'f', 0)));
    m_portalOutY1Item = new QStandardItem(QString("y: %1").arg(QString::number(portalObject->GetOut().GetA().GetY(), 'f', 0)));
    startOutItem->appendRow({m_portalOutX1Item, m_portalOutY1Item});
    outItem->appendRow(startOutItem);

    auto endOutItem = new QStandardItem("Blue End");
    m_portalOutX2Item = new QStandardItem(QString("x: %1").arg(QString::number(portalObject->GetOut().GetB().GetX(), 'f', 0)));
    m_portalOutY2Item = new QStandardItem(QString("y: %1").arg(QString::number(portalObject->GetOut().GetB().GetY(), 'f', 0)));
    endOutItem->appendRow({m_portalOutX2Item, m_portalOutY2Item});
    outItem->appendRow(endOutItem);

    auto normalOutItem = new QStandardItem("Blue Normal");
    m_portalOutNXItem = new QStandardItem(QString("x: %1").arg(QString::number(portalObject->GetOut().GetNormal().GetX(), 'f', 3)));
    m_portalOutNYItem = new QStandardItem(QString("y: %1").arg(QString::number(portalObject->GetOut().GetNormal().GetY(), 'f', 3)));
    normalOutItem->appendRow({m_portalOutNXItem, m_portalOutNYItem});
    outItem->appendRow(normalOutItem);

    break;
  } default:
    break;
  }
}

void CreateLevelObjectsDetailModel::UpdateCurrentObject() {
  auto objectType = m_object->GetObjectType();

  switch (objectType) {
  case Object::eTape:{
    auto tapeObject = static_cast<Tape*>(m_object);

    m_tapeWItem->setText(QString("w: %1").arg(QString::number(tapeObject->GetW(), 'f', 0)));
    m_tapeHItem->setText(QString("h: %1").arg(QString::number(tapeObject->GetH(), 'f', 0)));

    m_tapeX1Item->setText(QString("x: %1").arg(QString::number(tapeObject->GetX1(), 'f', 0)));
    m_tapeY1Item->setText(QString("y: %1").arg(QString::number(tapeObject->GetY1(), 'f', 0)));

    m_tapeX2Item->setText(QString("x: %1").arg(QString::number(tapeObject->GetX2(), 'f', 0)));
    m_tapeY2Item->setText(QString("y: %1").arg(QString::number(tapeObject->GetY2(), 'f', 0)));

    break;
  } case Object::eMirror:{
    auto mirrorObject = static_cast<Mirror*>(m_object);

    m_mirrorX1Item->setText(QString("x: %1").arg(QString::number(mirrorObject->GetX1(), 'f', 0)));
    m_mirrorY1Item->setText(QString("y: %1").arg(QString::number(mirrorObject->GetY1(), 'f', 0)));

    m_mirrorX2Item->setText(QString("x: %1").arg(QString::number(mirrorObject->GetX2(), 'f', 0)));
    m_mirrorY2Item->setText(QString("y: %1").arg(QString::number(mirrorObject->GetY2(), 'f', 0)));

    m_mirrorNXItem->setText(QString("x: %1").arg(QString::number(mirrorObject->GetLine().GetNormal().GetX(), 'f', 3)));
    m_mirrorNYItem->setText(QString("y: %1").arg(QString::number(mirrorObject->GetLine().GetNormal().GetY(), 'f', 3)));

    break;
  } case Object::eOneWay:{
    auto oneWayObject = static_cast<OneWay*>(m_object);

    m_oneWayX1Item->setText(QString("x: %1").arg(QString::number(oneWayObject->GetX1(), 'f', 0)));
    m_oneWayY1Item->setText(QString("y: %1").arg(QString::number(oneWayObject->GetY1(), 'f', 0)));

    m_oneWayX2Item->setText(QString("x: %1").arg(QString::number(oneWayObject->GetX2(), 'f', 0)));
    m_oneWayY2Item->setText(QString("y: %1").arg(QString::number(oneWayObject->GetY2(), 'f', 0)));

    m_oneWayNXItem->setText(QString("x: %1").arg(QString::number(oneWayObject->GetNX(), 'f', 3)));
    m_oneWayNYItem->setText(QString("y: %1").arg(QString::number(oneWayObject->GetNY(), 'f', 3)));

    break;
  } case Object::ePortal:{
    auto portalObject = static_cast<Portal*>(m_object);

    // Yellow
    m_portalInX1Item->setText(QString::number(portalObject->GetIn().GetA().GetX(), 'f', 0));
    m_portalInY1Item->setText(QString::number(portalObject->GetIn().GetA().GetY(), 'f', 0));

    m_portalInX2Item->setText(QString::number(portalObject->GetIn().GetB().GetX(), 'f', 0));
    m_portalInY2Item->setText(QString::number(portalObject->GetIn().GetB().GetY(), 'f', 0));

    m_portalInNXItem->setText(QString::number(portalObject->GetIn().GetNormal().GetX(), 'f', 3));
    m_portalInNYItem->setText(QString::number(portalObject->GetIn().GetNormal().GetY(), 'f', 3));

    // Blue
    m_portalOutX1Item->setText(QString::number(portalObject->GetOut().GetA().GetX(), 'f', 0));
    m_portalOutY1Item->setText(QString::number(portalObject->GetOut().GetA().GetY(), 'f', 0));

    m_portalOutX2Item->setText(QString::number(portalObject->GetOut().GetB().GetX(), 'f', 0));
    m_portalOutY2Item->setText(QString::number(portalObject->GetOut().GetB().GetY(), 'f', 0));

    m_portalOutNXItem->setText(QString::number(portalObject->GetOut().GetNormal().GetX(), 'f', 3));
    m_portalOutNYItem->setText(QString::number(portalObject->GetOut().GetNormal().GetY(), 'f', 3));

    break;
  } default:
    break;
  }
}
