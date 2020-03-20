#ifndef CREATELEVELCONTROLLER_HXX
#define CREATELEVELCONTROLLER_HXX

#include <QObject>

#include "Core/Geometry/Polygon.hxx"
#include "Core/Geometry/Point.hxx"
#include "GUI/Views/CreateLevelWidget.hxx"
#include "GUI/Models/CreateLevelModel.hxx"

class ObjectModel;
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
    ePortalMode
  };

  explicit CreateLevelController(CreateLevelWidget* p_view,  QObject *parent = nullptr);
  ~CreateLevelController() override;

  void SetToolBar(QToolBar* p_toolbar);

  inline CreateLevelModel* GetPolygonModel() const { return m_model; }
  int GetLinesGoal() const;
  int GetPartsGoal() const;
  int GetMaxGapToWin() const;
  int GetTolerance() const;

  void RedrawFromPolygons();
  void Redraw();
  void UndoRedo();

  void InsertPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void AppendPolygon(ppxl::Polygon const& p_polygon);
  void RemovePolygon(int p_polygonRow);
  void MovePolygon(int p_polygonRow, const ppxl::Vector& p_direction, bool p_pushToStack = true);

  void InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void AppendVertex(int p_polygonRow, ppxl::Point const& p_vertex);
  void RemoveVertex(int p_polygonRow, int p_vertexRow);
  void MoveVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction, bool p_pushToStack = true);

protected:
  void UpdateXVertex(int p_value, const QModelIndex& p_index);
  void UpdateYVertex(int p_value, const QModelIndex& p_index);
  void TranslateXVertex(int p_value, const QModelIndex& p_index);
  void TranslateYVertex(int p_value, const QModelIndex& p_index);
  void SnapToGrid();
  void SnapCurrentPolygonToGrid(QModelIndex const& p_currentIndex);
  void CheckTestAvailable();

  void MousePressEvent(QMouseEvent* p_event);
  void MouseMoveEvent(QMouseEvent* p_event);
  void MouseReleaseEvent(QMouseEvent* p_event);

  void DisableObjectItems();
  void SelectObjectUnderCursor();
  void CreateObect();
  void FindNearestControlPoint(bool& nearControlPoint, QPoint& nearestControlPoint, const QPoint& p_pos) const;
  void MoveNewObject(QPoint const& p_pos);
  void MoveExistingObject(QPoint const& p_pos);
  void GetDiscreteEnd(QPoint const& p_pos, double& p_nx, double& p_ny);
  void HighlightObjectUnderCursor(QPoint const& p_pos);

  void NewLevel();
  void OpenLevel(QString const& p_fileName);

private:
  CreateLevelModel* m_model;
  CreateLevelWidget* m_view;
  QUndoStack* m_undoStack;
  QToolBar* m_toolbar;

  ToolMode m_toolMode;

  QAction* m_selectAction;
  QAction* m_polygonAction;
  QMap<CreateLevelModel::ObjectType, QAction*> m_objectTypeAction;

  QPoint m_objectStartPoint;
  bool m_creatingObject;
  bool m_editingObject;
  bool m_mousePressed;
  bool m_nearControlPoint;

  QStandardItem* m_hoveredItem;
  QStandardItem* m_selectedItem;
};

#endif
