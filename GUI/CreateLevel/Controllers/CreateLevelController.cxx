#include "CreateLevelController.hxx"

#include "GUI/CreateLevel/Views/CreateLevelWidget.hxx"
#include "Parser/Parser.hxx"

#include <QAction>
#include <QItemSelectionModel>
#include <QToolBar>
#include <QMouseEvent>

#include <cmath>

CreateLevelController::CreateLevelController(CreateLevelWidget* p_view, QObject* p_parent):
  QObject(p_parent),
  m_objectsListModel(new CreateLevelObjectsListModel(this)),
  m_objectsDetailModel(new CreateLevelObjectsDetailModel(this)),
  m_vertexListModel(new CreateLevelVertexListModel(this)),
  m_createLevelWidget(p_view),
  m_toolbar(nullptr),
  m_objectStartPoint(),
  m_creatingNewPolygon(false),
  m_creatingObject(false),
  m_mousePressed(false),
  m_isNearVertex(false),
  m_nearestVertex(),
  m_nearestVertexRow(-1),
  m_isNearControlPoint(false),
  m_nearestControlPoint(),
  m_hoveredItem(nullptr),
  m_clipboardIndex() {

  m_createLevelWidget->SetObjectsListModel(m_objectsListModel);
  m_createLevelWidget->SetObjectsDetailModel(m_objectsDetailModel);
  m_createLevelWidget->SetVertexListModel(m_vertexListModel);

  connect(m_objectsListModel, &CreateLevelObjectsListModel::rowsAboutToBeRemoved, this, &CreateLevelController::UpdateClipboardIndex);
  connect(m_objectsListModel, &CreateLevelObjectsListModel::dataChanged, this, &CreateLevelController::CheckTestAvailable);
  connect(m_objectsListModel, &CreateLevelObjectsListModel::rowsInserted, this, &CreateLevelController::CheckTestAvailable);
  connect(m_objectsListModel, &CreateLevelObjectsListModel::rowsRemoved, this, &CreateLevelController::CheckTestAvailable);
  connect(m_createLevelWidget, &CreateLevelWidget::CurrentVertexChanged, this, &CreateLevelController::UpdateCurrentVertex);
  connect(m_vertexListModel, &CreateLevelObjectsListModel::dataChanged, this, &CreateLevelController::CheckTestAvailable);
  connect(m_vertexListModel, &CreateLevelObjectsListModel::rowsInserted, this, &CreateLevelController::CheckTestAvailable);
  connect(m_vertexListModel, &CreateLevelObjectsListModel::rowsRemoved, this, &CreateLevelController::CheckTestAvailable);

  connect(m_createLevelWidget, &CreateLevelWidget::MousePressed, this, &CreateLevelController::MousePressEvent);
  connect(m_createLevelWidget, &CreateLevelWidget::MouseMoved, this, &CreateLevelController::MouseMoveEvent);
  connect(m_createLevelWidget, &CreateLevelWidget::MouseReleased, this, &CreateLevelController::MouseReleaseEvent);

  connect(m_createLevelWidget, &CreateLevelWidget::SnapToGridRequested, this, &CreateLevelController::SnapToGrid);
  connect(m_createLevelWidget, &CreateLevelWidget::SnapAllToGridRequested, this, &CreateLevelController::SnapAllToGrid);
  connect(m_createLevelWidget, &CreateLevelWidget::NewLevelRequested, this, &CreateLevelController::NewLevel);
  connect(m_createLevelWidget, &CreateLevelWidget::OpenLevelRequested, this, &CreateLevelController::OpenLevel);
  connect(m_createLevelWidget, &CreateLevelWidget::KeyDeletePressed, this, &CreateLevelController::DeleteCurrent);
  connect(m_createLevelWidget, &CreateLevelWidget::KeyReturnPressed, this, &CreateLevelController::PolygonComplete);
  connect(m_createLevelWidget, &CreateLevelWidget::KeyLeftPressed, this, &CreateLevelController::MoveCurrentLeft);
  connect(m_createLevelWidget, &CreateLevelWidget::KeyUpPressed, this, &CreateLevelController::MoveCurrentUp);
  connect(m_createLevelWidget, &CreateLevelWidget::KeyRightPressed, this, &CreateLevelController::MoveCurrentRight);
  connect(m_createLevelWidget, &CreateLevelWidget::KeyDownPressed, this, &CreateLevelController::MoveCurrentDown);
  connect(m_createLevelWidget, &CreateLevelWidget::SelectionChanged, this, &CreateLevelController::UpdateSelection);
  connect(m_createLevelWidget, &CreateLevelWidget::CopyRequested, this, &CreateLevelController::CopyItem);
  connect(m_createLevelWidget, &CreateLevelWidget::PasteRequested, this, &CreateLevelController::PasteItem);
}

CreateLevelController::~CreateLevelController() = default;

void CreateLevelController::SetToolBar(QToolBar* p_toolbar) {
  m_toolbar = p_toolbar;

  auto groupAction = new QActionGroup(m_toolbar);

  m_selectAction = new QAction(QIcon("../POLYPIXEL/resources/icons/tools/selectionToolIcon.png"), "Selection (S)", groupAction);
  m_selectAction->setShortcut(QKeySequence(Qt::Key_S));
  m_toolbar->addAction(m_selectAction);
  m_selectAction->setCheckable(true);
  connect(m_selectAction, &QAction::triggered, [this](){
    m_toolMode = eSelectionMode;
    DisableObjectItems();
    m_hoveredItem = nullptr;
  });
  m_actionToolModeMap[m_selectAction] = eSelectionMode;

  m_polygonAction = new QAction(QIcon("../POLYPIXEL/resources/icons/tools/polygonToolIcon.png"), "Polygon (P)", groupAction);
  m_polygonAction->setShortcut(QKeySequence(Qt::Key_P));
  m_toolbar->addAction(m_polygonAction);
  m_polygonAction->setCheckable(true);
  m_actionToolModeMap[m_polygonAction] = ePolygonMode;
  m_objectTypeAction[CreateLevelObjectsListModel::ePolygonObjectType] = m_polygonAction;

  m_tapeAction = new QAction(QIcon("../POLYPIXEL/resources/icons/tools/tapeToolIcon.png"), "Tape (T)", groupAction);
  m_tapeAction->setShortcut(QKeySequence(Qt::Key_T));
  m_toolbar->addAction(m_tapeAction);
  m_tapeAction->setCheckable(true);
  m_actionToolModeMap[m_tapeAction] = eTapeMode;
  m_objectTypeAction[CreateLevelObjectsListModel::eTapeObjectType] = m_tapeAction;

  m_mirrorAction = new QAction(QIcon("../POLYPIXEL/resources/icons/tools/mirrorToolIcon.png"), "Mirror (M)", groupAction);
  m_mirrorAction->setShortcut(QKeySequence(Qt::Key_M));
  m_toolbar->addAction(m_mirrorAction);
  m_mirrorAction->setCheckable(true);
  m_actionToolModeMap[m_mirrorAction] = eMirrorMode;
  m_objectTypeAction[CreateLevelObjectsListModel::eMirrorObjectType] = m_mirrorAction;

  m_oneWayAction = new QAction(QIcon("../POLYPIXEL/resources/icons/tools/oneWayToolIcon.png"), "One Way (O)", groupAction);
  m_oneWayAction->setShortcut(QKeySequence(Qt::Key_O));
  m_toolbar->addAction(m_oneWayAction);
  m_oneWayAction->setCheckable(true);
  m_actionToolModeMap[m_oneWayAction] = eOneWayMode;
  m_objectTypeAction[CreateLevelObjectsListModel::eOneWayObjectType] = m_oneWayAction;

  m_portalAction = new QAction(QIcon("../POLYPIXEL/resources/icons/tools/portalToolIcon.png"), "Portal (L)", groupAction);
  m_portalAction->setShortcut(QKeySequence(Qt::Key_L));
  m_toolbar->addAction(m_portalAction);
  m_portalAction->setCheckable(true);
  m_actionToolModeMap[m_portalAction] = ePortalMode;
  m_objectTypeAction[CreateLevelObjectsListModel::ePortalObjectType] = m_portalAction;

  m_rectangleSelectionAction = new QAction(QIcon("../POLYPIXEL/resources/icons/tools/rectangleSelectionToolIcon.png"), "Portal (R)", groupAction);

  m_rectangleSelectionAction->setShortcut(QKeySequence(Qt::Key_R));
  m_toolbar->addAction(m_rectangleSelectionAction);
  m_rectangleSelectionAction->setCheckable(true);
  connect(m_rectangleSelectionAction, &QAction::triggered, [this](){
    m_toolMode = eRectangleSelectionMode;
    DisableObjectItems();
    m_hoveredItem = nullptr;
  });
  m_actionToolModeMap[m_rectangleSelectionAction] = eRectangleSelectionMode;

  ConnectToolsActions();

  m_objectTypeAction[CreateLevelObjectsListModel::ePolygonObjectType]->trigger();

  connect(m_createLevelWidget, &CreateLevelWidget::CurrentObjectIndexChanged, this, &CreateLevelController::UpdateGraphicsSelection);

  for (auto action: m_actionToolModeMap.keys()) {
    m_createLevelWidget->addAction(action);
  }
}

int CreateLevelController::GetLinesGoal() const {
  return m_createLevelWidget->GetLinesGoal();
}

int CreateLevelController::GetPartsGoal() const {
  return m_createLevelWidget->GetPartsGoal();
}

int CreateLevelController::GetMaxGapToWin() const {
  return m_createLevelWidget->GetMaxGapToWin();
}

int CreateLevelController::GetTolerance() const {
  return m_createLevelWidget->GetTolerance();
}

std::vector<ppxl::Polygon*> CreateLevelController::GetPolygonsList() const {
  return m_objectsListModel->GetPolygonsList();
}

std::vector<Object*> CreateLevelController::GetObjectsList() const {
  return m_objectsListModel->GetObjectsList();
}

void CreateLevelController::SnapToGrid() {
  auto currentIndex = m_createLevelWidget->FindCurrentPolygonIndex();
  if (currentIndex.isValid()) {
    SnapPolygonToGrid(currentIndex);
    return;
  }

  currentIndex = m_createLevelWidget->FindCurrentObjectIndex();
  if (currentIndex.isValid()) {
    SnapObjectToGrid(currentIndex);
  }
}

void CreateLevelController::SnapAllToGrid() {
  for (int listRow = 0; listRow < m_objectsListModel->rowCount(); ++listRow) {
    auto listIndex = m_objectsListModel->index(listRow, 0);
    for (int row = 0; row < m_objectsListModel->rowCount(listIndex); ++row) {
      auto index = m_objectsListModel->index(row, 0, listIndex);
      if (m_objectsListModel->IsPolygonIndex(index)) {
        SnapPolygonToGrid(index);
      } else if (m_objectsListModel->IsObjectIndex(index)) {
        SnapObjectToGrid(index);
      }
    }
  }
}

ppxl::Point CreateLevelController::FindNearestGridNode(ppxl::Point const& p_point) {
  auto gridNode = p_point;
  auto x = p_point.GetX();
  auto y = p_point.GetY();

  int caseSide = 50;
  int threshold = 20;
  int nearestX = static_cast<int>(x)%caseSide;
  int nearestY = static_cast<int>(y)%caseSide;

  if (nearestX < threshold)
    gridNode.SetX(x - nearestX);
  else if (nearestX > caseSide - threshold)
    gridNode.SetX(x - nearestX + caseSide);

  if (nearestY < threshold)
    gridNode.SetY(y - nearestY);
  else if (nearestY > caseSide - threshold)
    gridNode.SetY(y - nearestY + caseSide);

  return gridNode;
}

void CreateLevelController::SnapObjectToGrid(QModelIndex const& p_currentIndex) {
  auto object = m_objectsListModel->GetObjectFromIndex(p_currentIndex);

  auto objctType = object->GetObjectType();
  switch (objctType) {
  case Object::eTape:{
    m_objectsListModel->MoveObject(p_currentIndex, FindNearestGridNode(object->GetTopLeft()), Object::eTopLeft);
    object->MoveControlPoint(FindNearestGridNode(object->GetBottomRight()), Object::eBottomRight);
    break;
  } case Object::eMirror:{
    auto mirror = static_cast<Mirror*>(object);
    if (static_cast<int>(mirror->GetX1()) == static_cast<int>(mirror->GetX2())
      || static_cast<int>(mirror->GetY1()) == static_cast<int>(mirror->GetY2())
      || static_cast<int>(std::abs(mirror->GetX2()-mirror->GetX1())) == static_cast<int>(mirror->GetY2()-mirror->GetY1())) {
      object->MoveControlPoint(FindNearestGridNode(object->GetTopLeft()), Object::eTopLeft);
      object->MoveControlPoint(FindNearestGridNode(object->GetBottomRight()), Object::eBottomRight);
    }
    break;
  } case Object::eOneWay:{
    auto oneWay = static_cast<OneWay*>(object);
    if (static_cast<int>(std::abs(oneWay->GetX1()) - oneWay->GetX2()) == 0
      || static_cast<int>(std::abs(oneWay->GetY1() - oneWay->GetY2())) == 0
      || static_cast<int>(std::abs(oneWay->GetX2()-oneWay->GetX1()) - std::abs(oneWay->GetY2()-oneWay->GetY1())) == 0) {
      object->MoveControlPoint(FindNearestGridNode(object->GetTopLeft()), Object::eTopLeft);
      object->MoveControlPoint(FindNearestGridNode(object->GetBottomRight()), Object::eBottomRight);
    }
    break;
  } case Object::ePortal:{
    auto portal = static_cast<Portal*>(object);
    if (static_cast<int>(portal->GetIn().GetA().GetX()) == static_cast<int>(portal->GetIn().GetB().GetX())
      || static_cast<int>(portal->GetIn().GetA().GetY()) == static_cast<int>(portal->GetIn().GetB().GetY())
      || static_cast<int>(std::abs(portal->GetIn().GetB().GetX()-portal->GetIn().GetA().GetX())) == static_cast<int>(portal->GetIn().GetB().GetY()-portal->GetIn().GetA().GetY())) {
      object->MoveControlPoint(FindNearestGridNode(object->GetTopLeftYellow()), Object::eTopLeftYellow);
      object->MoveControlPoint(FindNearestGridNode(object->GetBottomRightYellow()), Object::eBottomRightYellow);
    }
    if (static_cast<int>(portal->GetOut().GetA().GetX()) == static_cast<int>(portal->GetOut().GetB().GetX())
      || static_cast<int>(portal->GetOut().GetA().GetY()) == static_cast<int>(portal->GetOut().GetB().GetY())
      || static_cast<int>(std::abs(portal->GetOut().GetB().GetX()-portal->GetOut().GetA().GetX())) == static_cast<int>(portal->GetOut().GetB().GetY()-portal->GetOut().GetA().GetY())) {
      object->MoveControlPoint(FindNearestGridNode(object->GetTopLeftBlue()), Object::eTopLeftBlue);
      object->MoveControlPoint(FindNearestGridNode(object->GetBottomRightBlue()), Object::eBottomRightBlue);
    }
    break;
  } default:
    break;
  }
}

void CreateLevelController::SnapPolygonToGrid(QModelIndex const& p_currentIndex) {
  auto polygon = m_objectsListModel->GetPolygonFromIndex(p_currentIndex);

  int vertexRow = 0;
  for (auto vertex: polygon->GetVertices()) {
    auto newVertex = FindNearestGridNode(vertex);
    if (newVertex != vertex) {
      MoveVertexAt(vertexRow, newVertex);
    }
    ++vertexRow;
  }
}

void CreateLevelController::CheckTestAvailable() {
  auto const& polygonsList = m_objectsListModel->GetPolygonsList();

  for (auto polygon: polygonsList) {
    // Check if a polygon has less than 3 vertices and is not crossed
    if (!polygon->HasEnoughVertices() || !polygon->IsGoodPolygon()) {
      m_createLevelWidget->SetTestAvailable(false);
      if (!polygon->HasEnoughVertices())
        /// qDebug() << "Not enough vertices";
      if (!polygon->IsGoodPolygon())
        /// qDebug() << "Vertex alignment issue";
      return;
    }
    // Check intersections between polygons
    for (auto polygon2: polygonsList) {
      if (polygon == polygon2) {
        continue;
      }
      auto vertices2 = polygon2->GetVertices();
      for (auto k = 0ul; k < vertices2.size(); ++k) {
        auto A = vertices2.at(k);
        if (polygon->IsPointInside(A)) {
          m_createLevelWidget->SetTestAvailable(false);
          /// qDebug() << "Polygons overlap issue";
          return;
        }

        auto B = vertices2.at((k+1)%vertices2.size());
        if (polygon->IsCrossing(ppxl::Segment(A, B))) {
          m_createLevelWidget->SetTestAvailable(false);
          /// qDebug() << "Polygons overlap issue";
          return;
        }
      }
    }
  }

  m_createLevelWidget->SetTestAvailable(true);
}

void CreateLevelController::MousePressEvent(QMouseEvent* p_event) {
  m_objectStartPoint = p_event->pos();
  m_mousePressed = true;

  switch(m_toolMode) {
  case eSelectionMode: {
    SelectObjectUnderCursor(p_event->pos());
    break;
  } case ePolygonMode: {
    if (m_isNearVertex) {
      m_createLevelWidget->setCursor(Qt::ClosedHandCursor);
      m_createLevelWidget->SetCurrentVertexIndex(m_nearestVertexRow);
    } else if (m_creatingNewPolygon) {
      CreatePolygon();
      InsertVertex(m_objectStartPoint);
      m_creatingNewPolygon = false;
    } else {
      InsertVertex(m_objectStartPoint);
    }
    break;
  }
  case eTapeMode:
  case eMirrorMode:
  case eOneWayMode:
  case ePortalMode: {
    m_createLevelWidget->setCursor(Qt::ClosedHandCursor);
    if (m_isNearControlPoint) {
      m_creatingObject = false;
      FindNearestControlPoint(m_isNearControlPoint, m_nearestControlPoint, p_event->pos());
    } else {
      m_creatingObject = true;
      DisableObjectItems();
      CreateObject();
    }
    break;
  } case eRectangleSelectionMode: {
    m_createLevelWidget->setCursor(Qt::CrossCursor);
    m_createLevelWidget->SetRubberBandDragMode(true);
    break;
  } default:
    break;
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
    if (m_mousePressed) {
      if (m_nearestVertex != ppxl::Point()) {
        auto newVertex = ppxl::Point(p_event->pos().x(), p_event->pos().y());
        int vertexRow = m_nearestVertexRow;
        ppxl::Vector direction(m_nearestVertex, newVertex);
        if (vertexRow == -1) {
          TranslatePolygon(direction);
        } else {
          MoveCurrentVertex(newVertex);
        }
        m_nearestVertex = newVertex;
      }
    } else {
      FindNearestVertex(m_isNearVertex, m_nearestVertex, m_nearestVertexRow, p_event->pos());
      if (m_isNearVertex) {
        m_createLevelWidget->setCursor(Qt::OpenHandCursor);
      } else {
        m_createLevelWidget->setCursor(Qt::ArrowCursor);
      }
    }
    break;
  }
  case eTapeMode:
  case eMirrorMode:
  case eOneWayMode:
  case ePortalMode: {
    if (m_mousePressed) {
      MoveObject(p_event->pos());
    } else {
      FindNearestControlPoint(m_isNearControlPoint, m_nearestControlPoint, p_event->pos());
      if (m_isNearControlPoint) {
        m_createLevelWidget->setCursor(Qt::OpenHandCursor);
      } else {
        m_createLevelWidget->setCursor(Qt::ArrowCursor);
      }
    }
    break;
  } case eRectangleSelectionMode: {
    if (m_mousePressed) {
      MoveRectangleSelection(p_event->pos());
    }
    break;
  } default:
    break;
  }
}

void CreateLevelController::MouseReleaseEvent(QMouseEvent*) {
  m_objectStartPoint = QPoint();
  m_mousePressed = false;

  switch(m_toolMode) {
  case ePolygonMode: {
    m_createLevelWidget->setCursor(Qt::OpenHandCursor);
    break;
  }
  case eTapeMode:
  case eMirrorMode:
  case eOneWayMode:
  case ePortalMode: {
    m_creatingObject = false;
    m_createLevelWidget->setCursor(Qt::OpenHandCursor);
    if (m_toolMode == ePortalMode) {
      auto currentIndex = m_createLevelWidget->GetCurrentObjectIndex();
      auto portal = static_cast<Portal*>(currentIndex.data(CreateLevelObjectsListModel::eObjectRole).value<Object*>());
      portal->SetCreating(false);
      currentIndex.data(CreateLevelObjectsListModel::eGraphicsItemRole).value<GraphicsObjectItem*>()->UpdateControlPoints();
    }
    break;
  } case eRectangleSelectionMode: {
    m_createLevelWidget->SetRubberBandDragMode(false);
    m_createLevelWidget->setCursor(Qt::ArrowCursor);
    break;
  }
  default:
    break;
  }
}

void CreateLevelController::DisableObjectItems() {
  for (int objectsListRow = 0; objectsListRow < m_objectsListModel->rowCount(); ++objectsListRow) {
    auto objectListIndex = m_objectsListModel->index(objectsListRow, 0);
    for (int objectRow = 0; objectRow < m_objectsListModel->rowCount(objectListIndex); ++objectRow) {
      auto objectIndex = m_objectsListModel->index(objectRow, 0, objectListIndex);
      auto graphicsItem = m_objectsListModel->GetGraphicsFromIndex(objectIndex);
      graphicsItem->SetState(GraphicsObjectItem::eDisabledState);
    }
  }
}

void CreateLevelController::FindNearestVertex(bool& p_isNearVertex, ppxl::Point& p_nearestVertex, int& p_nearestVertexRow, QPoint const& p_pos) const {
  auto currentIndex = m_createLevelWidget->FindCurrentPolygonIndex();
  ppxl::Polygon* polygon = m_objectsListModel->FindPolygonFromIndex(currentIndex);

  p_isNearVertex = false;
  p_nearestVertex = ppxl::Point();
  p_nearestVertexRow = -1;

  if (polygon) {
    auto vertices = polygon->GetVertices();
    auto pos = ppxl::Point(p_pos.x(), p_pos.y());
    for (auto row = 0ul; row < vertices.size(); ++row) {
      auto vertex = polygon->GetVertices().at(row);
      if (ppxl::Point::Distance(pos, vertex) < 10) {
        p_isNearVertex = true;
        p_nearestVertex = vertex;
        p_nearestVertexRow = row;
        return;
      }
    }
    if (vertices.size() > 2 && ppxl::Point::Distance(pos, polygon->Barycenter()) < 10) {
      p_isNearVertex = true;
      p_nearestVertex = polygon->Barycenter();
    }
  }

}

void CreateLevelController::FindNearestControlPoint(bool& p_isNearControlPoint, QPair<QPoint, Object::ControlPointType>& p_nearestControlPoint, QPoint const& p_pos) const {
  auto currentIndex = m_createLevelWidget->FindCurrentObjectIndex();
  GraphicsObjectItem* graphicsObjectItem = nullptr;
  if (currentIndex.isValid()) {
    graphicsObjectItem = m_objectsListModel->GetGraphicsFromIndex(currentIndex);
  }

  if (graphicsObjectItem) {
    auto controlPoints = graphicsObjectItem->GetControlPoints();
    for (auto const& controlPointAndType: controlPoints) {
      auto controlPoint = controlPointAndType.first;
      if (ppxl::Point::Distance(ppxl::Point(controlPoint.x(), controlPoint.y()), ppxl::Point(p_pos.x(), p_pos.y())) < 10) {
        p_isNearControlPoint = true;
        p_nearestControlPoint = controlPointAndType;
        return;
      }
    }
  }

  p_isNearControlPoint = false;
  p_nearestControlPoint = QPair<QPoint, Object::ControlPointType>();
}

void CreateLevelController::SelectObjectUnderCursor(QPoint const& p_pos) {
  ppxl::Point pos(p_pos.x(), p_pos.y());

  for (int listRow = 0; listRow < m_objectsListModel->rowCount(); ++listRow) {
    auto listIndex = m_objectsListModel->index(listRow, 0);
    for (int row = 0; row < m_objectsListModel->rowCount(listIndex); ++row) {
      auto index = m_objectsListModel->index(row, 0, listIndex);
      auto graphicsItem = m_objectsListModel->GetGraphicsFromIndex(index);
      auto item = m_objectsListModel->itemFromIndex(index);
      if (m_hoveredItem == item) {
        if (graphicsItem->Intersect(pos)) {
          graphicsItem->SetState(GraphicsObjectItem::eSelectedState);
          m_createLevelWidget->SetCurrentObjectOrPolygonIndex(index);
        } else {
          graphicsItem->SetState(GraphicsObjectItem::eDisabledState);
        }
      }
    }
  }
}

void CreateLevelController::CreateObject(Object* p_object) {
  QStandardItem* item = nullptr;
  GraphicsObjectItem* graphicsItem = nullptr;
  switch(m_toolMode) {
  case eTapeMode: {
    auto tape = static_cast<Tape*>(p_object);
    if (!tape) {
      tape = new Tape(m_objectStartPoint.x(), m_objectStartPoint.y(), 0., 0.);
    }
    graphicsItem = new GraphicsTapeItem(tape);
    item = m_objectsListModel->AddTape(tape, graphicsItem);
    break;
  }
  case eMirrorMode: {
    auto mirror = static_cast<Mirror*>(p_object);
    if (!mirror) {
      mirror = new Mirror(m_objectStartPoint.x(), m_objectStartPoint.y(), m_objectStartPoint.x(), m_objectStartPoint.y());
    }
    graphicsItem = new GraphicsMirrorItem(mirror);
    item = m_objectsListModel->AddMirror(mirror, graphicsItem);
    break;
  }
  case eOneWayMode: {
    auto oneWay = static_cast<OneWay*>(p_object);
    if (!oneWay) {
      oneWay = new OneWay(m_objectStartPoint.x(), m_objectStartPoint.y(), m_objectStartPoint.x(), m_objectStartPoint.y());
    }
    graphicsItem = new GraphicsOneWayItem(oneWay);
    item = m_objectsListModel->AddOneWay(oneWay, graphicsItem);
    break;
  }
  case ePortalMode: {
    auto shift = 10;
    auto portal = static_cast<Portal*>(p_object);
    if (!portal) {
      portal = new Portal(
        m_objectStartPoint.x()-shift, m_objectStartPoint.y(), m_objectStartPoint.x()-shift, m_objectStartPoint.y(),
        m_objectStartPoint.x()+shift, m_objectStartPoint.y(), m_objectStartPoint.x()+shift, m_objectStartPoint.y());
    }
    graphicsItem = new GraphicsPortalItem(portal);
    item = m_objectsListModel->AddPortal(portal, graphicsItem);
    break;
  }
  default:
    break;
  }

  if (item) {
    m_createLevelWidget->AddGraphicsItem(graphicsItem);
    graphicsItem->SetState(GraphicsObjectItem::eSelectedState);
    connect(graphicsItem, &GraphicsObjectItem::StateChanged, m_createLevelWidget, &CreateLevelWidget::UpdateView);
    m_createLevelWidget->SetCurrentObjectOrPolygonIndex(item->index());
    m_createLevelWidget->ShowDetailListView();
    m_objectsDetailModel->ResetCurrentObject(m_objectsListModel->GetObjectFromItem(item));
  }
}

void CreateLevelController::MoveObject(QPoint const& p_pos) {
  Object::ControlPointType controlPointType;
  if (m_creatingObject) {
    controlPointType = Object::eBottomRight;
  } else {
    controlPointType = m_nearestControlPoint.second;
  }

  auto currentObjectIndex = m_createLevelWidget->GetCurrentObjectIndex();
  auto currentGraphicsItem = m_objectsListModel->GetGraphicsFromIndex(currentObjectIndex);
  m_objectsListModel->MoveObject(currentObjectIndex, ppxl::Point(p_pos.x(), p_pos.y()), controlPointType);
  m_objectsDetailModel->UpdateCurrentObject();
  currentGraphicsItem->ComputeBoundingPolygon();
}

void CreateLevelController::TranslateObject(ppxl::Vector const& p_direction) {
  auto currentObjectIndex = m_createLevelWidget->GetCurrentObjectIndex();
  auto currentGraphicsItem = m_objectsListModel->GetGraphicsFromIndex(currentObjectIndex);
  m_objectsListModel->TranslateObject(currentObjectIndex, p_direction);
  m_objectsDetailModel->UpdateCurrentObject();
  currentGraphicsItem->ComputeBoundingPolygon();
}

void CreateLevelController::HighlightObjectUnderCursor(QPoint const& p_pos) {
  m_hoveredItem = nullptr;
  ppxl::Point pos(p_pos.x(), p_pos.y());

  for (int listRow = 0; listRow < m_objectsListModel->rowCount(); ++listRow) {
    auto listIndex = m_objectsListModel->index(listRow, 0);
    for (int row = 0; row < m_objectsListModel->rowCount(listIndex); ++row) {
      auto index = m_objectsListModel->index(row, 0, listIndex);
      auto graphicsItem = m_objectsListModel->GetGraphicsFromIndex(index);
      if (graphicsItem) {
        if (m_hoveredItem) {
          graphicsItem->SetState(GraphicsObjectItem::eHighlightDownState);
        } else if (graphicsItem->Intersect(pos)) {
          graphicsItem->SetState(GraphicsObjectItem::eHighlightUpState);
          m_hoveredItem = m_objectsListModel->itemFromIndex(index);
        } else {
          graphicsItem->SetState(GraphicsObjectItem::eHighlightDownState);
        }
      }
    }
  }
}

void CreateLevelController::MoveRectangleSelection(QPoint const& p_pos) {
  m_createLevelWidget->SetSelectionArea(QRect(m_objectStartPoint, p_pos));
}

void CreateLevelController::NewLevel() {
  m_objectStartPoint = QPoint();
  m_creatingNewPolygon = true;
  m_creatingObject = false;
  m_mousePressed = false;
  m_isNearVertex = false;
  m_nearestVertex = ppxl::Point();
  m_nearestVertexRow = -1;
  m_isNearControlPoint = false;
  m_nearestControlPoint = {};
  m_hoveredItem = nullptr;

  m_createLevelWidget->ClearImage();
  m_objectsListModel->Clear();
  m_objectsDetailModel->ClearObject();
  m_vertexListModel->ClearPolygon();
  m_createLevelWidget->ResetGameInfo();
}

void CreateLevelController::OpenLevel(const QString& p_fileName) {
  NewLevel();
  Parser parser(p_fileName);
  m_createLevelWidget->SetLinesGoal(parser.GetLinesGoal());
  m_createLevelWidget->SetPartsGoal(parser.GetPartsGoal());
  m_createLevelWidget->SetMaxGapToWin(parser.GetMaxGapToWin());
  m_createLevelWidget->SetTolerance(parser.GetTolerance());
  for (auto const& polygon: parser.GetPolygonsList()){
    CreatePolygon(polygon);
  }
  m_toolMode = eTapeMode;
  for (auto const& tape: parser.GetTapesList()) {
    CreateObject(new Tape(tape));
  }
  m_toolMode = eMirrorMode;
  for (auto const& tape: parser.GetMirrorsList()) {
    CreateObject(new Mirror(tape));
  }
  m_toolMode = eOneWayMode;
  for (auto const& tape: parser.GetOneWaysList()) {
    CreateObject(new OneWay(tape));
  }
  m_toolMode = ePortalMode;
  for (auto const& tape: parser.GetPortalsList()) {
    CreateObject(new Portal(tape));
  }
  m_selectAction->trigger();
}

/// REWORK
void CreateLevelController::UpdateGraphicsSelection(QModelIndex const& p_current, QModelIndex const&) {
  qDebug() << "UPDATE GRAPHICS SELECTION";
  DisableObjectItems();
  auto objectListType = CreateLevelObjectsListModel::eUnknownObjectListType;
  QModelIndex listIndex;
  if (m_objectsListModel->IsObjectIndex(p_current)) {
    m_objectsListModel->GetGraphicsFromIndex(p_current)->SetState(GraphicsObjectItem::eSelectedState);
    listIndex = p_current.parent();
  } else {
    listIndex = p_current;
  }
  objectListType = m_objectsListModel->GetListTypFromIndex(listIndex);

  DisconnectToolsActions();
  auto action = m_objectTypeAction[static_cast<CreateLevelObjectsListModel::ObjectType>(objectListType)];
  m_toolMode = m_actionToolModeMap[action];
  action->trigger();
  /// NOT SURE
  if (m_toolMode == ePolygonMode) {
    m_createLevelWidget->ShowVertexListView();
  } else {
    m_createLevelWidget->ShowDetailListView();
  }
  ConnectToolsActions();
}

void CreateLevelController::UpdateSelection() {
  /// NOT CALLED?
  qDebug() << "UPDATE SELECTION";
  for (auto item: m_createLevelWidget->GetGraphicsItemsList()) {
    if (item->flags().testFlag(QGraphicsItem::ItemIsSelectable)) {
      auto objectItem = static_cast<GraphicsObjectItem*>(item);
      objectItem->isSelected()?
        objectItem->SetState(GraphicsObjectItem::eSelectedState):
        objectItem->SetState(GraphicsObjectItem::eDisabledState);
    }
  }
}

void CreateLevelController::CopyItem() {
  auto currentIndex = m_createLevelWidget->GetCurrentIndex();
  if (m_objectsListModel->IsPolygonIndex(currentIndex) || m_objectsListModel->IsObjectIndex(currentIndex)) {
    m_clipboardIndex = currentIndex;
  }
}

void CreateLevelController::PasteItem() {
  if (m_clipboardIndex.isValid()) {
    if (auto object = m_objectsListModel->FindObjectFromIndex(m_clipboardIndex); object) {
      switch (object->GetObjectType()) {
      case Object::eTape:{
        m_toolMode = eTapeMode;
        CreateObject(new Tape(*static_cast<Tape*>(object)));
        break;
      } case Object::eMirror:{
        m_toolMode = eMirrorMode;
        CreateObject(new Mirror(*static_cast<Mirror*>(object)));
        break;
      } case Object::eOneWay:{
        m_toolMode = eOneWayMode;
        CreateObject(new OneWay(*static_cast<OneWay*>(object)));
        break;
      } case Object::ePortal:{
        m_toolMode = ePortalMode;
        CreateObject(new Portal(*static_cast<Portal*>(object)));
        break;
      }default:
        break;
      }
    } else if (auto polygon = m_objectsListModel->FindPolygonFromIndex(m_clipboardIndex); polygon) {
      CreatePolygon(*polygon);
    }
  }
}

void CreateLevelController::UpdateClipboardIndex(QModelIndex const& p_parent, int p_first, int p_last) {
  for (int row = p_first; row <= p_last; ++row) {
    if (m_objectsListModel->index(row, 0, p_parent) == m_clipboardIndex) {
      m_clipboardIndex = QModelIndex();
      return;
    }
  }
}

void CreateLevelController::UpdateCurrentVertex(int p_currentVertex) {
  auto graphicsItem = static_cast<GraphicsPolygonItem*>(m_objectsListModel->GetGraphicsFromIndex(m_createLevelWidget->GetCurrentIndex()));
  graphicsItem->SetCurrentVertexRow(p_currentVertex);
  m_createLevelWidget->UpdateView();
}

void CreateLevelController::ChangeCurrentTool() {
  auto action = qobject_cast<QAction*>(sender());
  m_toolMode = m_actionToolModeMap[action];
  DisableObjectItems();
  if (action == m_polygonAction) {
    m_creatingNewPolygon = true;
    m_createLevelWidget->ShowVertexListView();
  } else if (action != m_selectAction) {
    m_createLevelWidget->ShowDetailListView();
  }
  /// NOT WORKING ON SELECTION TOOL
  /// m_createLevelWidget->SetCurrentObjectOrPolygonIndex(m_objectsListModel->index(m_toolMode-1, 0));
}

void CreateLevelController::ConnectToolsActions() {
  for (auto action: m_objectTypeAction) {
    connect(action, &QAction::triggered, this, &CreateLevelController::ChangeCurrentTool);
  }
}

void CreateLevelController::DisconnectToolsActions() {
  for (auto action: m_objectTypeAction) {
    disconnect(action, &QAction::triggered, this, &CreateLevelController::ChangeCurrentTool);
  }
}

void CreateLevelController::CreatePolygon(ppxl::Polygon const& p_polygon) {
  auto polygon = new ppxl::Polygon(p_polygon);
  auto polygonGraphicsItem = new GraphicsPolygonItem(polygon);
  auto polygonItem = m_objectsListModel->AddPolygon(polygon, polygonGraphicsItem);

  m_createLevelWidget->AddGraphicsItem(polygonGraphicsItem);
  polygonGraphicsItem->SetState(GraphicsObjectItem::eSelectedState);
  connect(polygonGraphicsItem, &GraphicsObjectItem::StateChanged, m_createLevelWidget, &CreateLevelWidget::UpdateView);
  m_createLevelWidget->SetCurrentObjectOrPolygonIndex(polygonItem->index());
  m_createLevelWidget->ShowVertexListView();
  m_vertexListModel->SetPolygon(polygon);
}

void CreateLevelController::InsertVertex(const QPoint& p_pos) {
  auto currentPolygonIndex = m_createLevelWidget->GetCurrentPolygonIndex();
  auto currentVertexIndex = m_createLevelWidget->FindCurrentVertexIndex();
  auto currentGraphicsItem = m_objectsListModel->GetGraphicsFromIndex(currentPolygonIndex);
  m_objectsListModel->InsertVertex(currentPolygonIndex.row(), currentVertexIndex.row()+1, ppxl::Point(p_pos.x(), p_pos.y()));
  auto polygon = m_objectsListModel->GetPolygonFromIndex(currentPolygonIndex);
  m_vertexListModel->SetPolygon(polygon);
  m_createLevelWidget->SetCurrentVertexIndex(currentVertexIndex.row()+1);
  currentGraphicsItem->ComputeBoundingPolygon();
}

void CreateLevelController::TranslatePolygon(ppxl::Vector const& p_direction) {
  auto currentPolygonIndex = m_createLevelWidget->GetCurrentPolygonIndex();
  m_objectsListModel->TranslatePolygon(currentPolygonIndex.row(), p_direction);
  m_vertexListModel->Update();
}

void CreateLevelController::MoveCurrentVertex(ppxl::Point const& p_pos) {
  MoveVertexAt(m_createLevelWidget->FindCurrentVertexIndex().row(), p_pos);
}

void CreateLevelController::MoveVertexAt(int p_vertexIndex, ppxl::Point const& p_pos) {
  auto currentPolygonIndex = m_createLevelWidget->GetCurrentPolygonIndex();
  m_objectsListModel->SetVertex(currentPolygonIndex.row(), p_vertexIndex, p_pos);
  m_vertexListModel->Update();
}

void CreateLevelController::RemoveCurrentVertex() {
  auto currentVertexIndex = m_createLevelWidget->GetCurrentVertexIndex();
  auto currentPolygonIndex = m_createLevelWidget->GetCurrentPolygonIndex();
  auto currentGraphicsItem = m_objectsListModel->GetGraphicsFromIndex(currentPolygonIndex);
  m_objectsListModel->RemoveVertex(currentPolygonIndex.row(), currentVertexIndex.row());
  auto polygon = m_objectsListModel->GetPolygonFromIndex(currentPolygonIndex);
  m_vertexListModel->SetPolygon(polygon);
  m_createLevelWidget->SetCurrentVertexIndex(std::max(0, currentVertexIndex.row()-1));
  currentGraphicsItem->ComputeBoundingPolygon();
}

void CreateLevelController::UpdateView() {
  m_createLevelWidget->UpdateView();
}

void CreateLevelController::PolygonComplete() {
  auto currentPolygonIndex = m_createLevelWidget->FindCurrentPolygonIndex();
  auto currentGraphicsPolygon = m_objectsListModel->GetGraphicsFromIndex(currentPolygonIndex);
  if (currentGraphicsPolygon) {
    currentGraphicsPolygon->SetState(GraphicsObjectItem::eDisabledState);
    m_createLevelWidget->SetCurrentObjectOrPolygonIndex(currentPolygonIndex.parent());
  }
  m_creatingNewPolygon = true;
}

void CreateLevelController::DeleteCurrent(bool p_shiftPressed) {
  disconnect(m_createLevelWidget, &CreateLevelWidget::CurrentObjectIndexChanged, this, &CreateLevelController::UpdateGraphicsSelection);

  auto currentIndex = m_createLevelWidget->GetCurrentIndex();
  auto parentIndex = currentIndex.parent();
  if (m_objectsListModel->IsPolygonIndex(currentIndex)) {
    if (p_shiftPressed) {
      delete m_objectsListModel->GetPolygonFromIndex(currentIndex);
      m_vertexListModel->ClearPolygon();
      m_creatingNewPolygon = true;
    } else {
      RemoveCurrentVertex();
      connect(m_createLevelWidget, &CreateLevelWidget::CurrentObjectIndexChanged, this, &CreateLevelController::UpdateGraphicsSelection);
      return;
    }
  } else if (m_objectsListModel->IsObjectIndex(currentIndex)) {
    delete m_objectsListModel->GetObjectFromIndex(currentIndex);
    m_objectsDetailModel->ClearObject();
  } else {
    return;
  }
  auto graphicsItem = m_objectsListModel->GetGraphicsFromIndex(currentIndex);
  m_createLevelWidget->RemoveGraphicsItem(graphicsItem);
  delete graphicsItem;
  m_objectsListModel->removeRow(currentIndex.row(), parentIndex);

  connect(m_createLevelWidget, &CreateLevelWidget::CurrentObjectIndexChanged, this, &CreateLevelController::UpdateGraphicsSelection);

  m_createLevelWidget->SetCurrentObjectOrPolygonIndex(parentIndex);
}

void CreateLevelController::MoveCurrent(ppxl::Vector const& p_direction, bool p_shiftPressed) {
  auto direction = p_direction;
  if (p_shiftPressed) {
    direction *= 10;
  }

  if (m_createLevelWidget->FindCurrentObjectIndex().isValid()) {
    TranslateObject(direction);
  } else if (m_createLevelWidget->FindCurrentPolygonIndex().isValid()) {
    TranslatePolygon(direction);
  }
}

void CreateLevelController::MoveCurrentLeft(bool p_shiftPressed) {
  MoveCurrent({-1, 0}, p_shiftPressed);
}

void CreateLevelController::MoveCurrentUp(bool p_shiftPressed) {
  MoveCurrent({0, -1}, p_shiftPressed);
}

void CreateLevelController::MoveCurrentRight(bool p_shiftPressed) {
  MoveCurrent({1, 0}, p_shiftPressed);
}

void CreateLevelController::MoveCurrentDown(bool p_shiftPressed) {
  MoveCurrent({0, 1}, p_shiftPressed);
}
