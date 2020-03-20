#ifndef CREATELEVELWIDGET_HXX
#define CREATELEVELWIDGET_HXX

#include <QWidget>

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Polygon.hxx"

class CreateLevelModel;
class ObjectModel;
class CreateLevelScribblingView;
class QLabel;
class QPushButton;
class QTreeView;
class QItemSelectionModel;
class QUndoView;
class QUndoStack;
class QSpinBox;
class QStackedWidget;
class QStandardItem;

class CreateLevelWidget: public QWidget {
  Q_OBJECT

public:
  enum ToolMode {
    eSelectionTool,
    ePolygonTool,
    eTapeTool,
    eMirrorTool,
    eOneWayTool,
    ePortalTool
  };

  explicit CreateLevelWidget(QWidget* parent = nullptr);

  void InitViews();

  int GetLinesGoal() const;
  void SetLinesGoal(int p_value);
  int GetPartsGoal() const;
  void SetPartsGoal(int p_value);
  int GetMaxGapToWin() const;
  void SetMaxGapToWin(int p_value);
  int GetTolerance() const;
  void SetTolerance(int p_value);
  void ResetGameInfo();

  void SetModel(CreateLevelModel* p_model);
  void SetUndoStack(QUndoStack* p_undoStack);
  QItemSelectionModel* GetSelectionModel() const;

  void DrawFromModel();
  void ClearImage();
  void Redraw();
  void RedrawFromPolygons();

  void SetTestAvailable(bool p_enable);

  qreal GetSceneRectWidth() const;
  qreal GetSceneRectHeight() const;

  void CreateGraphicsObjectFromItem(QStandardItem* p_item);

signals:
  void TestLevelRequested();
  void Done();
  void PolygonInserted(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void PolygonRemoved(int p_polygonRow);
  void PolygonMoved(int p_polygonRow, ppxl::Vector const& p_direction, bool p_pushToStack);
  void VertexInserted(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void VertexRemoved(int p_polygonRow, int p_vertexRow);
  void VertexMoved(int p_polygonRow, int p_vertexRow, const ppxl::Vector& p_direction, bool p_pushToStack);
  void PolygonSelected();
  void ValueXChanged(int p_value, QModelIndex const& p_index);
  void ValueYChanged(int p_value, QModelIndex const& p_index);
  void EditionXDone(int p_value, QModelIndex const& p_index);
  void EditionYDone(int p_value, QModelIndex const& p_index);
  void SnappedToGrid();
  void NewLevelRequested();
  void OpenLevelRequested(QString const& p_fileName);
  void ToolActivated(ToolMode p_tool);
  void MousePressed(QMouseEvent* p_event);
  void MouseMoved(QMouseEvent* p_event);
  void MouseReleased(QMouseEvent* p_event);

protected:
  void UpdateMaxGapToWinPrefix(int p_value);
  void UpdateTolerancePrefix(int p_value);

private:
  QLabel* m_createLevelLabel;
  QPushButton* m_testLevelButton;
  QPushButton* m_menuButton;
  CreateLevelModel* m_model;
  CreateLevelScribblingView* m_scribblingView;
  QTreeView* m_treeView;
  QUndoView* m_undoView;
  QSpinBox* m_linesGoalSpinBox;
  QSpinBox* m_partsGoalSpinBox;
  QSpinBox* m_maxGapToWinSpinBox;
  QSpinBox* m_toleranceSpinBox;
};

#endif
