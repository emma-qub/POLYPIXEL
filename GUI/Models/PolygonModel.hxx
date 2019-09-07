#ifndef ABSTRACTLEVELMODEL_HXX
#define ABSTRACTLEVELMODEL_HXX

#include <QStandardItemModel>

#include "Core/Geometry/Polygon.hxx"

class PolygonModel: public QStandardItemModel {
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

  PolygonModel(QObject* p_parent = nullptr);
  ~PolygonModel() override;

  inline QColor GetColor() const { return m_color; }
  void InitColor();

  void InsertPolygon(int p_row, ppxl::Polygon const& p_polygon);
  void AppendPolygon(ppxl::Polygon const& p_polygon);
  inline int GetPolygonsCount() const { return m_polygonsItem->rowCount(); }
  inline QStandardItem* GetPolygonsItem() const { return m_polygonsItem; }
  inline QList<ppxl::Polygon*> GetPolygonsList() const { return m_polygons; }
  void SetPolygonsList(QList<ppxl::Polygon> const& p_polygonsList);
  void RemovePolygonAt(int p_polygonRow);
  void ClearPolygons();

signals:
  void PolygonListChanged();

private:
  QList<ppxl::Polygon*> m_polygons;
  QStandardItem* m_polygonsItem;
  QColor m_color;
};

Q_DECLARE_METATYPE(ppxl::Polygon*)
Q_DECLARE_METATYPE(PolygonModel::ItemType);

#endif
