#ifndef CREATELEVELSCRIBBLINGVIEW_HXX
#define CREATELEVELSCRIBBLINGVIEW_HXX

#include "GUI/Views/AbstractScribblingView.hxx"
#include "Core/Point.hxx"
#include "Core/Vector.hxx"
#include "Core/Polygon.hxx"

class CreateLevelModel;
class ObjectModel;
class QItemSelectionModel;
class QStandardItem;

class CreateLevelScribblingView: public AbstractScribblingView {
  Q_OBJECT

public:
  enum ToolMode {
    ePolygonMode,
    eTapeMode,
    eMirrorMode,
    eOneWayMode,
    ePortalMode
  };

  CreateLevelScribblingView(QWidget* parent = nullptr);
  ~CreateLevelScribblingView() override;

  void InitView() override;

  void SetPolygonModel(PolygonModel* p_model) override;
  void SetObjectModelsList(QList<ObjectModel*> const& p_objectModelsList);
  void SetSelectionModel(QItemSelectionModel* p_selectionModel);

  inline void SetToolMode(ToolMode p_toolMode) { m_toolMode = p_toolMode; }

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

  void DrawPolygonsFromCore();
  void DrawPolygonFromCore(QStandardItem* p_polygonItem, bool p_isSelectedPolygon);
  void DrawPolygonsFromModel();
  void DrawPolygonFromModel(QStandardItem* p_polygonItem, bool p_isSelectedPolygon);
  void DrawObjectsFromModel();

  void MousePressForPolygon(QMouseEvent* p_event);
  void MouseMoveForPolygon(QMouseEvent* p_event);
  void MouseReleaseForPolygon(QMouseEvent* p_event);

  void MousePressForTape(QMouseEvent* p_event);
  void MouseMoveForTape(QMouseEvent* p_event);
  void MouseReleaseForTape(QMouseEvent* p_event);

  void MousePressForObject(QMouseEvent* p_event);
  void MouseMoveForObject(QMouseEvent* p_event);
  void MouseReleaseForObject(QMouseEvent* p_event);

  void GetDiscreteEnd(QMouseEvent* p_event, double& p_nx, double& p_ny);

  bool ConfirmClear();
  void ConfirmNewLevel();
  void ConfirmOpenLevel();

private:
  QPixmap m_gripPixmap;
  CreateLevelModel* m_polygonModel;
  QList<ObjectModel*> m_objectModelsList;
  QItemSelectionModel* m_selectionModel;
  ToolMode m_toolMode;
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

  QPoint m_objectStartPoint;
  bool m_drawingObject;

  static const int PEN_WIDTH;
  static const QColor NOT_SELECTED_COLOR;
};

double Distance(double p_ax, double p_ay, double p_bx, double p_by);

#endif
