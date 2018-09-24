#include "PolygonModel.hxx"

#include <random>

PolygonModel::PolygonModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_polygonsItem(new QStandardItem("Polygons")) {

  setColumnCount(3);

  m_polygonsItem->setData(ePolygons, eItemTypeRole);
  appendRow(m_polygonsItem);
}

PolygonModel::~PolygonModel() {
  for (auto* polygon: m_polygons) {
    delete polygon;
  }
}

void PolygonModel::InsertVertex(int p_row, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex, bool p_updatePolygon) {
  auto polygon = p_polygonItem->data(ePolygonRole).value<ppxl::Polygon*>();

  // Insert vertex
  if (p_updatePolygon) {
    polygon->InsertVertex(p_vertex, static_cast<unsigned int>(p_row));
  }

  // Insert vertex row
  auto& vertex = polygon->GetVertices()[static_cast<unsigned long>(p_row)];

  auto* vertexXItem = new VertexXItem(vertex);
  vertexXItem->setData(eX, eItemTypeRole);

  auto* vertexYItem = new VertexYItem(vertex);
  vertexYItem->setData(eY, eItemTypeRole);

  auto* vertexItem = new VertexLabelItem;
  vertexItem->setData(eVertex, eItemTypeRole);

  QList<QStandardItem*> vertexItemsList;
  vertexItemsList << vertexItem << vertexXItem << vertexYItem;
  p_polygonItem->insertRow(p_row, vertexItemsList);
}

void PolygonModel::AppendVertex(QStandardItem* p_polygonItem, ppxl::Point const& p_vertex, bool p_updatePolygon) {
  InsertVertex(p_polygonItem->rowCount(), p_polygonItem, p_vertex, p_updatePolygon);
}

void PolygonModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygonItem = new QStandardItem("Polygon #"+QString::number(p_row));
  auto polygon = new ppxl::Polygon(p_polygon);
  polygonItem->setData(ePolygon, eItemTypeRole);
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon*>(polygon), ePolygonRole);
  polygonItem->setData(QColor("#d64e9a"), Qt::DecorationRole);

  for (auto vertex: polygon->GetVertices()) {
    AppendVertex(polygonItem, vertex, false);
  }

  m_polygonsItem->insertRow(p_row, QList<QStandardItem*>() << polygonItem << new QStandardItem << new QStandardItem);
  m_polygons << polygon;
}

void PolygonModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_polygonsItem->rowCount(), p_polygon);
}

void PolygonModel::SetPolygonsList(const QList<ppxl::Polygon>& p_polygonsList)
{
  ClearPolygons();
  for (auto const& polygon: p_polygonsList) {
    AppendPolygon(polygon);
  }

  Q_EMIT(PolygonListChanged());
}

void PolygonModel::RemovePolygonAt(int p_polygonRow) {
  delete m_polygons.takeAt(p_polygonRow);
  for (auto* item: m_polygonsItem->takeRow(p_polygonRow))
  {
    delete item;
  }
}

void PolygonModel::ClearPolygons() {
  for (int row = m_polygonsItem->rowCount()-1; row > -1; --row)
  {
    RemovePolygonAt(row);
  }
  m_polygons.clear();
  m_polygonsItem->setRowCount(0);
}



VertexXItem::VertexXItem(ppxl::Point p_vertex):
  QStandardItem(),
  m_vertex(p_vertex) {
}

VertexXItem::~VertexXItem() = default;

QVariant VertexXItem::data(int p_role) const {
  if (p_role == Qt::DisplayRole) {
    return QVariant(QString::number(static_cast<int>(m_vertex.GetX())));
  }

  return QStandardItem::data(p_role);
}



VertexYItem::VertexYItem(ppxl::Point p_vertex):
  QStandardItem(),
  m_vertex(p_vertex) {
}

VertexYItem::~VertexYItem() = default;

QVariant VertexYItem::data(int p_role) const {
  if (p_role == Qt::DisplayRole) {
    return QVariant(QString::number(static_cast<int>(m_vertex.GetY())));
  }

  return QStandardItem::data(p_role);
}



VertexLabelItem::VertexLabelItem():
  QStandardItem() {
}

VertexLabelItem::~VertexLabelItem() = default;

QVariant VertexLabelItem::data(int p_role) const {
  if (p_role == Qt::DisplayRole) {
    char i = 'A';
    i += row();
    return QVariant(QString(i));
  }

  return QStandardItem::data(p_role);
}



PolygonItem::PolygonItem(ppxl::Polygon* p_polygon, QString const& p_text):
  QStandardItem(p_text),
  m_polygon(p_polygon) {

  for (auto& vertex: m_polygon->GetVertices()) {
    auto vertexLabelItem = new VertexLabelItem;
    auto vertexXItem = new VertexXItem(vertex);
    auto vertexYItem = new VertexYItem(vertex);

    appendRow(QList<QStandardItem*>() << vertexLabelItem << vertexXItem << vertexYItem);
  }
}

PolygonItem::~PolygonItem() = default;

QVariant PolygonItem::data(int p_role) const {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> distribution(0, 255);

  auto r = distribution(rng);
  auto g = distribution(rng);
  auto b = distribution(rng);

  if (p_role == Qt::DecorationRole) {
    return QVariant(QColor(r, g, b));
  }

  return QStandardItem::data(p_role);
}



PolygonItemModel::PolygonItemModel(QObject* p_parent):
  QStandardItemModel(p_parent) {
}

PolygonItemModel::~PolygonItemModel() = default;

void PolygonItemModel::InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex) {

  auto* polygonItem = static_cast<PolygonItem*>(item(p_polygonRow, 0));
  auto* polygon = polygonItem->GetPolygon();
  polygon->InsertVertex(p_vertex, static_cast<unsigned int>(p_vertexRow));
  auto& vertex = polygon->GetVertices()[static_cast<unsigned long>(p_vertexRow)];

  auto vertexLabelItem = new VertexLabelItem;
  auto vertexXItem = new VertexXItem(vertex);
  auto vertexYItem = new VertexYItem(vertex);
  polygonItem->insertRow(p_vertexRow, QList<QStandardItem*>() << vertexLabelItem << vertexXItem << vertexYItem);
}

void PolygonItemModel::AppendVertex(int p_polygonRow, ppxl::Point const& p_vertex) {
  InsertVertex(p_polygonRow, rowCount(index(p_polygonRow, 0)), p_vertex);
}

void PolygonItemModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  insertRow(p_row, new PolygonItem(new ppxl::Polygon(p_polygon), "Polygon "+QString::number(p_row)));
}

void PolygonItemModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(rowCount(), p_polygon);
}

void PolygonItemModel::RemovePolygonAt(int p_row) {
  delete takeItem(p_row);
}

void PolygonItemModel::ClearPolygons() {
  for (int row = rowCount()-1; row > -1; --row) {
    RemovePolygonAt(row);
  }

  setRowCount(0);
  setColumnCount(0);
}

void PolygonItemModel::SetPolygons(QList<ppxl::Polygon> const& p_polygons) {
  ClearPolygons();
  for (auto const& polygon: p_polygons) {
    AppendPolygon(polygon);
  }
}
