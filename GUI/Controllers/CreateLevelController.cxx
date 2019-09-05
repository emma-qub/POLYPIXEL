#include "CreateLevelController.hxx"

#include "GUI/Models/CreateLevelModel.hxx"
#include "GUI/Models/ObjectModel.hxx"
#include "GUI/Views/CreateLevelView.hxx"
#include "GUI/Commands/CreateLevelCommands.hxx"
#include "Parser/Parser.hxx"

#include <QUndoStack>
#include <QAction>
#include <QItemSelectionModel>
#include <QToolBar>

CreateLevelController::CreateLevelController(CreateLevelView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_polygonModel(new CreateLevelModel(this)),
  m_view(p_view),
  m_undoStack(new QUndoStack(this)),
  m_toolbar(nullptr) {

  m_view->SetPolygonModel(m_polygonModel);
  m_objectModelsList
    << new TapeModel(this)
    << new MirrorModel(this)
    << new OneWayModel(this)
    << new PortalModel(this);
  m_view->SetObjectModelsList(m_objectModelsList);
  connect(m_polygonModel, &CreateLevelModel::dataChanged, m_view, &CreateLevelView::Redraw);
  for (auto* objectModel: m_objectModelsList) {
    connect(objectModel, &ObjectModel::dataChanged, m_view, &CreateLevelView::Redraw);
  }

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

  connect(m_view, &CreateLevelView::SnappedToGrid, this, &CreateLevelController::SnapToGrid);
  connect(m_view, &CreateLevelView::NewLevelRequested, this, &CreateLevelController::NewLevel);
  connect(m_view, &CreateLevelView::OpenLevelRequested, this, &CreateLevelController::OpenLevel);

  connect(m_undoStack, &QUndoStack::indexChanged, this, &CreateLevelController::UndoRedo);
  connect(m_undoStack, &QUndoStack::indexChanged, this, &CreateLevelController::CheckTestAvailable);

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

void CreateLevelController::SetToolBar(QToolBar* p_toolbar) {
  m_toolbar = p_toolbar;

  auto groupAction = new QActionGroup(m_toolbar);

  m_selectAction = new QAction("S", groupAction);
  m_toolbar->addAction(m_selectAction);
  m_selectAction->setCheckable(true);
  connect(m_selectAction, &QAction::triggered, m_view, &CreateLevelView::ActivateSelectionTool);

  m_polygonAction = new QAction("P", groupAction);
  m_toolbar->addAction(m_polygonAction);
  m_polygonAction->setCheckable(true);
  connect(m_polygonAction, &QAction::triggered, m_view, &CreateLevelView::ActivatePolygonTool);

  m_tapeAction = new QAction("T", groupAction);
  m_toolbar->addAction(m_tapeAction);
  m_tapeAction->setCheckable(true);
  connect(m_tapeAction, &QAction::triggered, m_view, &CreateLevelView::ActivateTapeTool);

  m_mirrorAction = new QAction("M", groupAction);
  m_toolbar->addAction(m_mirrorAction);
  m_mirrorAction->setCheckable(true);
  connect(m_mirrorAction, &QAction::triggered, m_view, &CreateLevelView::ActivateMirrorTool);

  m_oneWayAction = new QAction("O", groupAction);
  m_toolbar->addAction(m_oneWayAction);
  m_oneWayAction->setCheckable(true);
  connect(m_oneWayAction, &QAction::triggered, m_view, &CreateLevelView::ActivateOneWayTool);

  m_portalAction = new QAction("R", groupAction);
  m_toolbar->addAction(m_portalAction);
  m_portalAction->setCheckable(true);
  connect(m_portalAction, &QAction::triggered, m_view, &CreateLevelView::ActivatePortalTool);

  connect(m_view, &CreateLevelView::ToolActivated, this, &CreateLevelController::SelectTool);

  SelectTool(CreateLevelView::ePolygonTool);
}

void CreateLevelController::SelectTool(CreateLevelView::Tool p_tool) {
  switch(p_tool) {
  case CreateLevelView::eSelectionTool: {
    m_selectAction->trigger();
    break;
  } case CreateLevelView::ePolygonTool: {
    m_polygonAction->trigger();
    break;
  } case CreateLevelView::eTapeTool: {
    m_tapeAction->trigger();
    break;
  } case CreateLevelView::eMirrorTool: {
    m_mirrorAction->trigger();
    break;
  } case CreateLevelView::eOneWayTool: {
    m_oneWayAction->trigger();
    break;
  } case CreateLevelView::ePortalTool: {
    m_portalAction->trigger();
    break;
  }
  }
}

int CreateLevelController::GetLinesGoal() const {
  return m_view->GetLinesGoal();
}

int CreateLevelController::GetPartsGoal() const {
  return m_view->GetPartsGoal();
}

int CreateLevelController::GetMaxGapToWin() const {
  return m_view->GetMaxGapToWin();
}

int CreateLevelController::GetTolerance() const {
  return m_view->GetTolerance();
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

void CreateLevelController::SnapToGrid() {
  auto currentIndex = m_view->GetSelectionModel()->currentIndex();

  auto itemType = currentIndex.data(PolygonModel::eItemTypeRole).value<PolygonModel::ItemType>();
  switch(itemType) {
  case (CreateLevelModel::ePolygons): {
    currentIndex = QModelIndex();
    break;
  } case (CreateLevelModel::ePolygon): {
    break;
  } case (CreateLevelModel::eVertex): {
    currentIndex = currentIndex.parent();
    break;
  }
  default: {
    currentIndex = QModelIndex();
    break;
  }
  }

  if (currentIndex.isValid()) {
    SnapCurrentPolygonToGrid(currentIndex);
  }
}

void CreateLevelController::SnapCurrentPolygonToGrid(QModelIndex const& p_currentIndex) {
  auto* polygonItem = m_polygonModel->itemFromIndex(p_currentIndex);

  for (int row = 0; row < polygonItem->rowCount(); ++row) {
    auto oldX = polygonItem->child(row, 1)->data(Qt::DisplayRole).toInt();
    auto oldY = polygonItem->child(row, 2)->data(Qt::DisplayRole).toInt();

    int caseSide = 50;
    int nearestX = static_cast<int>(oldX)%50;
    int nearestY = static_cast<int>(oldY)%50;
    int newX = oldX;
    int newY = oldY;

    int threshold = 20;
    if (nearestY < threshold)
      newY = oldY - nearestY;
    else if (nearestY > caseSide - threshold)
      newY = oldY + (caseSide - nearestY);

    if (nearestX < threshold)
      newX = oldX - nearestX;
    else if (nearestX > caseSide - threshold)
      newX = oldX + (caseSide - nearestX);

    if (oldX != newX || oldY != newY) {
      MoveVertex(polygonItem->row(), row, ppxl::Vector(newX-oldX, newY-oldY), true);
    }
  }
}

void CreateLevelController::CheckTestAvailable() {
  // Check if a polygon has less than 3 vertices
  for (auto* polygon: m_polygonModel->GetPolygonsList()) {
    if (!polygon->HasEnoughVertices()) {
      m_view->SetTestAvailable(false);
      return;
    }
  }

  // Check polygon one by one
  for (auto* polygon: m_polygonModel->GetPolygonsList()) {
    if (!polygon->IsGoodPolygon()) {
      m_view->SetTestAvailable(false);
      return;
    }
  }

  // Check intersections between polygons
  for (auto* polygon1: m_polygonModel->GetPolygonsList()) {
    for (auto* polygon2: m_polygonModel->GetPolygonsList()) {
      if (polygon1 == polygon2) {
        continue;
      }
      for (auto const& vertex: polygon2->GetVertices()) {
        if (polygon1->IsPointInside(vertex)) {
          m_view->SetTestAvailable(false);
          return;
        }
      }
    }
  }

  m_view->SetTestAvailable(true);
}

void CreateLevelController::NewLevel() {
  m_undoStack->clear();
  m_polygonModel->ClearPolygons();
  m_view->ResetGameInfo();
  Redraw();
}

void CreateLevelController::OpenLevel(const QString& p_fileName) {
  m_undoStack->clear();

  Parser parser(p_fileName);
  m_view->SetLinesGoal(parser.GetLinesGoal());
  m_view->SetPartsGoal(parser.GetPartsGoal());
  m_view->SetMaxGapToWin(parser.GetMaxGapToWin());
  m_view->SetTolerance(parser.GetTolerance());
  m_polygonModel->SetPolygonsList(parser.GetPolygonsList());

  Redraw();
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
  auto selection = m_polygonModel->GetSelection();
  auto polygonRow = selection.last().first;
  auto vertexRow = selection.last().second;

  auto polygonsIndex = m_polygonModel->index(0, 0);
  if (polygonRow == -1 && vertexRow == -1) {
    selectionModel->setCurrentIndex(polygonsIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  } else {
    auto polygonIndex = m_polygonModel->index(polygonRow, 0, polygonsIndex);
    if (vertexRow == -1) {
      selectionModel->setCurrentIndex(polygonIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    } else {
      auto vertexIndex = m_polygonModel->index(vertexRow, 0, polygonIndex);
      selectionModel->setCurrentIndex(vertexIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }
  }
  Redraw();
}

void CreateLevelController::InsertPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon) {
  QUndoCommand* addPolygonCommand = new AddPolygonCommand(m_polygonModel, m_view->GetSelectionModel(), p_polygonRow, p_polygon, p_polygonRow, -1);
  m_undoStack->push(addPolygonCommand);
}

void CreateLevelController::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_polygonModel->rowCount(), p_polygon);
}

void CreateLevelController::RemovePolygon(int p_polygonRow) {
  int newPolygonRow = p_polygonRow;
  if (m_polygonModel->rowCount() == 0) {
    newPolygonRow = -1;
  } else {
    if (p_polygonRow == m_polygonModel->rowCount()-1) {
      newPolygonRow = p_polygonRow-1;
    }
  }

  auto* polygon = m_polygonModel->GetPolygonsList().at(p_polygonRow);

  QUndoCommand* removePolygonCommand = new RemovePolygonCommand(m_polygonModel, m_view->GetSelectionModel(), p_polygonRow, *polygon, newPolygonRow, -1);
  m_undoStack->push(removePolygonCommand);
}

void CreateLevelController::MovePolygon(int p_polygonRow, ppxl::Vector const& p_direction, bool p_pushToStack) {
  if (p_pushToStack) {
    QUndoCommand* movePolygonCommand = new MovePolygonCommand(m_polygonModel, m_view->GetSelectionModel(), p_polygonRow, p_direction, p_polygonRow, -1);
    m_undoStack->push(movePolygonCommand);
  } else {
    m_polygonModel->TranslatePolygon(p_polygonRow, p_direction);
  }
}

void CreateLevelController::InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex) {
  QUndoCommand* addVertexCommand = new AddVertexCommand(m_polygonModel, m_view->GetSelectionModel(), p_polygonRow, p_vertexRow, p_vertex, p_polygonRow, p_vertexRow);
  m_undoStack->push(addVertexCommand);
}

void CreateLevelController::AppendVertex(int p_polygonRow, const ppxl::Point& p_vertex) {
  InsertVertex(p_polygonRow, m_polygonModel->GetPolygonsItem()->child(p_polygonRow, 0)->rowCount(), p_vertex);
}

void CreateLevelController::RemoveVertex(int p_polygonRow, int p_vertexRow) {
  auto const& vertex = m_polygonModel->GetPolygonsList().at(p_polygonRow)->GetVertices().at(static_cast<unsigned long>(p_vertexRow));

  QUndoCommand* removeVertexCommand = new RemoveVertexCommand(m_polygonModel, m_view->GetSelectionModel(), p_polygonRow, p_vertexRow, vertex, p_polygonRow, 0);
  m_undoStack->push(removeVertexCommand);
}

void CreateLevelController::MoveVertex(int p_polygonRow, int p_vertexRow, const ppxl::Vector& p_direction, bool p_pushToStack) {
  if (p_pushToStack) {
    QUndoCommand* moverVertexCommand = new MoveVertexCommand(m_polygonModel, m_view->GetSelectionModel(), p_polygonRow, p_vertexRow, p_direction, p_polygonRow, p_vertexRow);
    m_undoStack->push(moverVertexCommand);
  } else {
    m_polygonModel->TranslateVertex(p_polygonRow, p_vertexRow, p_direction);
  }
}
