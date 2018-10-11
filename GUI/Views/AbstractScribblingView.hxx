#ifndef ABSTRACTSCRIBBLINGVIEW2_HXX
#define ABSTRACTSCRIBBLINGVIEW2_HXX

#include <QGraphicsView>
#include <QGraphicsPolygonItem>

#include "Core/Segment.hxx"
#include "Core/Vector.hxx"

class PolygonModel;
class Object;

class GraphicsPolygonItem: public QObject, public QGraphicsPolygonItem {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
  explicit GraphicsPolygonItem(QGraphicsItem* p_parent = nullptr): QGraphicsPolygonItem(p_parent) {}
  explicit GraphicsPolygonItem(QPolygonF const& p_polygon, QGraphicsItem* p_parent = nullptr):
    QGraphicsPolygonItem(p_polygon, p_parent) {}

  ~GraphicsPolygonItem() override = default;

};

class AbstractScribblingView: public QGraphicsView {
  Q_OBJECT

public:
  AbstractScribblingView(QWidget* p_parent = nullptr);
  ~AbstractScribblingView() override;

  virtual void InitView();

  virtual void SetModel(PolygonModel* p_model);
  virtual void SetObjectsList(QList<Object*> const& p_objectsList);
  void SetCanScribble(bool p_value);
  bool GetCanScribble() const;
  inline QList<GraphicsPolygonItem*> GetGraphicsPolygonList() const { return m_graphicsPolygonList; }

  void DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void DrawText(ppxl::Point p_position, const QString& p_text, int p_weight, const ppxl::Vector& shiftVector = ppxl::Vector());
  virtual void DrawFromModel();
  void DrawObjects();

  void ClearImage();

protected:
  inline PolygonModel* GetModel() const { return m_polygonModel; }
  inline void setPenWidth(int p_width) { m_penWidth = p_width; }
  inline QGraphicsScene* GetScene() const { return m_scene; }

  void DrawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color);
  void DrawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color);

private:
  QGraphicsScene* m_scene;
  int m_penWidth;
  PolygonModel* m_polygonModel;
  QList<Object*> m_objectsList;
  QColor m_penColor;
  QPen m_pen;
  bool m_canScribble;
  bool m_viewInitialized;
  QList<GraphicsPolygonItem*> m_graphicsPolygonList;
};

#endif
