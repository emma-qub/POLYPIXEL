#ifndef COMMANDS_HXX
#define COMMANDS_HXX

#include <QUndoCommand>

#include "Core/Point.hxx"
#include "Core/Vector.hxx"
#include "Core/Polygon.hxx"

class CreateLevelModel;
class QStandardItem;
class QItemSelectionModel;

class UndoCommand: public QUndoCommand {
public:
  UndoCommand(CreateLevelModel* p_model, QItemSelectionModel* p_selectionModel, QUndoCommand* p_parent = nullptr);

  ~UndoCommand() override;

protected:
  CreateLevelModel* m_model;
  QItemSelectionModel* m_selectionModel;
};

class AddVertexCommand: public UndoCommand {
public:
  AddVertexCommand(CreateLevelModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex,
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
  RemoveVertexCommand(CreateLevelModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow, int p_vertexRow,
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
  MoveVertexCommand(CreateLevelModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow, int p_vertexRow,
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
  AddPolygonCommand(CreateLevelModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow,
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
  RemovePolygonCommand(CreateLevelModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow,
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
  MovePolygonCommand(CreateLevelModel* p_model, QItemSelectionModel* p_selectionModel, int p_polygonRow,
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
