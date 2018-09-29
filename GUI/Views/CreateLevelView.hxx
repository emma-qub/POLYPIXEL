#ifndef CREATELEVELVIEW_HXX
#define CREATELEVELVIEW_HXX

#include <QWidget>

#include "Core/Point.hxx"
#include "Core/Vector.hxx"
#include "Core/Polygon.hxx"

class CreateLevelModel;
class CreateLevelScribblingView;
class QLabel;
class QPushButton;
class QTreeView;
class QItemSelectionModel;
class QUndoView;
class QUndoStack;
class QSpinBox;

class CreateLevelView: public QWidget {
  Q_OBJECT

public:
  explicit CreateLevelView(QWidget* parent = nullptr);

  int GetLinesGoal() const;
  void SetLinesGoal(int p_value);
  int GetPartsGoal() const;
  int SetPartsGoal(int p_value);
  int GetMaxGapToWin() const;
  int SetMaxGapToWin(int p_value);
  int GetTolerance() const;
  int SetTolerance(int p_value);
  void ResetGameInfo();

  void SetModel(CreateLevelModel* p_model);
  void SetUndoStack(QUndoStack* p_undoStack);
  QItemSelectionModel* GetSelectionModel() const;

  void DrawFromModel();
  void ClearImage();
  void Redraw();
  void RedrawFromPolygons();

  void SetTestAvailable(bool p_enable);

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
