#ifndef CREATELEVELMODEL_HXX
#define CREATELEVELMODEL_HXX

#include "GUI/Models/AbstractLevelModel.hxx"

class CreateLevelModel: public AbstractLevelModel {
  Q_OBJECT

public:
  CreateLevelModel(QObject* p_parent = nullptr);
  ~CreateLevelModel() override;

  void SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void SetVertex(QStandardItem* p_polygonItem, int p_vertexRow, ppxl::Point const& p_vertex);
};

#endif
