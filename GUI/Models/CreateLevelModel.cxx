#include "CreateLevelModel.hxx"

CreateLevelModel::CreateLevelModel(QObject* p_parent):
  AbstractLevelModel(p_parent) {
}

CreateLevelModel::~CreateLevelModel() = default;

void CreateLevelModel::SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  GetPolygonItemsList().at(p_polygonRow)->setData(QVariant::fromValue<ppxl::Polygon>(p_polygon), ePolygonRole);
}

void CreateLevelModel::RemovePolygonAt(int p_polygonRow) {
  GetPolygonItemsList().removeAt(p_polygonRow);
}

void CreateLevelModel::TranslatePolygon(int p_polygonRow, const ppxl::Vector& p_direction) {
  // Update polygon AND polygon item
  auto polygonItem = GetPolygonItemsList().at(p_polygonRow);
  for (int row = 0; row < polygonItem->rowCount(); ++row) {
    TranslateVertex(p_polygonRow, row, p_direction);
  }
}

void CreateLevelModel::InsertVertex(int p_polygonRow, int p_vertexRow, const ppxl::Point& p_vertex) {
  auto polygonItem = GetPolygonItemsList().at(p_polygonRow);
  AbstractLevelModel::InsertVertex(p_vertexRow, polygonItem, p_vertex);
}

void CreateLevelModel::RemoveVertexAt(int p_polygonRow, int p_vertexRow) {
  // Update vertex
  auto polygonItem = GetPolygonItemsList().at(p_polygonRow);
  auto polygon = polygonItem->data(ePolygonRole).value<ppxl::Polygon>();
  auto vertices = polygon.GetVertices();
  vertices.erase(vertices.cbegin()+p_vertexRow);
  polygon.SetVertices(vertices);
  SetPolygon(p_polygonRow, polygon);
  polygonItem->removeRow(p_vertexRow);

  // Update vertex item
  polygonItem->removeRow(p_vertexRow);
}

void CreateLevelModel::TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction) {
  // Update polygon
  auto polygonItem = GetPolygonItemsList().at(p_polygonRow);
  auto polygon = polygonItem->data(ePolygonRole).value<ppxl::Polygon>();
  auto vertices = polygon.GetVertices();
  ppxl::Point translatedVertex;
  std::vector<ppxl::Point> newVertices;
  for (unsigned int row = 0; row < vertices.size(); ++row) {
    if (row == static_cast<unsigned int>(p_vertexRow)) {
      translatedVertex = vertices.at(row).Translated(p_direction);
      newVertices.push_back(translatedVertex);
    } else {
      newVertices.push_back(vertices.at(row));
    }
  }
  polygon.SetVertices(newVertices);
  SetPolygon(p_polygonRow, polygon);

  // Update polygon item
  auto vertexItemX = polygonItem->child(p_vertexRow, 1);
  vertexItemX->setText(QString::number(translatedVertex.GetX()));
  auto vertexItemY = polygonItem->child(p_vertexRow, 2);
  vertexItemY->setText(QString::number(translatedVertex.GetY()));
}

QModelIndex CreateLevelModel::GetVertexIndex(int p_polygonRow, int p_vertexRow) const {
  auto polygonItemsList = GetPolygonItemsList();
  if (polygonItemsList.size() > p_polygonRow) {
    auto polygonItem = polygonItemsList.at(p_polygonRow);
    if (polygonItem->rowCount() > p_vertexRow) {
      return polygonItem->child(p_vertexRow, 0)->index();
    }
  }

  return QModelIndex();
}
