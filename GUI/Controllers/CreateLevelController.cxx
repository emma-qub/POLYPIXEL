#include "CreateLevelController.hxx"

#include "GUI/Models/ObjectModel.hxx"
#include "GUI/Views/CreateLevelView.hxx"
#include "GUI/Commands/CreateLevelCommands.hxx"
#include "Parser/Parser.hxx"

#include <QUndoStack>
#include <QAction>
#include <QItemSelectionModel>
#include <QToolBar>
#include <QMouseEvent>

#include <cmath>

CreateLevelController::CreateLevelController(CreateLevelView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_model(new CreateLevelModel(this)),
  m_view(p_view),
  m_undoStack(new QUndoStack(this)),
  m_toolbar(nullptr),
  m_objectStartPoint(),
  m_creatingObject(false),
  m_editingObject(false),
  m_mousePressed(false),
  m_nearControlPoint(false),
  m_hoveredItem(nullptr),
  m_selectedItem(nullptr) {

  m_view->SetModel(m_model);
  connect(m_model, &CreateLevelModel::dataChanged, m_view, &CreateLevelView::Redraw);

  m_view->SetUndoStack(m_undoStack);

  connect(m_view, &CreateLevelView::PolygonInserted, this, &CreateLevelController::InsertPolygon);
  connect(m_view, &CreateLevelView::PolygonRemoved, this, &CreateLevelController::RemovePolygon);
  connect(m_view, &CreateLevelView::PolygonMoved, this, &CreateLevelController::MovePolygon);
  connect(m_view, &CreateLevelView::VertexInserted, this, &CreateLevelController::InsertVertex);
  connect(m_view, &CreateLevelView::VertexRemoved, this, &CreateLevelController::RemoveVertex);
  connect(m_view, &CreateLevelView::VertexMoved, this, &CreateLevelController::MoveVertex);

  connect(m_view, &CreateLevelView::MousePressed, this, &CreateLevelController::MousePressEvent);
  connect(m_view, &CreateLevelView::MouseMoved, this, &CreateLevelController::MouseMoveEvent);
  connect(m_view, &CreateLevelView::MouseReleased, this, &CreateLevelController::MouseReleaseEvent);

  connect(m_view, &CreateLevelView::ValueXChanged, this, &CreateLevelController::UpdateXVertex);
  connect(m_view, &CreateLevelView::ValueYChanged, this, &CreateLevelController::UpdateYVertex);
  connect(m_view, &CreateLevelView::EditionXDone, this, &CreateLevelController::TranslateXVertex);
  connect(m_view, &CreateLevelView::EditionYDone, this, &CreateLevelController::TranslateYVertex);

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
  connect(m_selectAction, &QAction::toggled, [this](bool p_checked){
    m_toolMode = eSelectionMode;
    if (p_checked) {
      m_hoveredItem = nullptr;
      m_selectedItem = nullptr;
    }
  });

  m_polygonAction = new QAction("P", groupAction);
  m_toolbar->addAction(m_polygonAction);
  m_polygonAction->setCheckable(true);
  connect(m_polygonAction, &QAction::triggered, [this](){ m_toolMode = ePolygonMode; });

  auto tapeAction = new QAction("T", groupAction);
  m_toolbar->addAction(tapeAction);
  tapeAction->setCheckable(true);
  connect(tapeAction, &QAction::triggered, [this](){ m_toolMode = eTapeMode; });
  m_objectTypeAction[CreateLevelModel::eTape] = tapeAction;

  auto mirrorAction = new QAction("M", groupAction);
  m_toolbar->addAction(mirrorAction);
  mirrorAction->setCheckable(true);
  connect(mirrorAction, &QAction::triggered, [this](){ m_toolMode = eMirrorMode; });
  m_objectTypeAction[CreateLevelModel::eMirror] = mirrorAction;

  auto oneWayAction = new QAction("O", groupAction);
  m_toolbar->addAction(oneWayAction);
  oneWayAction->setCheckable(true);
  connect(oneWayAction, &QAction::triggered, [this](){ m_toolMode = eOneWayMode; });
  m_objectTypeAction[CreateLevelModel::eOneWay] = oneWayAction;

  auto portalAction = new QAction("R", groupAction);
  m_toolbar->addAction(portalAction);
  portalAction->setCheckable(true);
  connect(portalAction, &QAction::triggered, [this](){ m_toolMode = ePortalMode; });
  m_objectTypeAction[CreateLevelModel::ePortal] = portalAction;

  m_polygonAction->trigger();
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
  auto* polygon = p_index.parent().data(CreateLevelModel::eObjectRole).value<ppxl::Polygon*>();
  auto& vertex = polygon->GetVertices().at(static_cast<unsigned int>(p_index.row()));
  vertex.SetX(p_value);

  RedrawFromPolygons();
}

void CreateLevelController::UpdateYVertex(int p_value, QModelIndex const& p_index) {
  auto* polygon = p_index.parent().data(CreateLevelModel::eObjectRole).value<ppxl::Polygon*>();
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

  auto itemType = currentIndex.data(CreateLevelModel::eItemTypeRole).value<CreateLevelModel::ItemType>();
  switch(itemType) {
  case (CreateLevelModel::eObjectList): {
    currentIndex = QModelIndex();
    break;
  } case (CreateLevelModel::eObject): {
    break;
  } case (CreateLevelModel::ePoint): {
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
  auto* polygonItem = m_model->itemFromIndex(p_currentIndex);

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
  for (auto* polygon: m_model->GetPolygonsList()) {
    if (!polygon->HasEnoughVertices()) {
      m_view->SetTestAvailable(false);
      return;
    }
  }

  // Check polygon one by one
  for (auto* polygon: m_model->GetPolygonsList()) {
    if (!polygon->IsGoodPolygon()) {
      m_view->SetTestAvailable(false);
      return;
    }
  }

  // Check intersections between polygons
  for (auto* polygon1: m_model->GetPolygonsList()) {
    for (auto* polygon2: m_model->GetPolygonsList()) {
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

void CreateLevelController::MousePressEvent(QMouseEvent* p_event) {
  m_objectStartPoint = p_event->pos();
  m_mousePressed = true;

  switch(m_toolMode) {
  case eSelectionMode: {
    SelectObjectUnderCursor();
    m_selectedItem = m_hoveredItem;
    m_hoveredItem = nullptr;
    break;
  }
  case ePolygonMode: {
    break;
  }
  case eTapeMode:
  case eMirrorMode:
  case eOneWayMode:
  case ePortalMode: {
    if (m_nearControlPoint) {
      m_editingObject = true;
      m_creatingObject = false;
    } else {
      m_editingObject = false;
      m_creatingObject = true;
      CreateObect();
    }

    break;
  }
  }
}

void CreateLevelController::MouseMoveEvent(QMouseEvent* p_event) {
  switch(m_toolMode) {
  case eSelectionMode: {
    if (m_mousePressed == false) {
      HighlightObjectUnderCursor(p_event->pos());
    }
    break;
  }
  case ePolygonMode: {
    break;
  }
  case eTapeMode:
  case eMirrorMode:
  case eOneWayMode:
  case ePortalMode: {
    if (m_mousePressed) {
      if (m_creatingObject) {
        MoveNewObject(p_event->pos());
      } else {
        MoveExistingObject(p_event->pos());
      }
    } else if (m_selectedItem) {
      bool nearControlPoint = false;
      QPoint nearestControlPoint;
      FindNearestControlPoint(nearControlPoint, nearestControlPoint, p_event->pos());
      if (nearControlPoint) {
        m_nearControlPoint = true;
      }
    }
    break;
  }
  }
}

void CreateLevelController::MouseReleaseEvent(QMouseEvent* p_event) {
  m_objectStartPoint = QPoint();
  m_mousePressed = false;

  switch(m_toolMode) {
  case ePolygonMode: {
    break;
  }
  case eTapeMode:
  case eMirrorMode:
  case eOneWayMode:
  case ePortalMode: {
    m_creatingObject = false;
    m_editingObject = false;
    break;
  }
  default:
    break;
  }
}

void CreateLevelController::FindNearestControlPoint(bool& nearControlPoint, QPoint& nearestControlPoint, QPoint const& p_pos) const {
  auto object = m_selectedItem->data(CreateLevelModel::eGraphicsItemRole).value<GraphicsObjectItem*>();
  qDebug() << object->GetControlPoints();
}

void CreateLevelController::SelectObjectUnderCursor() {
  if (m_hoveredItem) {
    for (int objectsListRow = 0; objectsListRow < m_model->rowCount(); ++objectsListRow) {
      auto objectListIndex = m_model->index(objectsListRow, 0);
      for (int objectRow = 0; objectRow < m_model->rowCount(objectListIndex); ++objectRow) {
        auto objectIndex = m_model->index(objectRow, 0, objectListIndex);
        auto item = m_model->itemFromIndex(objectIndex);
        if (item == m_hoveredItem) {
          item->setData(CreateLevelModel::eSelected, CreateLevelModel::eStateRole);
          auto selectionModel = m_view->GetSelectionModel();
          selectionModel->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
          m_objectTypeAction[item->data(CreateLevelModel::eObjectTypeRole).value<CreateLevelModel::ObjectType>()]->trigger();
        } else {
          item->setData(CreateLevelModel::eDisabled, CreateLevelModel::eStateRole);
        }
      }
    }
  }
}

void CreateLevelController::CreateObect() {
  switch(m_toolMode) {
  case eTapeMode: {
    auto item = m_model->AddTape(Tape(m_objectStartPoint.x(), m_objectStartPoint.y(), 0., 0.));
    m_view->CreateObjectFromItem(item);
    break;
  }
  case eMirrorMode: {
    auto item = m_model->AddMirror(Mirror(m_objectStartPoint.x(), m_objectStartPoint.y(), m_objectStartPoint.x(), m_objectStartPoint.y()));
    m_view->CreateObjectFromItem(item);
    break;
  }
  case eOneWayMode: {
    auto item = m_model->AddOneWay(OneWay(m_objectStartPoint.x(), m_objectStartPoint.y(), m_objectStartPoint.x(), m_objectStartPoint.y()));
    m_view->CreateObjectFromItem(item);
    break;
  }
  case ePortalMode: {
    auto item = m_model->AddPortal(Portal(
      m_objectStartPoint.x(), m_objectStartPoint.y(), m_objectStartPoint.x(), m_objectStartPoint.y(),
      m_objectStartPoint.x()+10, m_objectStartPoint.y(), m_objectStartPoint.x()+10, m_objectStartPoint.y()));
    m_view->CreateObjectFromItem(item);
    break;
  }
  default:
    break;
  }
}

void CreateLevelController::MoveNewObject(const QPoint& p_pos) {
  if (m_creatingObject) {
    double nx;
    double ny;
    double ox = m_objectStartPoint.x();
    double oy = m_objectStartPoint.y();

    GetDiscreteEnd(p_pos, nx, ny);

    switch(m_toolMode) {
    case eTapeMode: {
      auto x1 = qMin(m_objectStartPoint.x(), p_pos.x());
      auto y1 = qMin(m_objectStartPoint.y(), p_pos.y());
      auto x2 = qMax(m_objectStartPoint.x(), p_pos.x());
      auto y2 = qMax(m_objectStartPoint.y(), p_pos.y());
      Tape tape(x1, y1, x2-x1, y2-y1);
      m_model->SetTape(m_model->GetTapesList().size()-1, &tape);
      break;
    }
    case eMirrorMode: {
      Mirror mirror(ox, oy, nx, ny);
      m_model->SetMirror(m_model->GetMirrorsList().size()-1, &mirror);
      break;
    }
    case eOneWayMode: {
      OneWay oneWay(ox, oy, nx, ny);
      m_model->SetOneWay(m_model->GetOneWaysList().size()-1, &oneWay);
      break;
    }
    case ePortalMode: {
      auto angle = ppxl::Vector::Angle(ppxl::Vector(ppxl::Point(ox, oy), ppxl::Point(ox+100, oy)), ppxl::Vector(ppxl::Point(ox, oy), ppxl::Point(nx, ny)))+(M_PI_4+M_PI_2)/3.;
      Portal portal(ox, oy, nx, ny, ox+std::sin(angle)*20., oy+std::cos(angle)*20., nx+std::sin(angle)*20., ny+std::cos(angle)*20.);
      m_model->SetPortal(m_model->GetPortalsList().size()-1, &portal);
      break;
    }
    default:
      break;
    }
  }
}

void CreateLevelController::MoveExistingObject(const QPoint& p_pos) {
  if (m_editingObject) {

  }
}

void CreateLevelController::GetDiscreteEnd(QPoint const& p_pos, double& p_nx, double& p_ny) {
  auto ox = static_cast<double>(m_objectStartPoint.x());
  auto oy = static_cast<double>(m_objectStartPoint.y());
  auto px = static_cast<double>(p_pos.x());
  auto py = static_cast<double>(p_pos.y());
  auto radius = ppxl::Point::Distance(ppxl::Point(ox, oy), ppxl::Point(px, py));

  p_nx = 2.*m_view->GetSceneRectWidth();
  p_ny = 2.*m_view->GetSceneRectHeight();
  double minDist = -1;
  for (int k = 0; k < 24; ++k) {
    auto dk = static_cast<double>(k);
    auto dx = radius*std::cos(dk*M_PI/12.)+ox;
    auto dy = radius*std::sin(dk*M_PI/12.)+oy;

    auto dist = ppxl::Point::Distance(ppxl::Point(px, py), ppxl::Point(dx, dy));
    if (minDist < 0 || dist < minDist) {
      p_nx = dx;
      p_ny = dy;
      minDist = dist;
    }
  }
}

void CreateLevelController::HighlightObjectUnderCursor(const QPoint& p_pos) {
  Object* objectUnderCursor = nullptr;
  for (auto object: m_model->GetObjectsList()) {
    if (object->Intersect(ppxl::Point(p_pos.x(), p_pos.y()), 10.)) {
      objectUnderCursor = object;
      break;
    }
  }

  for (int objectsListRow = 0; objectsListRow < m_model->rowCount(); ++objectsListRow) {
    auto objectListIndex = m_model->index(objectsListRow, 0);
    for (int objectRow = 0; objectRow < m_model->rowCount(objectListIndex); ++objectRow) {
      auto objectIndex = m_model->index(objectRow, 0, objectListIndex);
      auto item = m_model->itemFromIndex(objectIndex);
      bool isRightObject = item->data(CreateLevelModel::eItemTypeRole) == CreateLevelModel::eObject;
      Q_ASSERT_X(isRightObject, "CreateLevelController::HighlightObjectUnderCursor",
        tr("This item %1 has no object").arg(item->text()).toStdString().c_str());
      auto object = item->data(CreateLevelModel::eObjectRole).value<Object*>();
      if (object != nullptr) {
        if (objectUnderCursor == nullptr) {
          item->setData(ObjectModel::eEnabled, CreateLevelModel::eStateRole);
          m_hoveredItem = nullptr;
        } else {
          if (object == objectUnderCursor) {
            item->setData(ObjectModel::eHighlightUp, CreateLevelModel::eStateRole);
            m_hoveredItem = item;
          } else {
            item->setData(ObjectModel::eHighlightDown, CreateLevelModel::eStateRole);
          }
        }
      }
    }
  }
}

void CreateLevelController::NewLevel() {
  m_undoStack->clear();
  m_model->ClearPolygons();
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
  m_model->SetPolygonsList(parser.GetPolygonsList());

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
