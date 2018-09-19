#include "Commands.hxx"

#include "GUI/Models/CreateLevelModel.hxx"

#include <QStandardItem>

AddVertexCommand::AddVertexCommand(CreateLevelModel* p_model, int p_polygonRow, int p_vertexRow,
  ppxl::Point const& p_vertex, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent):
  QUndoCommand(p_parent),
  m_model(p_model),
  m_polygonRow(p_polygonRow),
  m_vertexRow(p_vertexRow),
  m_vertex(p_vertex),
  m_selectionPolygonRow(p_selectionPolygonRow),
  m_selectionVertexRow(p_selectionVertexRow) {

  setText("Add new Vertex");
}

AddVertexCommand::~AddVertexCommand() = default;

void AddVertexCommand::undo() {
  m_model->RemoveVertexAt(m_polygonRow, m_vertexRow);
  //m_model->PopSelection();
}

void AddVertexCommand::redo() {
  m_model->InsertVertex(m_polygonRow, m_vertexRow, m_vertex);
  //m_model->PushSelection(m_selectionPolygonRow, m_selectionVertexRow);
}



RemoveVertexCommand::RemoveVertexCommand(CreateLevelModel* p_model, int p_polygonRow, int p_vertexRow,
  ppxl::Point const& p_vertex, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent):
  QUndoCommand(p_parent),
  m_model(p_model),
  m_polygonRow(p_polygonRow),
  m_vertexRow(p_vertexRow),
  m_vertex(p_vertex),
  m_selectionPolygonRow(p_selectionPolygonRow),
  m_selectionVertexRow(p_selectionVertexRow) {

  setText("Remove "+p_model->data(p_model->index(p_vertexRow, 1, p_model->index(p_polygonRow, 0))).toString());
}

RemoveVertexCommand::~RemoveVertexCommand() = default;

void RemoveVertexCommand::undo() {
  m_model->InsertVertex(m_polygonRow, m_vertexRow, m_vertex);
  //m_model->PopSelection();
}

void RemoveVertexCommand::redo() {
  m_model->RemoveVertexAt(m_polygonRow, m_vertexRow);
  //m_model->PushSelection(m_selectionPolygonRow, m_selectionVertexRow);
}



MoveVertexCommand::MoveVertexCommand(CreateLevelModel* p_model, int p_polygonRow, int p_vertexRow,
  ppxl::Vector const& p_direction, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent):
  QUndoCommand(p_parent),
  m_model(p_model),
  m_polygonRow(p_polygonRow),
  m_vertexRow(p_vertexRow),
  m_direction(p_direction),
  m_selectionPolygonRow(p_selectionPolygonRow),
  m_selectionVertexRow(p_selectionVertexRow) {

  setText("Move "+p_model->data(p_model->index(m_vertexRow, 1, p_model->index(m_polygonRow, 0))).toString()
    +" in direction ("+QString::number(m_direction.GetX())+";"+QString::number(m_direction.GetY())+")");
}

MoveVertexCommand::~MoveVertexCommand() = default;

void MoveVertexCommand::undo() {
  m_model->TranslateVertex(m_polygonRow, m_vertexRow, -m_direction);
  //m_model->popSelection();
}

void MoveVertexCommand::redo() {
  m_model->TranslateVertex(m_polygonRow, m_vertexRow, m_direction);
  //m_model->PushSelection(m_selectionPolygonRow, m_selectionVertexRow);
}



AddPolygonCommand::AddPolygonCommand(CreateLevelModel* p_model, int p_polygonRow,
  ppxl::Polygon const& p_polygon, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  m_model(p_model),
  m_polygonRow(p_polygonRow),
  m_polygon(p_polygon),
  m_selectionPolygonRow(p_selectionPolygonRow),
  m_selectionVertexRow(p_selectionVertexRow) {

  setText("Add Polygon"+QString::number(p_polygonRow));
}

AddPolygonCommand::~AddPolygonCommand() = default;

void AddPolygonCommand::undo() {
  m_model->RemovePolygonAt(m_polygonRow);
  //m_model->PopSelection();
}

void AddPolygonCommand::redo() {
  m_model->InsertPolygon(m_polygonRow, m_polygon);
  //m_model->PushSelection(m_selectionPolygonRow, m_selectionVertexRow);
}



RemovePolygonCommand::RemovePolygonCommand(CreateLevelModel* p_model, int p_polygonRow,
  ppxl::Polygon const& p_polygon, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent):
  QUndoCommand(p_parent),
  m_model(p_model),
  m_polygonRow(p_polygonRow),
  m_polygon(p_polygon),
  m_selectionPolygonRow(p_selectionPolygonRow),
  m_selectionVertexRow(p_selectionVertexRow) {

  setText("Remove Polygon"+QString::number(p_polygonRow));
}

RemovePolygonCommand::~RemovePolygonCommand() = default;

void RemovePolygonCommand::undo() {
  m_model->InsertPolygon(m_polygonRow, m_polygon);
  //m_model->PopSelection();
}

void RemovePolygonCommand::redo() {
  m_model->RemovePolygonAt(m_polygonRow);
  //m_model->PushSelection(m_selectionPolygonRow, m_selectionVertexRow);
}



MovePolygonCommand::MovePolygonCommand(CreateLevelModel* p_model, int p_polygonRow,
  ppxl::Vector const& p_direction, int p_selectionPolygonRow, int p_selectionVertexRow, QUndoCommand* p_parent):
  QUndoCommand(p_parent),
  m_model(p_model),
  m_polygonRow(p_polygonRow),
  m_direction(p_direction),
  m_selectionPolygonRow(p_selectionPolygonRow),
  m_selectionVertexRow(p_selectionVertexRow) {

  setText("Move Polygon"+QString::number(m_polygonRow)+" according to vector ("+QString::number(m_direction.GetX())+";"+QString::number(m_direction.GetY())+")");
}

MovePolygonCommand::~MovePolygonCommand() = default;

void MovePolygonCommand::undo() {
  m_model->TranslatePolygon(m_polygonRow, -m_direction);
  //m_model->PopSelection();
}

void MovePolygonCommand::redo() {
  m_model->TranslatePolygon(m_polygonRow, m_direction);
  //m_model->PushSelection(m_selectionPolygonRow, m_selectionVertexRow);
}
