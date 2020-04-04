#include "CreateLevelVertexListModel.hxx"

CreateLevelVertexListModel::CreateLevelVertexListModel(QObject* p_parent):
  QStandardItemModel(p_parent) {
}

CreateLevelVertexListModel::~CreateLevelVertexListModel() = default;

void CreateLevelVertexListModel::ClearPolygon() {
  clear();
  m_polygon = nullptr;
}

void CreateLevelVertexListModel::SetPolygon(ppxl::Polygon* p_polygon) {
  setRowCount(0);
  setColumnCount(3);

  m_polygon = p_polygon;

  auto vertices = m_polygon->GetVertices();
  for (auto row = 0ul; row < vertices.size(); ++row) {
    auto vertex = vertices.at(row);
    auto vertexItem = new QStandardItem(QString(QChar(static_cast<char>('A'+row))));
    auto vertexXItem = new QStandardItem(QString::number(vertex.GetX(), 'f', 0));
    auto vertexYItem = new QStandardItem(QString::number(vertex.GetY(), 'f', 0));
    appendRow({vertexItem, vertexXItem, vertexYItem});
  }
}

ppxl::Polygon* CreateLevelVertexListModel::GetPolygon() const {
  return m_polygon;
}

void CreateLevelVertexListModel::Update() {
  auto vertices = m_polygon->GetVertices();
  for (auto row = 0ul; row < vertices.size(); ++row) {
    auto vertex = vertices.at(row);
    item(row, 0)->setText(QString(QChar(static_cast<char>('A'+row))));
    item(row, 1)->setText(QString::number(vertex.GetX(), 'f', 0));
    item(row, 2)->setText(QString::number(vertex.GetY(), 'f', 0));
  }
}
