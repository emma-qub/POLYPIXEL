#include "PolygonModel.hxx"

#include <random>

PolygonModel::PolygonModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_polygonsItem(new QStandardItem("Polygons")),
  m_color() {

  setColumnCount(3);

  m_polygonsItem->setData(ePolygons, eItemTypeRole);
  appendRow(m_polygonsItem);
}

PolygonModel::~PolygonModel() {
  for (auto* polygon: m_polygons) {
    delete polygon;
  }
}

void PolygonModel::InitColor() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> distribution(0, 255);

  auto r = distribution(rng);
  auto g = distribution(rng);
  auto b = distribution(rng);

  m_color = QColor(r, g, b);
}

void PolygonModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygon = new ppxl::Polygon(p_polygon);
  auto polygonItem = new PolygonItem(polygon, m_color);
  polygonItem->setData(ePolygon, eItemTypeRole);
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon*>(polygon), ePolygonRole);

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
  m_polygonsItem->setColumnCount(0);
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



PolygonItem::PolygonItem(ppxl::Polygon* p_polygon, QColor const& p_color):
  QStandardItem(),
  m_polygon(p_polygon),
  m_color(p_color) {

  for (auto& vertex: m_polygon->GetVertices()) {
    auto vertexLabelItem = new VertexLabelItem;
    auto vertexXItem = new VertexXItem(vertex);
    auto vertexYItem = new VertexYItem(vertex);

    appendRow(QList<QStandardItem*>() << vertexLabelItem << vertexXItem << vertexYItem);
  }

  if (!m_color.isValid()) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> distribution(0, 255);

    auto r = distribution(rng);
    auto g = distribution(rng);
    auto b = distribution(rng);

    m_color = QColor(r, g, b);
  }
}

PolygonItem::~PolygonItem() = default;

QVariant PolygonItem::data(int p_role) const {
  if (p_role == Qt::DecorationRole) {
    return m_color;
  } else if (p_role == Qt::DisplayRole) {
    return "Polygon " + QString::number(row());
  }

  return QStandardItem::data(p_role);
}
