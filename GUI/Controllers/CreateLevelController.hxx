#ifndef CREATELEVELCONTROLLER_HXX
#define CREATELEVELCONTROLLER_HXX

#include <QObject>

#include "Core/Geometry/Polygon.hxx"
#include "Core/Geometry/Point.hxx"
#include "GUI/Views/CreateLevelWidget.hxx"
#include "GUI/Models/CreateLevelObjectsListModel.hxx"
#include "GUI/Models/CreateLevelObjectsDetailModel.hxx"
#include "GUI/Models/CreateLevelVertexListModel.hxx"

class QUndoStack;
class QStandardItem;
class QToolBar;
class QMouseEvent;

class CreateLevelController: public QObject {
  Q_OBJECT

public:
  enum ToolMode {
    eSelectionMode,
    ePolygonMode,
    eTapeMode,
    eMirrorMode,
    eOneWayMode,
    ePortalMode,
    eRectangleSelectionMode
  };

  explicit CreateLevelController(CreateLevelWidget* p_view,  QObject *parent = nullptr);
  ~CreateLevelController() override;

  void SetToolBar(QToolBar* p_toolbar);

  inline CreateLevelObjectsListModel* GetPolygonModel() const { return m_objectsListModel; }
  int GetLinesGoal() const;
  int GetPartsGoal() const;
  int GetMaxGapToWin() const;
  int GetTolerance() const;

  void UpdateView();

protected:
  void CreatePolygon(const ppxl::Polygon& p_polygon = ppxl::Polygon());
  void TranslatePolygon(ppxl::Vector const& p_direction);
  void InsertVertex(QPoint const& p_pos);
  void MoveCurrentVertex(ppxl::Point const& p_pos);
  void MoveVertexAt(int p_vertexIndex, ppxl::Point const& p_pos);

  void SnapToGrid();
  void SnapAllToGrid();
  void SnapObjectToGrid(QModelIndex const& p_currentIndex);
  void SnapPolygonToGrid(QModelIndex const& p_currentIndex);
  void CheckTestAvailable();

  void MousePressEvent(QMouseEvent* p_event);
  void MouseMoveEvent(QMouseEvent* p_event);
  void MouseReleaseEvent(QMouseEvent*);

  void DisableObjectItems();
  void SelectObjectUnderCursor(QPoint const& p_pos);
  void CreateObject(Object* p_object = nullptr);
  void FindNearestVertex(bool& p_isNearVertex, ppxl::Point& p_nearestVertex, int& p_nearestVertexRow, QPoint const& p_pos) const;
  void FindNearestControlPoint(bool& p_isNearControlPoint, QPair<QPoint, Object::ControlPointType>& p_nearestControlPoint, QPoint const& p_pos) const;
  void MoveObject(QPoint const& p_pos);
  void TranslateObject(const ppxl::Vector& p_direction);
  void HighlightObjectUnderCursor(QPoint const& p_pos);

  void MoveRectangleSelection(QPoint const& p_pos);

  void NewLevel();
  void OpenLevel(QString const& p_fileName);

  void UpdateGraphicsSelection(QModelIndex const& p_current, QModelIndex const&);
  void UpdateSelection();

  void ChangeCurrentTool();
  void ConnectToolsActions();
  void DisconnectToolsActions();

  ppxl::Point FindNearestGridNode(ppxl::Point const& p_point);

  void PolygonComplete();
  void DeleteCurrent();
  void MoveCurrent(ppxl::Vector const& p_direction, bool p_shiftPressed);
  void MoveCurrentLeft(bool p_shiftPressed);
  void MoveCurrentUp(bool p_shiftPressed);
  void MoveCurrentRight(bool p_shiftPressed);
  void MoveCurrentDown(bool p_shiftPressed);

private:
  CreateLevelObjectsListModel* m_objectsListModel;
  CreateLevelObjectsDetailModel* m_objectsDetailModel;
  CreateLevelVertexListModel* m_vertexListModel;
  CreateLevelWidget* m_createLevelWidget;

  QToolBar* m_toolbar;
  ToolMode m_toolMode;

  QAction* m_selectAction;
  QAction* m_polygonAction;
  QAction* m_tapeAction;
  QAction* m_mirrorAction;
  QAction* m_oneWayAction;
  QAction* m_portalAction;
  QAction* m_rectangleSelectionAction;

  QMap<CreateLevelObjectsListModel::ObjectType, QAction*> m_objectTypeAction;
  QMap<QAction*, ToolMode> m_actionToolModeMap;

  QPoint m_objectStartPoint;

  bool m_creatingNewPolygon;
  bool m_creatingObject;

  bool m_mousePressed;
  bool m_isNearVertex;
  ppxl::Point m_nearestVertex;
  int m_nearestVertexRow;
  bool m_isNearControlPoint;
  QPair<QPoint, Object::ControlPointType> m_nearestControlPoint;

  QStandardItem* m_hoveredItem;

  QModelIndex m_clipboardIndex;
};

#endif
