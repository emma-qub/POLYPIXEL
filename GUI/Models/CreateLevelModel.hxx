#ifndef CREATELEVELMODEL_HXX
#define CREATELEVELMODEL_HXX

#include "GUI/Models/AbstractLevelModel.hxx"

class CreateLevelModel: public AbstractLevelModel {
  Q_OBJECT

public:
  CreateLevelModel(QObject* p_parent = nullptr);
  ~CreateLevelModel() override;

  // Polygon
  void SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void RemovePolygonAt(int p_polygonRow);
  void TranslatePolygon(int p_polygonRow, ppxl::Vector const& p_direction);

  // Vertex
  void InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void RemoveVertexAt(int p_polygonRow, int p_vertexRow);
  void TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction);
};

#endif
