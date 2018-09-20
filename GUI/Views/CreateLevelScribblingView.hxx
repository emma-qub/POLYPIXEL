#ifndef CREATELEVELSCRIBBLINGVIEW_HXX
#define CREATELEVELSCRIBBLINGVIEW_HXX

#include "GUI/Views/AbstractScribblingView.hxx"
#include "Core/Point.hxx"
#include "Core/Vector.hxx"
#include "Core/Polygon.hxx"


class CreateLevelModel;
class QItemSelectionModel;

class CreateLevelScribblingView: public AbstractScribblingView {
  Q_OBJECT

public:
  CreateLevelScribblingView(QWidget* parent = nullptr);
  ~CreateLevelScribblingView() override;

  void SetModel(AbstractLevelModel* p_model) override;
  void SetSelectionModel(QItemSelectionModel* p_selectionModel);

  void DrawGrid();
  void DrawFromModel() override;

signals:
  void PolygonInserted(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void PolygonRemoved(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void PolygonMoved(int p_polygonRow, ppxl::Vector const& p_direction, bool p_pushToStack);
  void VertexInserted(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void VertexRemoved(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void VertexMoved(int p_polygonRow, int p_vertexRow, const ppxl::Vector& p_direction, bool p_pushToStack);

protected:
  void DrawPoint(QPoint const& p_point, QColor const& p_color);
  void DrawText(ppxl::Point p_position, const QString& p_text, const ppxl::Vector& shiftVector, int p_weight);
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private:
  CreateLevelModel* m_model;
  QItemSelectionModel* m_selectionModel;
  bool m_isMagnetic;
  bool m_isStuck;
  bool m_nearToVertex;
  bool m_nearToBarycenter;
  bool m_movingVertex;
  bool m_movingPolygon;
  bool m_lengthOn;
  bool m_angleOn;
  int m_currPolygonRow;
  int m_currVertexRow;
  int m_beforeMovingVertexX;
  int m_beforeMovingVertexY;
  int m_beforeMovingPolygonX;
  int m_beforeMovingPolygonY;
  int m_currOldX;
  int m_currOldY;

  static const int PEN_WIDTH;
  static const QColor NOT_SELECTED_COLOR;
};

#endif // CREATELEVELSCRIBBLINGVIEW_HXX
