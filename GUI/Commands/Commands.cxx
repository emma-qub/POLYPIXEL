#include "Commands.hxx"

#include "GUI/Models/CreateLevelModel.hxx"

#include <QStandardItem>

//AddVertexCommand::AddVertexCommand(CreateLevelModel* model, int polygonRow, int vertexRow, ppxl::Point const& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
//  QUndoCommand(parent),
//  m_model(model),
//  m_polygonRow(polygonRow),
//  m_vertexRow(vertexRow),
//  m_vertex(vertex),
//  m_selectionPolygonRow(selectionPolygonRow),
//  m_selectionVertexRow(selectionVertexRow) {

//  setText("Add new Vertex");
//}

//AddVertexCommand::~AddVertexCommand() = default;

//void AddVertexCommand::undo() {
//  m_model->removeRow(m_row, m_parent);
//}

//void AddVertexCommand::redo() {
//  m_model->insertRow();
//}



//RemoveVertexCommand::RemoveVertexCommand(CreateLevelModel* model, QModelIndex const& p_parentIndex, int polygonRow, int vertexRow, ppxl::Point const& vertex, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
//  QUndoCommand(parent),
//  m_model(model),
//  m_polygonRow(polygonRow),
//  m_vertexRow(vertexRow),
//  m_vertex(vertex),
//  m_selectionPolygonRow(selectionPolygonRow),
//  m_selectionVertexRow(selectionVertexRow) {

//RemoveVertexCommand::~RemoveVertexCommand() = default;

//  setText("Remove "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString());
//}

//void RemoveVertexCommand::undo() {
//  m_model->insertRow();
//}

//void RemoveVertexCommand::redo() {
//  m_model->removeRow();
//}



AddPolygonCommand::AddPolygonCommand(CreateLevelModel* model, int polygonRow, ppxl::Polygon const& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
  QUndoCommand(parent),
  m_model(model),
  m_polygonRow(polygonRow),
  m_polygon(polygon),
  m_selectionPolygonRow(selectionPolygonRow),
  m_selectionVertexRow(selectionVertexRow) {

  setText("Add Polygon"+QString::number(polygonRow));
}

AddPolygonCommand::~AddPolygonCommand() = default;

void AddPolygonCommand::undo() {
  m_polygonItem = m_model->InsertPolygon(m_polygonRow, m_polygon);
}

void AddPolygonCommand::redo() {
  m_model->removeRow(m_polygonItem->row(), m_polygonItem->parent()->index());
}



//RemovePolygonCommand::RemovePolygonCommand(CreateLevelModel* model, int polygonRow, ppxl::Polygon const& polygon, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
//  QUndoCommand(parent),
//  m_model(model),
//  m_polygonRow(polygonRow),
//  m_polygon(polygon),
//  m_selectionPolygonRow(selectionPolygonRow),
//  m_selectionVertexRow(selectionVertexRow) {

//  setText("Remove Polygon"+QString::number(polygonRow));
//}

//RemovePolygonCommand::~RemovePolygonCommand() = default;

//void RemovePolygonCommand::undo() {
//  m_model->insertPolygon(m_polygonRow, m_polygon);
//  m_model->popSelection();
//}

//void RemovePolygonCommand::redo() {
//  m_model->removePolygon(m_polygonRow);
//  m_model->addSelection(m_selectionPolygonRow, m_selectionVertexRow);
//}



//MovePolygonCommand::MovePolygonCommand(CreateLevelModel* model, int polygonRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
//  QUndoCommand(parent),
//  m_model(model),
//  m_polygonRow(polygonRow),
//  m_direction(newX-oldX, newY-oldY),
//  m_selectionPolygonRow(selectionPolygonRow),
//  m_selectionVertexRow(selectionVertexRow) {

//  setText("Move Polygon"+QString::number(m_polygonRow)+" according to vector ("+QString::number(m_direction.getX())+";"+QString::number(m_direction.getY())+")");
//}

//MovePolygonCommand::~MovePolygonCommand() = default;

//void MovePolygonCommand::undo() {
//  m_model->translatePolygon(m_polygonRow, -m_direction);
//  m_model->popSelection();
//}

//void MovePolygonCommand::redo() {
//  m_model->translatePolygon(m_polygonRow, m_direction);
//  m_model->addSelection(m_selectionPolygonRow, m_selectionVertexRow);
//}



//MoveVertexCommand::MoveVertexCommand(CreateLevelModel* model, int polygonRow, int vertexRow, int oldX, int oldY, int newX, int newY, int selectionPolygonRow, int selectionVertexRow, QUndoCommand* parent):
//  QUndoCommand(parent),
//  m_model(model),
//  m_polygonRow(polygonRow),
//  m_vertexRow(vertexRow),
//  m_oldX(oldX),
//  m_oldY(oldY),
//  m_newX(newX),
//  m_newY(newY),
//  m_selectionPolygonRow(selectionPolygonRow),
//  m_selectionVertexRow(selectionVertexRow) {

//  if (m_newX == -1)
//    m_newX = m_oldX;
//  else if (m_newY == -1)
//    m_newY = m_oldY;

//  setText("Move "+model->data(model->index(vertexRow, 1, model->index(polygonRow, 0))).toString()+" from ("+QString::number(m_oldX)+";"+QString::number(m_oldY)+") to ("+QString::number(m_newX)+";"+QString::number(m_newY)+")");
//}

//MoveVertexCommand::~MoveVertexCommand() = default;

//void MoveVertexCommand::undo() {
//  m_model->replaceVertex(m_polygonRow, m_vertexRow, ppxl::Point(m_oldX, m_oldY));
//  m_model->popSelection();
//}

//void MoveVertexCommand::redo() {
//  m_model->replaceVertex(m_polygonRow, m_vertexRow, ppxl::Point(m_newX, m_newY));
//  m_model->addSelection(m_polygonRow, m_vertexRow);
//}
