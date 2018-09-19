#include "CreateLevelModel.hxx"

CreateLevelModel::CreateLevelModel(QObject* p_parent):
  AbstractLevelModel(p_parent) {
}

CreateLevelModel::~CreateLevelModel() = default;

void CreateLevelModel::SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  auto polygonItemsList = GetPolygonItemsList();
  polygonItemsList.at(p_polygonRow)->setData(QVariant::fromValue<ppxl::Polygon>(p_polygon), ePolygonRole);
}

void CreateLevelModel::SetVertex(QStandardItem* p_polygonItem, int p_vertexRow, const ppxl::Point& p_vertex) {
  auto polygon = p_polygonItem->data(ePolygonRole).value<ppxl::Polygon>();
  auto vertices = polygon.GetVertices();
  std::vector<ppxl::Point> newVertices;
  for (unsigned int row = 0; row < vertices.size(); ++row) {
    if (row == static_cast<unsigned int>(p_vertexRow)) {
      newVertices.push_back(p_vertex);
    } else {
      newVertices.push_back(vertices.at(row));
    }
  }
  polygon.SetVertices(newVertices);
  SetPolygon(p_polygonItem->row(), polygon);
}
