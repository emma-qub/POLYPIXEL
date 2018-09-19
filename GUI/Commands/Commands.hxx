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
  AddVertexCommand(CreateLevelModel* p_model, int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex,
    int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
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
  RemoveVertexCommand(CreateLevelModel* p_model, int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex,
    int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
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



class MoveVertexCommand: public QUndoCommand {
public:
  MoveVertexCommand(CreateLevelModel* p_model, int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction,
     int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~MoveVertexCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  int m_vertexRow;
  ppxl::Vector m_direction;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class AddPolygonCommand: public QUndoCommand {
public:
  AddPolygonCommand(CreateLevelModel* p_model, int p_polygonRow, ppxl::Polygon const& p_polygon,
    int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
  ~AddPolygonCommand() override;

  void undo() override;
  void redo() override;

private:
  CreateLevelModel* m_model;
  int m_polygonRow;
  ppxl::Polygon m_polygon;
  int m_selectionPolygonRow;
  int m_selectionVertexRow;
};



class RemovePolygonCommand: public QUndoCommand {
public:
  RemovePolygonCommand(CreateLevelModel* p_model, int p_polygonRow, ppxl::Polygon const& p_polygon,
    int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
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
  MovePolygonCommand(CreateLevelModel* p_model, int p_polygonRow, ppxl::Vector const& p_direction,
    int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent = nullptr);
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


#endif
