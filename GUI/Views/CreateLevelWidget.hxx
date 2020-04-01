#ifndef CREATELEVELWIDGET_HXX
#define CREATELEVELWIDGET_HXX

#include <QWidget>

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Polygon.hxx"

class GraphicsObjectItem;
class CreateLevelObjectsListModel;
class CreateLevelObjectsDetailModel;
class CreateLevelVertexListModel;
class CreateLevelScribblingView;
class QLabel;
class QPushButton;
class QTreeView;
class QItemSelectionModel;
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

  void SetObjectsListModel(CreateLevelObjectsListModel* p_objectsListModel);
  void SetObjectsDetailModel(CreateLevelObjectsDetailModel* p_objectsDetailModel);
  void SetVertexListModel(CreateLevelVertexListModel* p_vertexListModel);

  QModelIndex GetCurrentObjectIndex() const;
  QModelIndex FindCurrentObjectIndex() const;
  QModelIndex GetCurrentPolygonIndex() const;
  QModelIndex FindCurrentPolygonIndex() const;
  QModelIndex GetCurrentVertexIndex() const;
  QModelIndex FindCurrentVertexIndex() const;
  void SetCurrentVertexIndex(int p_vertexRow);
  void SetCurrentObjectOrPolygonIndex(QModelIndex const& p_polygonIndex);

  void DrawFromModel();
  void ClearImage();

  void SetTestAvailable(bool p_enable);

  void ShowDetailListView();
  void ShowVertexListView();

  void UpdateView();

  void AddGraphicsItem(GraphicsObjectItem* p_graphicsItem);

Q_SIGNALS:
  void TestLevelRequested();
  void CreateLevelDone();
  void SnapToGridRequested();
  void SnapAllToGridRequested();
  void NewLevelRequested();
  void OpenLevelRequested(QString const& p_fileName);
  void MousePressed(QMouseEvent* p_event);
  void MouseMoved(QMouseEvent* p_event);
  void MouseReleased(QMouseEvent* p_event);
  void CurrentObjectIndexChanged(QModelIndex const& p_current, QModelIndex const& p_previous);
  void KeyReturnPressed();
  void KeyLeftPressed(bool p_shiftPressed);
  void KeyUpPressed(bool p_shiftPressed);
  void KeyRightPressed(bool p_shiftPressed);
  void KeyDownPressed(bool p_shiftPressed);

protected:
  enum ViewType {
    eDetailView,
    eVertexView
  };

  void UpdateMaxGapToWinPrefix(int p_value);
  void UpdateTolerancePrefix(int p_value);

  bool ConfirmClear();
  void ConfirmNewLevel();
  void ConfirmOpenLevel();

  int GetGraphicsItemCount() const;

private:
  QLabel* m_createLevelLabel;
  QPushButton* m_testLevelButton;
  QPushButton* m_menuButton;
  CreateLevelObjectsListModel* m_objectsListModel;
  CreateLevelObjectsDetailModel* m_objectsDetailModel;
  CreateLevelVertexListModel* m_vertexListModel;
  CreateLevelScribblingView* m_scribblingView;
  QTreeView* m_objectsListTreeView;
  QTreeView* m_objectsDetailTreeView;
  QTreeView* m_vertexTreeView;
  QStackedWidget* m_detailVertexStackWidget;
  QSpinBox* m_linesGoalSpinBox;
  QSpinBox* m_partsGoalSpinBox;
  QSpinBox* m_maxGapToWinSpinBox;
  QSpinBox* m_toleranceSpinBox;
};

#endif
