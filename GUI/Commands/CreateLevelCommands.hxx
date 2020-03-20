#ifndef CREATELEVELCOMMANDS_HXX
#define CREATELEVELCOMMANDS_HXX

#include <QUndoCommand>

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Polygon.hxx"

class CreateLevelObjectsListModel;
class QStandardItem;
class QItemSelectionModel;

class UndoCommand: public QUndoCommand {
public:
  UndoCommand(CreateLevelObjectsListModel* p_model, QItemSelectionModel* p_selectionModel, QUndoCommand* p_parent = nullptr);

  ~UndoCommand() override;

protected:
  CreateLevelObjectsListModel* m_model;
  QItemSelectionModel* m_selectionModel;
};

class AddVertexCommand: public UndoCommand {
public:
  AddVertexCommand(CreateLevelObjectsListModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex,
    int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~AddVertexCommand() override;

  void undo() override;
  void redo() override;

private:
  int m_polygonRow;
  int m_vertexRow;
  ppxl::Point m_vertex;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class RemoveVertexCommand: public UndoCommand {
public:
  RemoveVertexCommand(CreateLevelObjectsListModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow, int p_vertexRow,
    ppxl::Point const& p_vertex, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~RemoveVertexCommand() override;

  void undo() override;
  void redo() override;

private:
  int m_polygonRow;
  int m_vertexRow;
  ppxl::Point m_vertex;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class MoveVertexCommand: public UndoCommand {
public:
  MoveVertexCommand(CreateLevelObjectsListModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow, int p_vertexRow,
    ppxl::Vector const& p_direction, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~MoveVertexCommand() override;

  void undo() override;
  void redo() override;

private:
  int m_polygonRow;
  int m_vertexRow;
  ppxl::Vector m_direction;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class AddPolygonCommand: public UndoCommand {
public:
  AddPolygonCommand(CreateLevelObjectsListModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow,
    ppxl::Polygon const& p_polygon, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~AddPolygonCommand() override;

  void undo() override;
  void redo() override;

private:
  int m_polygonRow;
  ppxl::Polygon m_polygon;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class RemovePolygonCommand: public UndoCommand {
public:
  RemovePolygonCommand(CreateLevelObjectsListModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow,
    ppxl::Polygon const& p_polygon, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~RemovePolygonCommand() override;

  void undo() override;
  void redo() override;

private:
  int m_polygonRow;
  ppxl::Polygon m_polygon;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class MovePolygonCommand: public UndoCommand {
public:
  MovePolygonCommand(CreateLevelObjectsListModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow,
    ppxl::Vector const& p_direction, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~MovePolygonCommand() override;

  void undo() override;
  void redo() override;

private:
  int m_polygonRow;
  ppxl::Vector m_direction;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};


#endif
