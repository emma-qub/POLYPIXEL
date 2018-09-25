#include "CreateLevelController.hxx"

#include "GUI/Models/CreateLevelModel.hxx"
#include "GUI/Views/CreateLevelView.hxx"
#include "GUI/Commands/Commands.hxx"

#include <QUndoStack>
#include <QAction>
#include <QItemSelectionModel>

CreateLevelController::CreateLevelController(CreateLevelModel* p_model, CreateLevelView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_model(p_model),
  m_view(p_view),
  m_undoStack(new QUndoStack(this)) {

  m_view->SetModel(m_model);
  connect(m_model, &CreateLevelModel::dataChanged, m_view, &CreateLevelView::Redraw);

  m_view->SetUndoStack(m_undoStack);

  connect(m_view, &CreateLevelView::PolygonInserted, this, &CreateLevelController::InsertPolygon);
  connect(m_view, &CreateLevelView::PolygonRemoved, this, &CreateLevelController::RemovePolygon);
  connect(m_view, &CreateLevelView::PolygonMoved, this, &CreateLevelController::MovePolygon);
  connect(m_view, &CreateLevelView::VertexInserted, this, &CreateLevelController::InsertVertex);
  connect(m_view, &CreateLevelView::VertexRemoved, this, &CreateLevelController::RemoveVertex);
  connect(m_view, &CreateLevelView::VertexMoved, this, &CreateLevelController::MoveVertex);

  connect(m_view, &CreateLevelView::ValueXChanged, this, &CreateLevelController::UpdateXVertex);
  connect(m_view, &CreateLevelView::ValueYChanged, this, &CreateLevelController::UpdateYVertex);
  connect(m_view, &CreateLevelView::EditionXDone, this, &CreateLevelController::TranslateXVertex);
  connect(m_view, &CreateLevelView::EditionYDone, this, &CreateLevelController::TranslateYVertex);

  connect(m_view, &CreateLevelView::PolygonSelected, this, &CreateLevelController::Redraw);

  connect(m_undoStack, &QUndoStack::indexChanged, this, &CreateLevelController::UndoRedo);

  auto undoAction = m_undoStack->createUndoAction(m_view, tr("Undo"));
  undoAction->setShortcut(QKeySequence::Undo);
  m_view->addAction(undoAction);
  auto redoAction = m_undoStack->createRedoAction(m_view, tr("Redo"));
  redoAction->setShortcut(QKeySequence::Redo);
  m_view->addAction(redoAction);
}

CreateLevelController::~CreateLevelController() {
  // When QUndoStack is deleted, it emits a last indexChanged, calling Redraw
  // from the scribbling view, whose model is already deleted, so disconnect.
  disconnect(m_undoStack, &QUndoStack::indexChanged, m_view, &CreateLevelView::Redraw);
}

void CreateLevelController::UpdateXVertex(int p_value, QModelIndex const& p_index) {
  auto* polygon = p_index.parent().data(PolygonModel::ePolygonRole).value<ppxl::Polygon*>();
  auto& vertex = polygon->GetVertices().at(static_cast<unsigned int>(p_index.row()));
  vertex.SetX(p_value);

  RedrawFromPolygons();
}

void CreateLevelController::UpdateYVertex(int p_value, QModelIndex const& p_index) {
  auto* polygon = p_index.parent().data(PolygonModel::ePolygonRole).value<ppxl::Polygon*>();
  auto& vertex = polygon->GetVertices().at(static_cast<unsigned int>(p_index.row()));
  vertex.SetY(p_value);

  RedrawFromPolygons();
}

void CreateLevelController::TranslateXVertex(int p_value, QModelIndex const& p_index) {
  auto shiftX = static_cast<double>(p_value - p_index.data().toInt());
  MoveVertex(p_index.parent().row(), p_index.row(), ppxl::Vector(-shiftX, 0.), false);
  MoveVertex(p_index.parent().row(), p_index.row(), ppxl::Vector(shiftX, 0.), true);
}

void CreateLevelController::TranslateYVertex(int p_value, QModelIndex const& p_index) {
  auto shiftY = static_cast<double>(p_value - p_index.data().toInt());
  MoveVertex(p_index.parent().row(), p_index.row(), ppxl::Vector(0., -shiftY), false);
  MoveVertex(p_index.parent().row(), p_index.row(), ppxl::Vector(0., shiftY), true);
}

void CreateLevelController::RedrawFromPolygons() {
  m_view->ClearImage();
  m_view->RedrawFromPolygons();
}

void CreateLevelController::Redraw() {
  m_view->ClearImage();
  m_view->Redraw();
}

void CreateLevelController::UndoRedo() {
  auto selectionModel = m_view->GetSelectionModel();
  auto selection = m_model->GetSelection();
  auto polygonRow = selection.last().first;
  auto vertexRow = selection.last().second;

  auto polygonsIndex = m_model->index(0, 0);
  if (polygonRow == -1 && vertexRow == -1) {
    selectionModel->setCurrentIndex(polygonsIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  } else {
    auto polygonIndex = m_model->index(polygonRow, 0, polygonsIndex);
    if (vertexRow == -1) {
      selectionModel->setCurrentIndex(polygonIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    } else {
      auto vertexIndex = m_model->index(vertexRow, 0, polygonIndex);
      selectionModel->setCurrentIndex(vertexIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
  }
  Redraw();
}

void CreateLevelController::InsertPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  QUndoCommand* addPolygonCommand = new AddPolygonCommand(m_model, m_view->GetSelectionModel(), p_polygonRow, p_polygon, p_polygonRow, -1);
  m_undoStack->push(addPolygonCommand);
}

void CreateLevelController::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_model->rowCount(), p_polygon);
}

void CreateLevelController::RemovePolygon(int p_polygonRow) {
  int newPolygonRow = p_polygonRow;
  if (m_model->rowCount() == 0) {
    newPolygonRow = -1;
  } else {
    if (p_polygonRow == m_model->rowCount()-1) {
      newPolygonRow = p_polygonRow-1;
    }
  }

  auto* polygon = m_model->GetPolygonsList().at(p_polygonRow);

  QUndoCommand* removePolygonCommand = new RemovePolygonCommand(m_model, m_view->GetSelectionModel(), p_polygonRow, *polygon, newPolygonRow, -1);
  m_undoStack->push(removePolygonCommand);
}

void CreateLevelController::MovePolygon(int p_polygonRow, ppxl::Vector const& p_direction, bool p_pushToStack) {
  if (p_pushToStack) {
    QUndoCommand* movePolygonCommand = new MovePolygonCommand(m_model, m_view->GetSelectionModel(), p_polygonRow, p_direction, p_polygonRow, -1);
    m_undoStack->push(movePolygonCommand);
  } else {
    m_model->TranslatePolygon(p_polygonRow, p_direction);
  }
}

void CreateLevelController::InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex) {
  QUndoCommand* addVertexCommand = new AddVertexCommand(m_model, m_view->GetSelectionModel(), p_polygonRow, p_vertexRow, p_vertex, p_polygonRow, p_vertexRow);
  m_undoStack->push(addVertexCommand);
}

void CreateLevelController::AppendVertex(int p_polygonRow, const ppxl::Point& p_vertex) {
  InsertVertex(p_polygonRow, m_model->GetPolygonsItem()->child(p_polygonRow, 0)->rowCount(), p_vertex);
}

void CreateLevelController::RemoveVertex(int p_polygonRow, int p_vertexRow) {
  auto const& vertex = m_model->GetPolygonsList().at(p_polygonRow)->GetVertices().at(static_cast<unsigned long>(p_vertexRow));

  QUndoCommand* removeVertexCommand = new RemoveVertexCommand(m_model, m_view->GetSelectionModel(), p_polygonRow, p_vertexRow, vertex, p_polygonRow, 0);
  m_undoStack->push(removeVertexCommand);
}

void CreateLevelController::MoveVertex(int p_polygonRow, int p_vertexRow, const ppxl::Vector& p_direction, bool p_pushToStack) {
  if (p_pushToStack) {
    QUndoCommand* moverVertexCommand = new MoveVertexCommand(m_model, m_view->GetSelectionModel(), p_polygonRow, p_vertexRow, p_direction, p_polygonRow, p_vertexRow);
    m_undoStack->push(moverVertexCommand);
  } else {
    m_model->TranslateVertex(p_polygonRow, p_vertexRow, p_direction);
  }
}
