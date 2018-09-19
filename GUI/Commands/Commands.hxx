#ifndef COMMANDS_HXX
#define COMMANDS_HXX

#include <QUndoCommand>

#include "Core/Point.hxx"
#include "Core/Vector.hxx"
#include "Core/Polygon.hxx"

class CreateLevelModel;
class QStandardItem;

class AddVertexCommand: public QUndoCommand {
public:
  AddVertexCommand(CreateLevelModel* model, int polygonRow, int vertexRow, ppxl::Point const& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = nullptr);
  ~AddVertexCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  int m_vertexRow;
  ppxl::Point m_vertex;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class RemoveVertexCommand: public QUndoCommand {
public:
  RemoveVertexCommand(CreateLevelModel* model, int polygonRow, int vertexRow, ppxl::Point const& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = nullptr);
  ~RemoveVertexCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  int m_vertexRow;
  ppxl::Point m_vertex;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class AddPolygonCommand: public QUndoCommand {
public:
  AddPolygonCommand(CreateLevelModel* model, int polygonRow, ppxl::Polygon const& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = nullptr);
  ~AddPolygonCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  ppxl::Polygon m_polygon;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
  QStandardItem* m_polygonItem;
};



class RemovePolygonCommand: public QUndoCommand {
public:
  RemovePolygonCommand(CreateLevelModel* model, int polygonRow, ppxl::Polygon const& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = nullptr);
  ~RemovePolygonCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  ppxl::Polygon m_polygon;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class MovePolygonCommand: public QUndoCommand {
public:
  MovePolygonCommand(CreateLevelModel* model, int polygonRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = nullptr);
  ~MovePolygonCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  ppxl::Vector m_direction;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class MoveVertexCommand: public QUndoCommand {
public:
  MoveVertexCommand(CreateLevelModel* model, int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent = nullptr);
  ~MoveVertexCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  int m_vertexRow;
  int m_oldX;
  int m_oldY;
  int m_newX;
  int m_newY;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};


#endif
