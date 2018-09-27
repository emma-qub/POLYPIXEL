#ifndef ABSTRACTLEVELMODEL_HXX
#define ABSTRACTLEVELMODEL_HXX

#include <QStandardItemModel>

#include "Core/Polygon.hxx"

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
  QList<ppxl::Polygon*> m_polygons;
  QStandardItem* m_polygonsItem;
  QColor m_color;
};



class VertexXItem: public QStandardItem {
public:
  explicit VertexXItem(ppxl::Point p_vertex);
  ~VertexXItem() override;

  QVariant data(int p_role = Qt::UserRole + 1) const override;

  void SetVertex(ppxl::Point p_vertex) { m_vertex = p_vertex; }

private:
  ppxl::Point m_vertex;
};

class VertexYItem: public QStandardItem {
public:
  explicit VertexYItem(ppxl::Point p_vertex);
  ~VertexYItem() override;

  QVariant data(int p_role = Qt::UserRole + 1) const override;

  void SetVertex(ppxl::Point p_vertex) { m_vertex = p_vertex; }

private:
  ppxl::Point m_vertex;
};

class VertexLabelItem: public QStandardItem {
public:
  explicit VertexLabelItem();
  ~VertexLabelItem() override;

  QVariant data(int p_role = Qt::UserRole + 1) const override;
};

class PolygonItem: public QStandardItem {
public:
  explicit PolygonItem(ppxl::Polygon* p_polygon, const QColor& p_color = QColor());
  ~PolygonItem() override;

  QVariant data(int p_role = Qt::UserRole + 1) const override;

  inline ppxl::Polygon* GetPolygon() const { return m_polygon; }

private:
  ppxl::Polygon* m_polygon;
  QColor m_color;
};

Q_DECLARE_METATYPE(ppxl::Polygon*)
Q_DECLARE_METATYPE(PolygonModel::ItemType);

#endif
