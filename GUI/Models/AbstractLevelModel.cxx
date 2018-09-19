#include "AbstractLevelModel.hxx"

AbstractLevelModel::AbstractLevelModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_polygonsItem(new QStandardItem("Polygons")) {

  m_polygonsItem->setData(ePolygons, eItemTypeRole);
  appendRow(m_polygonsItem);
}

QStandardItem* AbstractLevelModel::InsertVertex(int p_row, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex) {

  char i = 'A';
  i += p_row;
  auto vertexItem = new QStandardItem(QString(i));
  vertexItem->setData(eVertex, eItemTypeRole);
  auto xItem = new QStandardItem(QString::number(p_vertex.GetX()));
  xItem->setData(eX, eItemTypeRole);
  auto yItem = new QStandardItem(QString::number(p_vertex.GetY()));
  yItem->setData(eY, eItemTypeRole);
  vertexItem->appendRow(QList<QStandardItem*>() << xItem << yItem);
  p_polygonItem->insertRow(p_row, vertexItem);

  return vertexItem;
}

QStandardItem* AbstractLevelModel::AppendVertex(QStandardItem* p_polygonItem, ppxl::Point const& p_vertex) {
  return InsertVertex(p_polygonItem->rowCount(), p_polygonItem, p_vertex);
}

AbstractLevelModel::~AbstractLevelModel() = default;

QStandardItem* AbstractLevelModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygonItem = new QStandardItem("Polygon #"+QString::number(p_row));
  polygonItem->setData(ePolygon, eItemTypeRole);
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon>(p_polygon), ePolygonRole);
  polygonItem->setData(QColor("#d64e9a"), Qt::DecorationRole);

  for (auto vertex: p_polygon.GetVertices()) {
    AppendVertex(polygonItem, vertex);
  }

  m_polygonsItem->insertRow(p_row, polygonItem);
  return polygonItem;
}

QStandardItem* AbstractLevelModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  return InsertPolygon(m_polygonsItem->rowCount(), p_polygon);
}

QList<QStandardItem*> AbstractLevelModel::GetPolygonItemsList() const
{
  QList<QStandardItem*> polygonItems;

  for (int row = 0; row < m_polygonsItem->rowCount(); ++row) {
    polygonItems << m_polygonsItem->child(row, 0);
  }

  return polygonItems;
}

QList<ppxl::Polygon> AbstractLevelModel::GetPolygonsList() const
{
  QList<ppxl::Polygon> polygonsList;
  for (int row = 0; row < m_polygonsItem->rowCount(); ++row) {
    polygonsList << m_polygonsItem->child(row, 0)->data(ePolygonRole).value<ppxl::Polygon>();
  }

  return polygonsList;
}

void AbstractLevelModel::SetPolygonsList(const QList<ppxl::Polygon>& p_polygonsList)
{
  ClearPolygons();
  for (auto const& polygon: p_polygonsList) {
    AppendPolygon(polygon);
  }

  Q_EMIT(PolygonListChanged());
}

void AbstractLevelModel::ClearPolygons() {
  for (int row = m_polygonsItem->rowCount()-1; row > -1; --row)
  {
    for (auto* item: m_polygonsItem->takeRow(row))
    {
      delete item;
    }
  }
}
