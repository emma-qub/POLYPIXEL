#ifndef CREATELEVELSCRIBBLINGVIEW_HXX
#define CREATELEVELSCRIBBLINGVIEW_HXX

#include "GUI/Views/AbstractScribblingView.hxx"
#include "Core/Point.hxx"
#include "Core/Vector.hxx"
#include "Core/Polygon.hxx"

class CreateLevelModel;
class QItemSelectionModel;
class QStandardItem;

class CreateLevelScribblingView: public AbstractScribblingView {
  Q_OBJECT

public:
  CreateLevelScribblingView(QWidget* parent = nullptr);
  ~CreateLevelScribblingView() override;

  void InitView() override;

  void SetModel(PolygonModel* p_model) override;
  void SetSelectionModel(QItemSelectionModel* p_selectionModel);
  inline QItemSelectionModel* GetSelectionModel() const { return m_selectionModel; }

  void DrawGrid();
  void DrawFromModel() override;
  void DrawFromCore();

signals:
  void PolygonInserted(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void PolygonRemoved(int p_polygonRow);
  void PolygonMoved(int p_polygonRow, ppxl::Vector const& p_direction, bool p_pushToStack);
  void VertexInserted(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void VertexRemoved(int p_polygonRow, int p_vertexRow);
  void VertexMoved(int p_polygonRow, int p_vertexRow, const ppxl::Vector& p_direction, bool p_pushToStack);
  void PolygonSelected();
  void SnappedToGrid();
  void NewLevelRequested();
  void OpenLevelRequested(QString const& p_fileName);

protected:
  ppxl::Polygon* GetCurrentPolygon() const;
  void InsertPolygon();
  void Remove();
  void DrawPoint(QPoint const& p_point, QColor const& p_color);
  void mousePressEvent(QMouseEvent* p_event) override;
  void mouseMoveEvent(QMouseEvent* p_event) override;
  void mouseReleaseEvent(QMouseEvent* p_event) override;
  bool ConfirmClear();
  void ConfirmNewLevel();
  void ConfirmOpenLevel();

  void DrawPolygonsFromCore();
  void DrawPolygonFromCore(QStandardItem* p_polygonItem, bool p_isSelectedPolygon);
  void DrawPolygonsFromModel();
  void DrawPolygonFromModel(QStandardItem* p_polygonItem, bool p_isSelectedPolygon);

private:
  QPixmap m_gripPixmap;
  CreateLevelModel* m_polygonModel;
  QItemSelectionModel* m_selectionModel;
  bool m_viewInitialized;
  bool m_isStuck;
  bool m_nextToVertex;
  bool m_nextToBarycenter;
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
  int m_startShiftX;
  int m_startShiftY;

  static const int PEN_WIDTH;
  static const QColor NOT_SELECTED_COLOR;
};

#endif
