#include "PolygonItem.hxx"

#include <random>

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
