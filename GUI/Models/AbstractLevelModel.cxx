#include "AbstractLevelModel.hxx"

AbstractLevelModel::AbstractLevelModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_polygonsItem(new QStandardItem("Polygons")) {

  setColumnCount(3);

  m_polygonsItem->setData(ePolygons, eItemTypeRole);
  appendRow(m_polygonsItem);
}

void AbstractLevelModel::InsertVertex(int p_row, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex) {
  // Update vertex item
  char vertexLetter = 'A';
  vertexLetter += p_row;

  auto vertexItem = new QStandardItem(QString(vertexLetter));
  vertexItem->setData(eVertex, eItemTypeRole);
  auto xItem = new QStandardItem(QString::number(p_vertex.GetX()));
  xItem->setData(eX, eItemTypeRole);
  auto yItem = new QStandardItem(QString::number(p_vertex.GetY()));
  yItem->setData(eY, eItemTypeRole);
  QList<QStandardItem*> vertexItemsList;
  vertexItemsList << vertexItem << xItem << yItem;
  p_polygonItem->insertRow(p_row, vertexItemsList);

  // Update vertices letters
  for (int row = p_row+1; row < p_polygonItem->rowCount(); ++row) {
    auto vertexItem = p_polygonItem->child(row, 0);
    char newLetter = vertexItem->text().toLatin1().data()[0];
    ++newLetter;
    vertexItem->setText(QString(newLetter));
  }

  // Update vertex
  auto polygon = p_polygonItem->data(ePolygonRole).value<ppxl::Polygon>();
  auto vertices = polygon.GetVertices();
  vertices.insert(vertices.cbegin()+p_row, p_vertex);
  polygon.SetVertices(vertices);
  p_polygonItem->setData(QVariant::fromValue<ppxl::Polygon>(polygon), ePolygonRole);
}

void AbstractLevelModel::AppendVertex(QStandardItem* p_polygonItem, ppxl::Point const& p_vertex) {
  InsertVertex(p_polygonItem->rowCount(), p_polygonItem, p_vertex);
}

AbstractLevelModel::~AbstractLevelModel() = default;

void AbstractLevelModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygonItem = new QStandardItem("Polygon #"+QString::number(p_row));
  polygonItem->setData(ePolygon, eItemTypeRole);
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon>(p_polygon), ePolygonRole);
  polygonItem->setData(QColor("#d64e9a"), Qt::DecorationRole);

  for (auto vertex: p_polygon.GetVertices()) {
    AppendVertex(polygonItem, vertex);
  }

  m_polygonsItem->insertRow(p_row, polygonItem);
}

void AbstractLevelModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_polygonsItem->rowCount(), p_polygon);
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
