#ifndef CREATELEVELCONTROLLER_HXX
#define CREATELEVELCONTROLLER_HXX

#include <QObject>

#include "Core/Polygon.hxx"
#include "Core/Point.hxx"

class CreateLevelView;
class CreateLevelModel;
class QUndoStack;
class QStandardItem;

class CreateLevelController: public QObject {
  Q_OBJECT

public:
  explicit CreateLevelController(CreateLevelModel* p_model, CreateLevelView* p_view,  QObject *parent = nullptr);
  ~CreateLevelController() override;

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

private:
  CreateLevelModel* m_model;
  CreateLevelView* m_view;
  QUndoStack* m_undoStack;
};

#endif // CREATELEVELCONTROLLER_HXX
