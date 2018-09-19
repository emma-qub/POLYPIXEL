#include "CreateLevelController.hxx"

#include "GUI/Models/CreateLevelModel.hxx"
#include "GUI/Views/CreateLevelView.hxx"
#include "GUI/Commands/Commands.hxx"

#include <QUndoStack>

CreateLevelController::CreateLevelController(CreateLevelModel* p_model, CreateLevelView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_model(p_model),
  m_view(p_view),
  m_undoStack(new QUndoStack(this)) {

  m_view->SetModel(m_model);
}

void CreateLevelController::Redraw() {
  m_view->Redraw();
}

void CreateLevelController::InsertPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  QUndoCommand* addPolygonCommand;// = new AddPolygonCommand(m_model, p_polygonRow, p_polygon, p_polygonRow, -1);
  m_undoStack->push(addPolygonCommand);
}

void CreateLevelController::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_model->rowCount(), p_polygon);
}

void CreateLevelController::RemovePolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  int newPolygonRow = p_polygonRow;
  if (m_model->rowCount() == 0) {
    newPolygonRow = -1;
  } else {
    if (p_polygonRow == m_model->rowCount()-1) {
      newPolygonRow = p_polygonRow-1;
    }
  }

  QUndoCommand* removePolygonCommand;// = new RemovePolygonCommand(m_model, p_polygonRow, p_polygon, newPolygonRow, -1);
  m_undoStack->push(removePolygonCommand);
}

void CreateLevelController::MovePolygon(int p_polygonRow, ppxl::Point const& p_oldBarycenter, ppxl::Point const& p_newBarycenter, bool p_pushToStack) {
  if (p_pushToStack) {
    QUndoCommand* movePolygonCommand;// = new MovePolygonCommand(m_model, polygonRow, p_oldBarycenter, p_newBarycenter, newX, newY, polygonRow, -1);
    m_undoStack->push(movePolygonCommand);
  } else {
    ppxl::Polygon polygon;
    polygon.Translate(ppxl::Vector(p_oldBarycenter - p_newBarycenter));
    m_model->SetPolygon(p_polygonRow, polygon);
  }
}

void CreateLevelController::InsertVertex(int p_vertexRow, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex) {
  QUndoCommand* addVertexCommand;// = new AddVertexCommand(m_model, polygonRow, p_vertexRow, p_vertex, polygonRow, p_vertexRow);
  m_undoStack->push(addVertexCommand);
}

void CreateLevelController::AppendVertex(QStandardItem* p_polygonItem, ppxl::Point const& p_vertex) {
  InsertVertex(p_polygonItem->rowCount(), p_polygonItem, p_vertex);
}

void CreateLevelController::RemoveVertex(int p_vertexRow, QStandardItem* p_polygonItem, ppxl::Point const& p_vertex) {
  int newVertexRow = p_vertexRow;
  newVertexRow = p_polygonItem->rowCount() - 1;

  QUndoCommand* removeVertexCommand;// = new RemoveVertexCommand(m_model, polygonRow, p_vertexRow, p_vertex, polygonRow, newVertexRow);
  m_undoStack->push(removeVertexCommand);
}

void CreateLevelController::MoveVertex(QStandardItem* p_polygonItem, int p_vertexRow, ppxl::Point const& p_oldVertex, ppxl::Point p_newVertex, bool p_pushToStack) {
  if (p_pushToStack) {
    QUndoCommand* moverVertexCommand;// = new MoveVertexCommand(m_model, polygonRow, p_vertexRow, oldX, oldY, newX, newY, polygonRow, p_vertexRow);
    m_undoStack->push(moverVertexCommand);
  } else {
    m_model->SetVertex(p_polygonItem, p_vertexRow, p_newVertex);
  }
}
