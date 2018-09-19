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

  void Redraw();

  void InsertPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void AppendPolygon(const ppxl::Polygon& p_polygon);
  void RemovePolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void MovePolygon(int p_polygonRow, ppxl::Point const& p_oldBarycenter, ppxl::Point const& p_newBarycenter, bool p_pushToStack = true);

  void InsertVertex(int p_vertexRow, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex);
  void AppendVertex(QStandardItem* p_polygonItem, ppxl::Point const& p_vertex);
  void RemoveVertex(int p_vertexRow, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex);
  void MoveVertex(QStandardItem* p_polygonItem, int p_vertexRow, ppxl::Point const& p_oldVertex, ppxl::Point p_newVertex, bool p_pushToStack = true);

private:
  CreateLevelModel* m_model;
  CreateLevelView* m_view;
  QUndoStack* m_undoStack;
};

#endif // CREATELEVELCONTROLLER_HXX
