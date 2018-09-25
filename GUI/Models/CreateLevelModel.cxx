#include "CreateLevelModel.hxx"

#include "Core/Vector.hxx"

CreateLevelModel::CreateLevelModel(QObject* p_parent):
  PolygonModel(p_parent),
  m_selections() {

  m_selections << QPair<int, int>(-1, -1);
}

CreateLevelModel::~CreateLevelModel() = default;

void CreateLevelModel::SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  auto* polygon = polygonItem->data(ePolygonRole).value<ppxl::Polygon*>();
  polygon->SetVertices(p_polygon.GetVertices());
}

void CreateLevelModel::TranslatePolygon(int p_polygonRow, const ppxl::Vector& p_direction) {
  // Update polygon AND polygon item
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  for (int row = 0; row < polygonItem->rowCount(); ++row) {
    TranslateVertex(p_polygonRow, row, p_direction);
  }
}

void CreateLevelModel::InsertVertex(int p_polygonRow, int p_vertexRow, const ppxl::Point& p_vertex, bool p_updatePolygon) {
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  PolygonModel::InsertVertex(p_vertexRow, polygonItem, p_vertex, p_updatePolygon);
}

void CreateLevelModel::RemoveVertexAt(int p_polygonRow, int p_vertexRow) {
  // Update vertex
  auto* polygon = GetPolygonsList().at(p_polygonRow);
  polygon->GetVertices().erase(polygon->GetVertices().begin());

  // Update vertex item
  GetPolygonsItem()->child(p_polygonRow)->removeRow(p_vertexRow);
}

void CreateLevelModel::TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction) {
  // Update polygon
  auto* polygon = GetPolygonsList().at(p_polygonRow);
  auto translatedVertex = polygon->GetVertices().at(static_cast<unsigned long>(p_vertexRow)).Translated(p_direction);

  // Update polygon item
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  auto vertexItemX = static_cast<VertexXItem*>(polygonItem->child(p_vertexRow, 1));
  vertexItemX->SetVertex(translatedVertex);
  auto vertexItemY = static_cast<VertexYItem*>(polygonItem->child(p_vertexRow, 2));
  vertexItemY->SetVertex(translatedVertex);

  Q_EMIT(dataChanged(
    index(p_vertexRow, 0, polygonItem->index()),
    index(p_vertexRow, 2, polygonItem->index())));
}

QModelIndex CreateLevelModel::GetVertexIndex(int p_polygonRow, int p_vertexRow) const {
  auto polygonsItem = GetPolygonsItem();
  auto polygonItem = polygonsItem->child(p_polygonRow, 0);
  if (polygonsItem->rowCount() > p_polygonRow && polygonItem->rowCount() > p_vertexRow) {
    return indexFromItem(polygonItem->child(p_vertexRow, 0));
  }

  return QModelIndex();
}
