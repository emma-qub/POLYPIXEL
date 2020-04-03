#ifndef CREATELEVELVERTEXLISTMODEL_HXX
#define CREATELEVELVERTEXLISTMODEL_HXX

#include <QStandardItemModel>

#include "Core/Geometry/Polygon.hxx"

class CreateLevelVertexListModel: public QStandardItemModel
{
  Q_OBJECT

public:
  enum ItemType {
    eVertexItemType,
    eXItemType,
    eYItemType
  };

  CreateLevelVertexListModel(QObject* p_parent = nullptr);
  ~CreateLevelVertexListModel() override;

  void ClearPolygon();
  void SetPolygon(ppxl::Polygon* p_polygon);
  ppxl::Polygon* GetPolygon() const;
  void Update();

private:
  ppxl::Polygon* m_polygon;
};

#endif
