#ifndef POLYGONITEM_HXX
#define POLYGONITEM_HXX

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Polygon.hxx"

#include <QStandardItem>

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

#endif
