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
  void InsertVertex(int p_row, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex);
  void AppendVertex(QStandardItem* p_polygonItem, const ppxl::Point& p_vertex);

  // Polygon
  void InsertPolygon(int p_row, ppxl::Polygon const& p_polygon);
  void AppendPolygon(const ppxl::Polygon& p_polygon);
  inline int GetPolygonsCount() const { return m_polygonsItem->rowCount(); }
  QList<QStandardItem*> GetPolygonItemsList() const;
  QList<ppxl::Polygon> GetPolygonsList() const;
  void SetPolygonsList(QList<ppxl::Polygon> const& p_polygonsList);
  void ClearPolygons();

signals:
  void PolygonListChanged();

private:
  QStandardItem* m_polygonsItem;
};

Q_DECLARE_METATYPE(ppxl::Polygon)
Q_DECLARE_METATYPE(AbstractLevelModel::ItemType);

#endif
