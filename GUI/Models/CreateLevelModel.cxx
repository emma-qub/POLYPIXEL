#include "CreateLevelModel.hxx"
#include "PolygonItem.hxx"

#include "Core/Geometry/Vector.hxx"

#include <random>

CreateLevelModel::CreateLevelModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_selections(),
  m_polygonsItem(new QStandardItem("Polygons")),
  m_tapesItem(new QStandardItem("Tapes")),
  m_mirrorsItem(new QStandardItem("Mirrors")),
  m_oneWaysItem(new QStandardItem("One Ways")),
  m_portalsItem(new QStandardItem("Portals")),
  m_color() {

  m_polygonsItem->setData(eObjectList, eItemTypeRole);
  appendRow(m_polygonsItem);
  m_tapesItem->setData(eObjectList, eItemTypeRole);
  appendRow(m_tapesItem);
  m_mirrorsItem->setData(eObjectList, eItemTypeRole);
  appendRow(m_mirrorsItem);
  m_oneWaysItem->setData(eObjectList, eItemTypeRole);
  appendRow(m_oneWaysItem);
  m_portalsItem->setData(eObjectList, eItemTypeRole);
  appendRow(m_portalsItem);

  setColumnCount(4);
  m_selections << QPair<int, int>(-1, -1);
}

CreateLevelModel::~CreateLevelModel() = default;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// POLYGON
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateLevelModel::InitColor() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> distribution(0, 255);

  auto r = distribution(rng);
  auto g = distribution(rng);
  auto b = distribution(rng);

  m_color = QColor(r, g, b);
}

void CreateLevelModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygon = new ppxl::Polygon(p_polygon);
  auto polygonItem = new PolygonItem(polygon, m_color);
  polygonItem->setData(eObject, eItemTypeRole);
  polygonItem->setData(ePolygon, eObjectTypeRole);
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon*>(polygon), eObjectRole);

  m_polygonsItem->insertRow(p_row, {polygonItem, new QStandardItem, new QStandardItem});
  m_polygons << polygon;
}

void CreateLevelModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_polygonsItem->rowCount(), p_polygon);
}

void CreateLevelModel::SetPolygonsList(QList<ppxl::Polygon> const& p_polygonsList) {
  ClearPolygons();
  for (auto const& polygon: p_polygonsList) {
    AppendPolygon(polygon);
  }

  Q_EMIT(PolygonListChanged());
}

void CreateLevelModel::RemovePolygonAt(int p_polygonRow) {
  delete m_polygons.takeAt(p_polygonRow);
  for (auto* item: m_polygonsItem->takeRow(p_polygonRow))
  {
    delete item;
  }
}

void CreateLevelModel::ClearPolygons() {
  for (int row = m_polygonsItem->rowCount()-1; row > -1; --row)
  {
    RemovePolygonAt(row);
  }
  m_polygons.clear();
  m_polygonsItem->setRowCount(0);
  m_polygonsItem->setColumnCount(0);
}

void CreateLevelModel::SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  auto* polygon = polygonItem->data(eObjectRole).value<ppxl::Polygon*>();
  polygon->SetVertices(p_polygon.GetVertices());
}

void CreateLevelModel::TranslatePolygon(int p_polygonRow, const ppxl::Vector& p_direction) {
  // Update polygon AND polygon item
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  for (int row = 0; row < polygonItem->rowCount(); ++row) {
    TranslateVertex(p_polygonRow, row, p_direction);
  }
}

void CreateLevelModel::InsertVertex(int p_polygonRow, int p_vertexRow, const ppxl::Point& p_vertex) {
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  auto polygon = polygonItem->data(eObjectRole).value<ppxl::Polygon*>();

  // Insert vertex
  polygon->InsertVertex(p_vertex, static_cast<unsigned int>(p_vertexRow));

  // Insert vertex row
  auto& vertex = polygon->GetVertices()[static_cast<unsigned long>(p_vertexRow)];

  auto* vertexXItem = new VertexXItem(vertex);
  vertexXItem->setData(eX, eItemTypeRole);
  auto* vertexYItem = new VertexYItem(vertex);
  vertexYItem->setData(eY, eItemTypeRole);
  auto* vertexItem = new VertexLabelItem;
  vertexItem->setData(ePoint, eItemTypeRole);

  polygonItem->insertRow(p_vertexRow, {vertexItem, vertexXItem, vertexYItem});
}

void CreateLevelModel::RemoveVertexAt(int p_polygonRow, int p_vertexRow) {
  // Update vertex
  auto* polygon = GetPolygonsList().at(p_polygonRow);
  polygon->GetVertices().erase(polygon->GetVertices().begin() + p_vertexRow);

  // Update vertex item
  GetPolygonsItem()->child(p_polygonRow)->removeRow(p_vertexRow);
}

void CreateLevelModel::TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction) {
  // Update polygon
  auto* polygon = GetPolygonsList().at(p_polygonRow);
  auto translatedVertex = polygon->GetVertices().at(static_cast<unsigned long>(p_vertexRow)).Translated(p_direction);

  // Update polygon item
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  auto vertexItemX = static_cast<VertexXItem*>(polygonItem->child(p_vertexRow, 1));
  vertexItemX->SetVertex(translatedVertex);
  auto vertexItemY = static_cast<VertexYItem*>(polygonItem->child(p_vertexRow, 2));
  vertexItemY->SetVertex(translatedVertex);

  Q_EMIT(dataChanged(
    index(p_vertexRow, 0, polygonItem->index()),
    index(p_vertexRow, 2, polygonItem->index())));
}

QModelIndex CreateLevelModel::GetVertexIndex(int p_polygonRow, int p_vertexRow) const {
  auto polygonsItem = GetPolygonsItem();
  auto polygonItem = polygonsItem->child(p_polygonRow, 0);
  if (polygonsItem->rowCount() > p_polygonRow && polygonItem->rowCount() > p_vertexRow) {
    return indexFromItem(polygonItem->child(p_vertexRow, 0));
  }

  return QModelIndex();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TAPE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelModel::AddTape(Tape const& p_tape) {
  auto tape = new Tape(p_tape);
  m_tapesList << tape;

  auto tapeItem = new QStandardItem(tr("%1 #%2  (%3 x %4)").arg(tape->GetName().c_str()).arg(m_tapesList.size()).arg(tape->GetW()).arg(tape->GetH()));
  tapeItem->setData(eObject, eItemTypeRole);
  tapeItem->setData(eTape, eObjectTypeRole);
  tapeItem->setData(QVariant::fromValue<Object*>(tape), eObjectRole);
  auto topLeftItem = new QStandardItem("Top left");
  topLeftItem->setData(ePoint, eItemTypeRole);
  topLeftItem->appendRow({
    new QStandardItem("A"),
    new QStandardItem(QString::number(p_tape.GetLeft(), 'f', 0)),
    new QStandardItem(QString::number(p_tape.GetTop(), 'f', 0)),
    new QStandardItem});
  tapeItem->appendRow(topLeftItem);
  auto bottomRightItem = new QStandardItem("Bottom right");
  bottomRightItem->setData(ePoint, eItemTypeRole);
  bottomRightItem->appendRow({
    new QStandardItem("B"),
    new QStandardItem(QString::number(p_tape.GetRight(), 'f', 0)),
    new QStandardItem(QString::number(p_tape.GetBottom(), 'f', 0)),
    new QStandardItem});
  tapeItem->appendRow(bottomRightItem);

  m_tapesItem->appendRow({tapeItem, new QStandardItem, new QStandardItem, new QStandardItem});
  RegisterObject(tape, tapeItem);

  return tapeItem;
}

void CreateLevelModel::SetTape(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eTape);
  auto newTape = static_cast<Tape*>(p_object);

  // Update Tape
  auto tape = m_tapesList.at(p_index);
  tape->SetX(newTape->GetX());
  tape->SetY(newTape->GetY());
  tape->SetW(newTape->GetW());
  tape->SetH(newTape->GetH());

  // Update Tape Item
  auto tapeItem = m_tapesItem->child(p_index, 0);
  tapeItem->setText(tr("%1 #%2  (%3 x %4)").arg(tape->GetName().c_str()).arg(m_tapesList.size()).arg(tape->GetW()).arg(tape->GetH()));
  auto topLeftItem = tapeItem->child(0, 0);
  topLeftItem->child(0, 1)->setText(QString::number(tape->GetLeft(), 'f', 0));
  topLeftItem->child(0, 2)->setText(QString::number(tape->GetTop(), 'f', 0));
  auto bottomRightItem = tapeItem->child(0, 0);
  bottomRightItem->child(0, 1)->setText(QString::number(tape->GetRight(), 'f', 0));
  bottomRightItem->child(0, 2)->setText(QString::number(tape->GetBottom(), 'f', 0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MIRROR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelModel::AddMirror(Mirror const& p_mirror) {
  auto mirror = new Mirror(p_mirror);
  m_mirrorsList << mirror;

  auto mirrorItem = new QStandardItem(tr("%1 #%2").arg(p_mirror.GetName().c_str()).arg(m_mirrorsList.size()));
  mirrorItem->setData(eObject, eItemTypeRole);
  mirrorItem->setData(eMirror, eObjectTypeRole);
  mirrorItem->setData(QVariant::fromValue<Object*>(mirror), eObjectRole);
  auto line = p_mirror.GetLine();
  auto mirrorLineItem = new QStandardItem("Line");
  mirrorLineItem->setData(eSegment, eItemTypeRole);
  mirrorLineItem->appendRow({
    new QStandardItem(QString::number(line.GetA().GetX(), 'f', 0)),
    new QStandardItem(QString::number(line.GetA().GetY(), 'f', 0)),
    new QStandardItem(QString::number(line.GetB().GetX(), 'f', 0)),
    new QStandardItem(QString::number(line.GetB().GetY(), 'f', 0))});
  mirrorItem->appendRow(mirrorLineItem);

  m_mirrorsItem->appendRow({mirrorItem, new QStandardItem, new QStandardItem, new QStandardItem});
  RegisterObject(mirror, mirrorItem);

  return mirrorItem;
}

void CreateLevelModel::SetMirror(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eMirror);
  auto newMirror = static_cast<Mirror*>(p_object);

  //Update Mirror
  auto mirror = m_mirrorsList.at(p_index);
  mirror->SetLine(newMirror->GetLine());

  // Update Mirror Item
  auto mirrorItem = m_mirrorsItem->child(p_index, 0);
  auto line = mirror->GetLine();
  auto mirrorLineItem = mirrorItem->child(0, 0);
  mirrorLineItem->child(0, 0)->setText(QString::number(line.GetA().GetX(), 'f', 0));
  mirrorLineItem->child(0, 1)->setText(QString::number(line.GetA().GetY(), 'f', 0));
  mirrorLineItem->child(0, 2)->setText(QString::number(line.GetB().GetX(), 'f', 0));
  mirrorLineItem->child(0, 3)->setText(QString::number(line.GetB().GetY(), 'f', 0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ONE WAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelModel::AddOneWay(OneWay const& p_oneWay) {
  auto oneWay = new OneWay(p_oneWay);
  m_oneWaysList << oneWay;

  auto line = p_oneWay.GetLine();
  auto lineItem = new QStandardItem(tr("Line"));
  lineItem->setData(eSegment, eItemTypeRole);
  lineItem->appendRow({
    new QStandardItem(QString::number(line.GetA().GetX(), 'f', 0)),
    new QStandardItem(QString::number(line.GetA().GetY(), 'f', 0)),
    new QStandardItem(QString::number(line.GetB().GetX(), 'f', 0)),
    new QStandardItem(QString::number(line.GetB().GetY(), 'f', 0))});

  auto direction = p_oneWay.GetDirection();
  auto directionItem = new QStandardItem(tr("Direction"));
  directionItem->appendRow({
    new QStandardItem(QString::number(direction.GetX(), 'f', 3)),
    new QStandardItem(QString::number(direction.GetY(), 'f', 3)),
    new QStandardItem,
    new QStandardItem});

  auto oneWayItem = new QStandardItem(tr("%1 #%2").arg(p_oneWay.GetName().c_str()).arg(m_oneWaysList.size()));
  oneWayItem->setData(eObject, eItemTypeRole);
  oneWayItem->setData(eOneWay, eObjectTypeRole);
  oneWayItem->setData(QVariant::fromValue<Object*>(oneWay), eObjectRole);
  oneWayItem->appendRow({lineItem, new QStandardItem, new QStandardItem, new QStandardItem});
  oneWayItem->appendRow({directionItem, new QStandardItem, new QStandardItem, new QStandardItem});

  m_oneWaysItem->appendRow({oneWayItem, new QStandardItem, new QStandardItem, new QStandardItem});
  RegisterObject(oneWay, oneWayItem);

  return oneWayItem;
}

void CreateLevelModel::SetOneWay(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::eOneWay);
  auto newOneWay = static_cast<OneWay*>(p_object);

  // Update OneWay
  auto oneWay = m_oneWaysList.at(p_index);
  oneWay->SetLine(newOneWay->GetLine());

  // Update OneWay Item
  auto oneWayItem = m_oneWaysItem->child(p_index, 0);
  auto lineItem = oneWayItem->child(0, 0);
  auto line = oneWay->GetLine();
  lineItem->child(0, 0)->setText(QString::number(line.GetA().GetX(), 'f', 0));
  lineItem->child(0, 1)->setText(QString::number(line.GetA().GetY(), 'f', 0));
  lineItem->child(0, 2)->setText(QString::number(line.GetB().GetX(), 'f', 0));
  lineItem->child(0, 3)->setText(QString::number(line.GetB().GetY(), 'f', 0));
  auto directionItem = oneWayItem->child(1, 0);
  directionItem->setData(eVector, eItemTypeRole);
  auto direction = oneWay->GetDirection();
  directionItem->child(0, 0)->setText(QString::number(direction.GetX(), 'f', 3));
  directionItem->child(0, 1)->setText(QString::number(direction.GetY(), 'f', 3));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PORTAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelModel::AddPortal(Portal const& p_portal) {
  auto portal = new Portal(p_portal);
  m_portalsList << portal;

  auto entranceItem = new QStandardItem(tr("Entrance"));
  auto entranceLineItem = new QStandardItem(tr("Line"));
  entranceLineItem->setData(eSegment, eItemTypeRole);
  auto entranceLine = p_portal.GetIn();
  entranceLineItem->appendRow({
    new QStandardItem(QString::number(entranceLine.GetA().GetX(), 'f', 0)),
    new QStandardItem(QString::number(entranceLine.GetA().GetY(), 'f', 0)),
    new QStandardItem(QString::number(entranceLine.GetB().GetX(), 'f', 0)),
    new QStandardItem(QString::number(entranceLine.GetB().GetY(), 'f', 0))});
  entranceItem->appendRow(entranceLineItem);
  auto entranceNormalItem = new QStandardItem(tr("Normal"));
  entranceNormalItem->setData(eVector, eItemTypeRole);
  auto entranceNormal = p_portal.GetNormalIn();
  entranceNormalItem->appendRow({
    new QStandardItem(QString::number(entranceNormal.GetX(), 'f', 3)),
    new QStandardItem(QString::number(entranceNormal.GetY(), 'f', 3)),
    new QStandardItem,
    new QStandardItem});
  entranceItem->appendRow(entranceNormalItem);

  auto exitItem = new QStandardItem(tr("Exit"));
  auto exitLineItem = new QStandardItem(tr("Line"));
  exitLineItem->setData(eSegment, eItemTypeRole);
  auto exit = p_portal.GetOut();
  exitLineItem->appendRow({
    new QStandardItem(QString::number(exit.GetA().GetX(), 'f', 0)),
    new QStandardItem(QString::number(exit.GetA().GetY(), 'f', 0)),
    new QStandardItem(QString::number(exit.GetB().GetX(), 'f', 0)),
    new QStandardItem(QString::number(exit.GetB().GetY(), 'f', 0))});
  exitItem->appendRow(exitLineItem);
  auto exitNormalItem = new QStandardItem(tr("Normal"));
  exitNormalItem->setData(eVector, eItemTypeRole);
  auto exitNormal = p_portal.GetNormalOut();
  exitNormalItem->appendRow({
   new QStandardItem(QString::number(exitNormal.GetX(), 'f', 3)),
   new QStandardItem(QString::number(exitNormal.GetY(), 'f', 3)),
   new QStandardItem,
   new QStandardItem});
 exitItem->appendRow(exitNormalItem);

  auto portalItem = new QStandardItem(tr("%1 #%2").arg(p_portal.GetName().c_str()).arg(m_portalsList.size()));
  portalItem->setData(eObject, eItemTypeRole);
  portalItem->setData(ePortal, eObjectTypeRole);
  portalItem->setData(QVariant::fromValue<Object*>(portal), eObjectRole);
  portalItem->appendRow({entranceItem, new QStandardItem, new QStandardItem, new QStandardItem});
  portalItem->appendRow({exitItem, new QStandardItem, new QStandardItem, new QStandardItem});

  m_portalsItem->appendRow({portalItem, new QStandardItem, new QStandardItem, new QStandardItem});
  RegisterObject(portal, portalItem);

  return portalItem;
}

void CreateLevelModel::SetPortal(int p_index, Object* p_object) {
  assert(p_object->GetObjectType() == Object::ePortal);
  auto newPortal = static_cast<Portal*>(p_object);

  // Update Portal
  auto portal = m_portalsList.at(p_index);
  portal->SetIn(newPortal->GetIn());
  portal->SetOut(newPortal->GetOut());

  // Update Portal Item
  auto portalItem = m_portalsItem->child(p_index, 0);
  auto inLine = portal->GetIn();
  auto inItem = portalItem->child(0, 0);
  auto inLineItem = inItem->child(0, 0);
  inLineItem->child(0, 0)->setText(QString::number(inLine.GetA().GetX(), 'f', 0));
  inLineItem->child(0, 1)->setText(QString::number(inLine.GetA().GetY(), 'f', 0));
  inLineItem->child(0, 2)->setText(QString::number(inLine.GetB().GetX(), 'f', 0));
  inLineItem->child(0, 3)->setText(QString::number(inLine.GetB().GetY(), 'f', 0));
  auto normalIn = portal->GetNormalIn();
  auto inNormalItem = inItem->child(1, 0);
  inNormalItem->child(0, 0)->setText(QString::number(normalIn.GetX(), 'f', 0));
  inNormalItem->child(0, 1)->setText(QString::number(normalIn.GetY(), 'f', 0));
  auto outLine = portal->GetOut();
  auto outItem = portalItem->child(1, 0);
  auto outLineItem = outItem->child(0, 0);
  outLineItem->child(0, 0)->setText(QString::number(outLine.GetA().GetX(), 'f', 0));
  outLineItem->child(0, 1)->setText(QString::number(outLine.GetA().GetY(), 'f', 0));
  outLineItem->child(0, 2)->setText(QString::number(outLine.GetB().GetX(), 'f', 0));
  outLineItem->child(0, 3)->setText(QString::number(outLine.GetB().GetY(), 'f', 0));
  auto normalOut = portal->GetNormalOut();
  auto normalOutItem = outItem->child(1, 0);
  normalOutItem->child(0, 0)->setText(QString::number(normalOut.GetX(), 'f', 0));
  normalOutItem->child(0, 1)->setText(QString::number(normalOut.GetY(), 'f', 0));
}
