#include "AbstractLevelModel.hxx"

AbstractLevelModel::AbstractLevelModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_polygonsItem(new QStandardItem("Polygons")) {

  setColumnCount(3);

  m_polygonsItem->setData(ePolygons, eItemTypeRole);
  appendRow(m_polygonsItem);
}

AbstractLevelModel::~AbstractLevelModel() {
  for (auto* polygon: m_polygons) {
    delete polygon;
  }
}

void AbstractLevelModel::InsertVertex(int p_row, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex, bool p_updatePolygon) {
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
  if (p_updatePolygon) {
    auto polygon = p_polygonItem->data(ePolygonRole).value<ppxl::Polygon*>();
    //polygon->Vertices().insert(polygon->GetVertices().begin()+p_row, p_vertex);
    polygon->Vertices().push_back(p_vertex);
  }
}

void AbstractLevelModel::AppendVertex(QStandardItem* p_polygonItem, ppxl::Point const& p_vertex, bool p_updatePolygon) {
  InsertVertex(p_polygonItem->rowCount(), p_polygonItem, p_vertex, p_updatePolygon);
}

void AbstractLevelModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygonItem = new QStandardItem("Polygon #"+QString::number(p_row));
  auto polygon = new ppxl::Polygon(p_polygon);
  polygonItem->setData(ePolygon, eItemTypeRole);
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon*>(polygon), ePolygonRole);
  polygonItem->setData(QColor("#d64e9a"), Qt::DecorationRole);

  for (auto vertex: polygon->GetVertices()) {
    AppendVertex(polygonItem, vertex, false);
  }

  m_polygonsItem->insertRow(p_row, polygonItem);
}

void AbstractLevelModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_polygonsItem->rowCount(), p_polygon);
}

void AbstractLevelModel::SetPolygonsList(const QList<ppxl::Polygon>& p_polygonsList)
{
  ClearPolygons();
  for (auto const& polygon: p_polygonsList) {
    AppendPolygon(polygon);
  }

  Q_EMIT(PolygonListChanged());
}

void AbstractLevelModel::RemovePolygonAt(int p_polygonRow) {
  for (auto* item: m_polygonsItem->takeRow(p_polygonRow))
  {
    delete item->data(ePolygonRole).value<ppxl::Polygon*>();
    delete item;
  }
}

void AbstractLevelModel::ClearPolygons() {
  for (int row = m_polygonsItem->rowCount()-1; row > -1; --row)
  {
    RemovePolygonAt(row);
  }
}
