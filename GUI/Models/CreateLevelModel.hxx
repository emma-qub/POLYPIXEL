#ifndef CREATELEVELMODEL_HXX
#define CREATELEVELMODEL_HXX

#include "GUI/Models/PolygonModel.hxx"

class CreateLevelModel: public PolygonModel {
  Q_OBJECT

public:
  CreateLevelModel(QObject* p_parent = nullptr);
  ~CreateLevelModel() override;

  // Polygon
  void SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void TranslatePolygon(int p_polygonRow, ppxl::Vector const& p_direction);

  // Vertex
  void InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex, bool p_updatePolygon);
  void RemoveVertexAt(int p_polygonRow, int p_vertexRow);
  void TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction);
  QModelIndex GetVertexIndex(int p_polygonRow, int p_vertexRow) const;
};

#endif
