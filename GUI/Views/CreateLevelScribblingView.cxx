#include "CreateLevelScribblingView.hxx"

#include "Core/Objects/Object.hxx"
#include "GUI/Models/CreateLevelObjectsListModel.hxx"
#include "GUI/GraphicsItem/GraphicsObjectItem.hxx"

#include <QPainter>
#include <QMouseEvent>
#include <QItemSelectionModel>
#include <QFontMetrics>

CreateLevelScribblingView::CreateLevelScribblingView(QWidget* p_parent):
  QGraphicsView(p_parent),
  m_scene(nullptr),
  m_gridPixmapItem(nullptr),
  m_objectsListModel(nullptr),
  m_objectSelectionModel(nullptr),
  m_rectangleSelectionItem(new GraphicsRectangleSelectionItem),
  m_viewInitialized(false) {

  setMouseTracking(true);
}

CreateLevelScribblingView::~CreateLevelScribblingView() {
  delete m_rectangleSelectionItem;
}

void CreateLevelScribblingView::InitView() {
  if (m_viewInitialized) {
    return;
  }

  // Hide scrollbars
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Set scene
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);
  m_scene->setSceneRect(0, 0, width(), height());
  connect(m_scene, &QGraphicsScene::selectionChanged, this, &CreateLevelScribblingView::SelectionChanged);

  int margin = 0;
  int xMin = margin, xMax = width()-margin;
  int yMin = margin, yMax = height()-margin;
  int subSubCaseSize = 10;
  QPen dashLinePen(Qt::lightGray, 1, Qt::DashLine);
  QPen solidPen(Qt::gray, 1, Qt::SolidLine);
  QPen thickPen(Qt::darkGray, 1);

  for (int i = xMin; i <= xMax; i += subSubCaseSize) {
    scene()->addLine(QLineF(i, yMin, i, yMax), dashLinePen);
  }
  for (int j = yMin; j <= yMax; j += subSubCaseSize) {
    scene()->addLine(QLineF(xMin, j, xMax, j), dashLinePen);
  }

  int subCaseSize = 50;
  for (int i = xMin; i <= xMax; i += subCaseSize) {
    scene()->addLine(QLineF(i, yMin, i, yMax), solidPen);
  }
  for (int j = yMin; j <= yMax; j += subCaseSize) {
    scene()->addLine(QLineF(xMin, j, xMax, j), solidPen);
  }

  int caseSize = 100;
  for (int i = xMin; i <= xMax; i += caseSize) {
    scene()->addLine(QLineF(i, yMin, i, yMax), thickPen);
  }
  for (int j = yMin; j <= yMax; j += caseSize) {
    scene()->addLine(QLineF(xMin, j, xMax, j), thickPen);
  }

  auto gridPixmap = grab(sceneRect().toRect());
  scene()->clear();
  m_gridPixmapItem = scene()->addPixmap(gridPixmap);

  m_viewInitialized = true;
}

void CreateLevelScribblingView::SetObjectsListModel(CreateLevelObjectsListModel* p_objectsListModel) {
  m_objectsListModel = p_objectsListModel;
}

void CreateLevelScribblingView::SetSelectionModel(QItemSelectionModel* p_selectionModel) {
  m_objectSelectionModel = p_selectionModel;
  m_objectSelectionModel->setCurrentIndex(m_objectsListModel->index(0, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void CreateLevelScribblingView::ClearImage() {
  for (auto* item: m_scene->items()) {
    if (item != m_gridPixmapItem) {
      m_scene->removeItem(item);
      delete item;
    }
  }
}

void CreateLevelScribblingView::UpdateView() {
  m_scene->update();
}

void CreateLevelScribblingView::AddGraphicsItem(QGraphicsItem* p_graphicsItem) {
  m_scene->addItem(p_graphicsItem);
}

QList<QGraphicsItem*> CreateLevelScribblingView::GetGraphicsItemsList() const {
  return m_scene->items();
}

void CreateLevelScribblingView::RemoveGraphicsItem(QGraphicsItem* p_graphicsItem) {
  m_scene->removeItem(p_graphicsItem);
}

int CreateLevelScribblingView::GetGraphicsItemCount() const {
  return m_scene->items().count();
}

void CreateLevelScribblingView::SetRubberBandDragMode(bool p_rubberBandOn) {
  if (p_rubberBandOn) {
    setDragMode(RubberBandDrag);
    m_scene->addItem(m_rectangleSelectionItem);
  } else {
    setDragMode(NoDrag);
    m_rectangleSelectionItem->setRect(QRect());
    m_scene->removeItem(m_rectangleSelectionItem);
  }
}

void CreateLevelScribblingView::SetSelectionArea(QRect const& p_rect) {
  m_rectangleSelectionItem->setRect(p_rect);

  QPainterPath selectionPath;
  selectionPath.addRect(p_rect);

  m_scene->setSelectionArea(selectionPath);
}

void CreateLevelScribblingView::mousePressEvent(QMouseEvent* p_event) {
  Q_EMIT MousePressed(p_event);
}

void CreateLevelScribblingView::mouseMoveEvent(QMouseEvent* p_event) {
  Q_EMIT MouseMoved(p_event);
}

void CreateLevelScribblingView::mouseReleaseEvent(QMouseEvent* p_event) {
  Q_EMIT MouseReleased(p_event);
}

void CreateLevelScribblingView::keyPressEvent(QKeyEvent* p_event) {
  auto shiftPressed = p_event->modifiers().testFlag(Qt::ShiftModifier);
  switch (p_event->key()) {
  case Qt::Key_Return:{
    Q_EMIT KeyReturnPressed();
    break;
  } case Qt::Key_Left: {
    Q_EMIT KeyLeftPressed(shiftPressed);
    break;
  } case Qt::Key_Up: {
    Q_EMIT KeyUpPressed(shiftPressed);
    break;
  } case Qt::Key_Right: {
    Q_EMIT KeyRightPressed(shiftPressed);
    break;
  } case Qt::Key_Down: {
    Q_EMIT KeyDownPressed(shiftPressed);
    break;
  } default:
    break;
  }

  QGraphicsView::keyPressEvent(p_event);
}
