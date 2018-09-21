#ifndef ABSTRACTLEVELMODEL_HXX
#define ABSTRACTLEVELMODEL_HXX

#include <QStandardItemModel>

#include "Core/Polygon.hxx"

class AbstractLevelModel: public QStandardItemModel {
  Q_OBJECT

public:
  enum ItemRoles {
    ePolygonRole = Qt::UserRole,
    eItemTypeRole
  };
  enum ItemType {
    ePolygons,
    ePolygon,
    eVertex,
    eX,
    eY
  };

  AbstractLevelModel(QObject* p_parent = nullptr);
  ~AbstractLevelModel() override;

  // Vertex
  void InsertVertex(int p_row, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex, bool p_updatePolygon);
  void AppendVertex(QStandardItem* p_polygonItem, const ppxl::Point& p_vertex, bool p_updatePolygon);

  // Polygon
  void InsertPolygon(int p_row, ppxl::Polygon const& p_polygon);
  void AppendPolygon(const ppxl::Polygon& p_polygon);
  inline int GetPolygonsCount() const { return m_polygonsItem->rowCount(); }
  inline QStandardItem* GetPolygonsItem() const { return m_polygonsItem; }
  inline QList<ppxl::Polygon*> GetPolygonsList() const { return m_polygons; }
  void SetPolygonsList(QList<ppxl::Polygon> const& p_polygonsList);
  void RemovePolygonAt(int p_polygonRow);
  void ClearPolygons();

signals:
  void PolygonListChanged();

private:
  QStandardItem* m_polygonsItem;
  QList<ppxl::Polygon*> m_polygons;
};

Q_DECLARE_METATYPE(ppxl::Polygon*)
Q_DECLARE_METATYPE(AbstractLevelModel::ItemType);

#endif
