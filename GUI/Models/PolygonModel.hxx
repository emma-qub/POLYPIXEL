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
  QList<ppxl::Polygon*> m_polygons;
  QStandardItem* m_polygonsItem;
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
  explicit PolygonItem(ppxl::Polygon* p_polygon, const QString& p_text);
  ~PolygonItem() override;

  QVariant data(int p_role = Qt::UserRole + 1) const override;

  inline ppxl::Polygon* GetPolygon() const { return m_polygon; }

private:
  ppxl::Polygon* m_polygon;
};

class PolygonItemModel: public QStandardItemModel {
  Q_OBJECT

public:
  explicit PolygonItemModel(QObject* p_parent = nullptr);
  ~PolygonItemModel() override;

  // Vertex
  void InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void AppendVertex(int p_polygonRow, ppxl::Point const& p_vertex);

  // Polygon
  void InsertPolygon(int p_row, ppxl::Polygon const& p_polygon);
  void AppendPolygon(ppxl::Polygon const& p_polygon);
  void RemovePolygonAt(int p_row);
  void ClearPolygons();
  void SetPolygons(QList<ppxl::Polygon> const& p_polygons);
};

Q_DECLARE_METATYPE(ppxl::Polygon*)
Q_DECLARE_METATYPE(PolygonModel::ItemType);

#endif