#include "CreateLevelObjectsListModel.hxx"

#include "Core/Geometry/Vector.hxx"

#include <random>

CreateLevelObjectsListModel::CreateLevelObjectsListModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_selections(),
  m_polygonsItem(nullptr),
  m_tapesItem(nullptr),
  m_mirrorsItem(nullptr),
  m_oneWaysItem(nullptr),
  m_portalsItem(nullptr) {

  SetDefaultItems();

  m_selections << QPair<int, int>(-1, -1);
}

CreateLevelObjectsListModel::~CreateLevelObjectsListModel() = default;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// POLYGON
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateLevelObjectsListModel::IsPolygonItem(QStandardItem* p_item) const {
  return IsPolygonIndex(p_item->index());
}

bool CreateLevelObjectsListModel::IsPolygonIndex(const QModelIndex& p_index) const {
  return p_index.data(eIsPolygonRole).toBool() && p_index.data(eIsObjectRole).toBool();
}

ppxl::Polygon* CreateLevelObjectsListModel::GetPolygonFromIndex(const QModelIndex& p_index) const {
  Q_ASSERT_X(IsPolygonIndex(p_index), "CreateLevelObjectsListModel::GetPolygonFromItem",
    QString("%1 is not a polygon item.").arg(p_index.data().toString()).toStdString().c_str());
  return FindPolygonFromIndex(p_index);
}

ppxl::Polygon* CreateLevelObjectsListModel::GetPolygonFromItem(QStandardItem* p_item) const {
  return GetPolygonFromIndex(p_item->index());
}

ppxl::Polygon* CreateLevelObjectsListModel::GetPolygonFromRow(int p_row) const {
  Q_ASSERT_X(p_row < m_polygonsItem->rowCount(), "CreateLevelObjectsListModel::GetPolygonFromRow",
    QString("Invalid ind %1 for polygon's row").arg(p_row).toStdString().c_str());
  return GetPolygonFromItem(m_polygonsItem->child(p_row));
}

ppxl::Polygon* CreateLevelObjectsListModel::FindPolygonFromIndex(const QModelIndex& p_index) const {
  auto polygonVariant = p_index.data(ePolygonRole);
  return polygonVariant.isValid()?
    polygonVariant.value<ppxl::Polygon*>():
    nullptr;
}

ppxl::Polygon* CreateLevelObjectsListModel::FindPolygonFromItem(QStandardItem* p_item) const {
  return FindPolygonFromIndex(p_item->index());
}

ppxl::Polygon* CreateLevelObjectsListModel::FindPolygonFromRow(int p_row) const {
  return FindPolygonFromItem(m_polygonsItem->child(p_row));
}

QColor CreateLevelObjectsListModel::GetRandomColor() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> distribution(0, 255);

  auto r = distribution(rng);
  auto g = distribution(rng);
  auto b = distribution(rng);

  return QColor(r, g, b);
}

QList<ppxl::Polygon*> CreateLevelObjectsListModel::GetPolygonsList() const {
  QList<ppxl::Polygon*> polygonsList;
  for (int row = 0; row < m_polygonsItem->rowCount() ;++row) {
    polygonsList << GetPolygonFromRow(row);
  }
  return polygonsList;
}

void CreateLevelObjectsListModel::MoveObject(QModelIndex const& p_objectIndex, ppxl::Point const& p_pos, Object::ControlPointType p_controlPointType) {
  auto object = GetObjectFromIndex(p_objectIndex);
  object->MoveControlPoint(p_pos, p_controlPointType);

  Q_EMIT ObjectChanged();
}

QStandardItem* CreateLevelObjectsListModel::AddPolygon(ppxl::Polygon* p_polygon, GraphicsPolygonItem* p_graphicsObjectItem) {
  auto color = GetRandomColor();
  auto polygonItem = new QStandardItem(tr("Polygon_%1").arg(m_polygonsItem->rowCount()));
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon*>(p_polygon), ePolygonRole);
  polygonItem->setData(color, Qt::DecorationRole);
  polygonItem->setData(true, eIsPolygonRole);
  polygonItem->setData(true, eIsObjectRole);
  p_graphicsObjectItem->SetColor(color);
  SetGraphicsToItem(p_graphicsObjectItem, polygonItem);

  m_polygonsItem->appendRow(polygonItem);

  Q_EMIT PolygonInserted();

  return polygonItem;
}

void CreateLevelObjectsListModel::TranslatePolygon(int p_polygonRow, ppxl::Vector const& p_direction) {
  auto polygon = GetPolygonFromRow(p_polygonRow);
  polygon->Translate(p_direction);

  Q_EMIT PolygonChanged();
}

void CreateLevelObjectsListModel::InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex) {
  auto polygon = GetPolygonFromRow(p_polygonRow);
  polygon->InsertVertex(p_vertex, p_vertexRow);

  Q_EMIT PolygonChanged();
}

void CreateLevelObjectsListModel::SetVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex) {
  auto polygon = GetPolygonFromRow(p_polygonRow);
  polygon->SetVertexAt(p_vertex, p_vertexRow);

  Q_EMIT PolygonChanged();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// OBJECT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateLevelObjectsListModel::IsObjectItem(QStandardItem* p_item) const {
  return IsObjectIndex(p_item->index());
}

bool CreateLevelObjectsListModel::IsObjectIndex(const QModelIndex& p_index) const {
  return p_index.data(eIsObjectRole).toBool();
}

bool CreateLevelObjectsListModel::IsNotPolygonItem(QStandardItem* p_item) const {
  return IsNotPolygonIndex(p_item->index());
}

bool CreateLevelObjectsListModel::IsNotPolygonIndex(const QModelIndex& p_index) const {
  return p_index.data(eIsObjectRole).toBool() && !p_index.data(eIsPolygonRole).toBool();
}

Object* CreateLevelObjectsListModel::GetObjectFromItem(QStandardItem* p_item) const {
  return GetObjectFromIndex(p_item->index());
}

Object* CreateLevelObjectsListModel::GetObjectFromIndex(const QModelIndex& p_index) const {
  Q_ASSERT_X(IsObjectIndex(p_index), QString("%1 is not an object item.").arg(p_index.data().toString()).toStdString().c_str(), "CreateLevelObjectsListModel::GetObjectFromIndex");
  return FindObjectFromIndex(p_index);
}

Object* CreateLevelObjectsListModel::FindObjectFromItem(QStandardItem* p_item) const {
  return FindObjectFromIndex(p_item->index());
}

Object* CreateLevelObjectsListModel::FindObjectFromIndex(const QModelIndex& p_index) const {
  auto polygonVariant = p_index.data(eObjectRole);
  return polygonVariant.isValid()?
    polygonVariant.value<Object*>():
        nullptr;
}

QStandardItem* CreateLevelObjectsListModel::AddObject(Object* p_object, GraphicsObjectItem* p_graphicsObjectItem, QStandardItem* p_listItem) {
  auto objectItem = new QStandardItem(tr("%1_%2").arg(p_object->GetName().c_str()).arg(p_listItem->rowCount()));
  objectItem->setData(true, eIsObjectRole);
  objectItem->setData(QVariant::fromValue<Object*>(p_object), eObjectRole);
  SetGraphicsToItem(p_graphicsObjectItem, objectItem);

  p_listItem->appendRow(objectItem);

  Q_EMIT ObjectInserted();

  return objectItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TAPE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelObjectsListModel::AddTape(Object* p_tape, GraphicsObjectItem* p_tapeGraphicsItem) {
  auto tapeItem = AddObject(p_tape, p_tapeGraphicsItem, m_tapesItem);
  tapeItem->setData(eTapeObjectType, eObjectTypeRole);
  return tapeItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MIRROR
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelObjectsListModel::AddMirror(Object* p_mirror, GraphicsObjectItem* p_mirrorGraphicsItem) {
  auto mirrorItem = AddObject(p_mirror, p_mirrorGraphicsItem, m_mirrorsItem);
  mirrorItem->setData(eTapeObjectType, eObjectTypeRole);
  return mirrorItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ONE WAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelObjectsListModel::AddOneWay(Object* p_oneWay, GraphicsObjectItem* p_oneWayGraphicsItem) {
  auto oneWayItem = AddObject(p_oneWay, p_oneWayGraphicsItem, m_oneWaysItem);
  oneWayItem->setData(eTapeObjectType, eObjectTypeRole);
  return oneWayItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PORTAL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QStandardItem* CreateLevelObjectsListModel::AddPortal(Object* p_portal, GraphicsObjectItem* p_portalGraphicsItem) {
  auto portalItem = AddObject(p_portal, p_portalGraphicsItem, m_portalsItem);
  portalItem->setData(eTapeObjectType, eObjectTypeRole);
  return portalItem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ALL
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateLevelObjectsListModel::Clear() {
  // Delete everything
  for (int listRow = 0; listRow < rowCount(); ++listRow) {
    auto listIndex = index(listRow, 0);
    for (int row = 0; row < rowCount(listIndex); ++row) {
      delete FindObjectFromIndex(index(row, 0, listIndex));
      delete FindPolygonFromIndex(index(row, 0, listIndex));
    }
  }

  // Clear model
  clear();

  // Reset default items
  SetDefaultItems();
}

void CreateLevelObjectsListModel::SetGraphicsToItem(GraphicsObjectItem* p_graphicsItem, QStandardItem* p_item) {
  p_item->setData(QVariant::fromValue<GraphicsObjectItem*>(p_graphicsItem), eGraphicsItemRole);
}

GraphicsObjectItem* CreateLevelObjectsListModel::GetGraphicsFromItem(QStandardItem* p_item) const {
  return GetGraphicsFromIndex(p_item->index());
}

GraphicsObjectItem* CreateLevelObjectsListModel::GetGraphicsFromIndex(QModelIndex const& p_index) const {
  return p_index.data(eGraphicsItemRole).value<GraphicsObjectItem*>();
}

CreateLevelObjectsListModel::ListType CreateLevelObjectsListModel::GetListTypFromIndex(QModelIndex const& p_index) const {
  return p_index.data(eListTypeRole).value<ListType>();
}

void CreateLevelObjectsListModel::SetDefaultItems() {
  m_polygonsItem = new QStandardItem("Polygons");
  m_polygonsItem->setData(true, eIsListRole);
  m_polygonsItem->setData(ePolygonsListType, eListTypeRole);
  appendRow(m_polygonsItem);
  m_tapesItem = new QStandardItem("Tapes");
  m_tapesItem->setData(true, eIsListRole);
  m_tapesItem->setData(eTapesListType, eListTypeRole);
  appendRow(m_tapesItem);
  m_mirrorsItem = new QStandardItem("Mirrors");
  m_mirrorsItem->setData(true, eIsListRole);
  m_mirrorsItem->setData(eMirrorsListType, eListTypeRole);
  appendRow(m_mirrorsItem);
  m_oneWaysItem = new QStandardItem("One Ways");
  m_oneWaysItem->setData(true, eIsListRole);
  m_oneWaysItem->setData(eOneWaysListType, eListTypeRole);
  appendRow(m_oneWaysItem);
  m_portalsItem = new QStandardItem("Portals");
  m_portalsItem->setData(true, eIsListRole);
  m_portalsItem->setData(ePortalsListType, eListTypeRole);
  appendRow(m_portalsItem);

  setColumnCount(1);
}
