#include "CreateLevelModel.hxx"

#include "Core/Vector.hxx"

CreateLevelModel::CreateLevelModel(QObject* p_parent):
  AbstractLevelModel(p_parent) {
}

CreateLevelModel::~CreateLevelModel() = default;

void CreateLevelModel::SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  auto* polygon = polygonItem->data(ePolygonRole).value<ppxl::Polygon*>();
  polygon->SetVertices(p_polygon.GetVertices());
}

void CreateLevelModel::TranslatePolygon(int p_polygonRow, const ppxl::Vector& p_direction, bool p_updatePolygon) {
  // Update polygon AND polygon item
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  for (int row = 0; row < polygonItem->rowCount(); ++row) {
    TranslateVertex(p_polygonRow, row, p_direction, p_updatePolygon);
  }
}

void CreateLevelModel::InsertVertex(int p_polygonRow, int p_vertexRow, const ppxl::Point& p_vertex, bool p_updatePolygon) {
  auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
  AbstractLevelModel::InsertVertex(p_vertexRow, polygonItem, p_vertex, p_updatePolygon);
}

void CreateLevelModel::RemoveVertexAt(int p_polygonRow, int p_vertexRow) {
  // Update vertex
  auto* polygon = GetPolygonsList().at(p_polygonRow);
  polygon->Vertices().erase(polygon->GetVertices().begin());

  // Update vertex item
  GetPolygonsItem()->child(p_polygonRow)->removeRow(p_vertexRow);
}

void CreateLevelModel::TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction, bool p_updatePolygon) {
  if (p_updatePolygon) {
    // Update polygon
    auto* polygon = GetPolygonsList().at(p_polygonRow);
    auto translatedVertex = polygon->Vertices().at(static_cast<unsigned long>(p_vertexRow)).Translated(p_direction);

    // Update polygon item
    auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
    auto vertexItemX = polygonItem->child(p_vertexRow, 1);
    vertexItemX->setText(QString::number(translatedVertex.GetX()));
    auto vertexItemY = polygonItem->child(p_vertexRow, 2);
    vertexItemY->setText(QString::number(translatedVertex.GetY()));
  } else {
    // Update polygon item
    auto polygonItem = GetPolygonsItem()->child(p_polygonRow, 0);
    auto vertexItemX = polygonItem->child(p_vertexRow, 1);
    vertexItemX->setText(QString::number(vertexItemX->text().toInt()+p_direction.GetX()));
    auto vertexItemY = polygonItem->child(p_vertexRow, 2);
    vertexItemY->setText(QString::number(vertexItemY->text().toInt()+p_direction.GetY()));
  }

}

QModelIndex CreateLevelModel::GetVertexIndex(int p_polygonRow, int p_vertexRow) const {
  auto polygonsItem = GetPolygonsItem();
  auto polygonItem = polygonsItem->child(p_polygonRow, 0);
  if (polygonsItem->rowCount() > p_polygonRow && polygonItem->rowCount() > p_vertexRow) {
    return indexFromItem(polygonItem->child(p_vertexRow, 0));
  }

  return QModelIndex();
}
