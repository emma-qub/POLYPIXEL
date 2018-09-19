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
  auto polygon = GetPolygonsList().at(p_polygonRow);
  polygon.Translate(p_direction);
  SetPolygon(p_polygonRow, polygon);
}

void CreateLevelModel::InsertVertex(int p_polygonRow, int p_vertexRow, const ppxl::Point& p_vertex) {
  auto polygonItem = GetPolygonItemsList().at(p_polygonRow);
  AbstractLevelModel::InsertVertex(p_vertexRow, polygonItem, p_vertex);
}

void CreateLevelModel::RemoveVertexAt(int p_polygonRow, int p_vertexRow) {
  auto polygonItem = GetPolygonItemsList().at(p_polygonRow);
  auto polygon = polygonItem->data(ePolygonRole).value<ppxl::Polygon>();
  auto vertices = polygon.GetVertices();
  std::vector<ppxl::Point> newVertices;
  for (unsigned int row = 0; row < vertices.size(); ++row) {
    if (row != static_cast<unsigned int>(p_vertexRow)) {
      newVertices.push_back(vertices.at(row));
    }
  }
  polygon.SetVertices(newVertices);
  SetPolygon(p_polygonRow, polygon);
}

void CreateLevelModel::TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction) {
  auto polygonItem = GetPolygonItemsList().at(p_polygonRow);
  auto polygon = polygonItem->data(ePolygonRole).value<ppxl::Polygon>();
  auto vertices = polygon.GetVertices();
  std::vector<ppxl::Point> newVertices;
  for (unsigned int row = 0; row < vertices.size(); ++row) {
    if (row == static_cast<unsigned int>(p_vertexRow)) {
      newVertices.push_back(vertices.at(row).Translated(p_direction));
    } else {
      newVertices.push_back(vertices.at(row));
    }
  }
  polygon.SetVertices(newVertices);
  SetPolygon(p_polygonRow, polygon);
}
