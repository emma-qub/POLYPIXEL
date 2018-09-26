#include "CreateLevelScribblingView.hxx"

#include "GUI/Models/CreateLevelModel.hxx"

#include <QPainter>
#include <QMouseEvent>
#include <QItemSelectionModel>
#include <QFontMetrics>
#include <QAction>



#include <QDebug>




const int CreateLevelScribblingView::PEN_WIDTH = 3;
const QColor CreateLevelScribblingView::NOT_SELECTED_COLOR = QColor("#CCCCCC");

CreateLevelScribblingView::CreateLevelScribblingView(QWidget* p_parent):
  AbstractScribblingView(p_parent),
  m_model(nullptr),
  m_selectionModel(nullptr),
  m_isStuck(false),
  m_nextToVertex(false),
  m_nextToBarycenter(false),
  m_movingVertex(false),
  m_movingPolygon(false),
  m_lengthOn(true),
  m_angleOn(false),
  m_currPolygonRow(-1),
  m_currVertexRow(-1),
  m_beforeMovingVertexX(-1),
  m_beforeMovingVertexY(-1),
  m_beforeMovingPolygonX(-1),
  m_beforeMovingPolygonY(-1),
  m_currOldX(-1),
  m_currOldY(-1),
  m_startShiftX(0),
  m_startShiftY(0){

  auto addPolygonAction = new QAction("New polygon", this);
  addPolygonAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
  connect(addPolygonAction, &QAction::triggered, this, &CreateLevelScribblingView::InsertPolygon);
  addAction(addPolygonAction);

  auto m_removeAction = new QAction("Remove", this);
  m_removeAction->setShortcut(QKeySequence(Qt::Key_Delete));
  connect(m_removeAction, &QAction::triggered, this, &CreateLevelScribblingView::Remove);
  addAction(m_removeAction);

  auto snapToGridAction = new QAction("Snap to grid", this);
  snapToGridAction->setShortcut(QKeySequence("CTRL+I"));
  connect(snapToGridAction, &QAction::triggered, this, &CreateLevelScribblingView::SnappedToGrid);
  addAction(snapToGridAction);

  setPenWidth(PEN_WIDTH);

  setMouseTracking(true);
}

CreateLevelScribblingView::~CreateLevelScribblingView() = default;

void CreateLevelScribblingView::SetModel(PolygonModel* p_model) {
  AbstractScribblingView::SetModel(p_model);
  m_model = static_cast<CreateLevelModel*>(p_model);
}

void CreateLevelScribblingView::SetSelectionModel(QItemSelectionModel* p_selectionModel) {
  m_selectionModel = p_selectionModel;
  m_selectionModel->setCurrentIndex(m_model->index(0, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

  connect(m_selectionModel, &QItemSelectionModel::currentChanged, this, [this] () {
    Q_EMIT(PolygonSelected());
  });
}

void CreateLevelScribblingView::DrawGrid() {
  QPainter painter(&GetImage());

  int margin = 0;
  int xMin = margin, xMax = width()-margin;
  int yMin = margin, yMax = height()-margin;
  int subSubCaseSize = 10;
  painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
  for (int i = xMin; i <= xMax; i += subSubCaseSize) {
    painter.drawLine(QPoint(i, yMin), QPoint(i, yMax));
  }
  for (int j = yMin; j <= yMax; j += subSubCaseSize) {
    painter.drawLine(QPoint(xMin, j), QPoint(xMax, j));
  }

  int subCaseSize = 50;
  painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
  for (int i = xMin; i <= xMax; i += subCaseSize) {
    painter.drawLine(QPoint(i, yMin), QPoint(i, yMax));
  }
  for (int j = yMin; j <= yMax; j += subCaseSize) {
    painter.drawLine(QPoint(xMin, j), QPoint(xMax, j));
  }

  int caseSize = 100;
  painter.setPen(QPen(Qt::darkGray, 1));
  for (int i = xMin; i <= xMax; i += caseSize) {
    painter.drawLine(QPoint(i, yMin), QPoint(i, yMax));
  }
  for (int j = yMin; j <= yMax; j += caseSize) {
    painter.drawLine(QPoint(xMin, j), QPoint(xMax, j));
  }
}

ppxl::Polygon* CreateLevelScribblingView::GetCurrentPolygon() const {
  ppxl::Polygon* currentPolygon = nullptr;

  auto currentIndex = m_selectionModel->currentIndex();
  currentIndex = currentIndex.sibling(currentIndex.row(), 0);

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
    currentPolygon = currentIndex.data(PolygonModel::ePolygonRole).value<ppxl::Polygon*>();
  }

  return currentPolygon;
}

void CreateLevelScribblingView::DrawFromModel() {
  DrawGrid();
  if (!m_model)
  {
    return;
  }

  DrawPolygonsFromModel();
}

void CreateLevelScribblingView::DrawFromCore() {
  DrawGrid();
  DrawPolygonsFromCore();
}


void CreateLevelScribblingView::DrawPolygonsFromModel() {
  auto polygonsItem = m_model->GetPolygonsItem();
  if (polygonsItem->rowCount() < 1) {
    return;
  }

  ppxl::Polygon* currentPolygon = GetCurrentPolygon();
  QStandardItem* currentPolygonItem = nullptr;

  for (int rowPolygon = 0; rowPolygon < polygonsItem->rowCount(); ++rowPolygon) {
    auto polygonItem = polygonsItem->child(rowPolygon, 0);
    auto const* polygon = polygonItem->data(CreateLevelModel::ePolygonRole).value<ppxl::Polygon*>();

    if (currentPolygon == polygon) {
      currentPolygonItem = polygonItem;
      continue;
    }
    DrawPolygonFromModel(polygonItem, false);
  }
  DrawPolygonFromModel(currentPolygonItem, true);
}

void CreateLevelScribblingView::DrawPolygonsFromCore() {
  auto polygonsItem = m_model->GetPolygonsItem();
  if (polygonsItem->rowCount() < 1) {
    return;
  }

  ppxl::Polygon* currentPolygon = GetCurrentPolygon();
  QStandardItem* currentPolygonItem = nullptr;

  for (int polygonRow = 0; polygonRow < m_model->rowCount(); ++polygonRow) {
    auto polygonItem = polygonsItem->child(polygonRow, 0);
    auto polygon = polygonItem->data(CreateLevelModel::ePolygonRole).value<ppxl::Polygon*>();

    if (currentPolygon == polygon) {
      currentPolygonItem = polygonItem;
      continue;
    }
    DrawPolygonFromCore(polygonItem, false);
  }
  DrawPolygonFromCore(currentPolygonItem, true);
}

void CreateLevelScribblingView::DrawPolygonFromModel(QStandardItem* p_polygonItem, bool p_isSelectedPolygon) {
  QColor color = p_isSelectedPolygon?
    p_polygonItem->data(Qt::DecorationRole).value<QColor>():
    QColor(NOT_SELECTED_COLOR);

  auto polygon = p_polygonItem->data(CreateLevelModel::ePolygonRole).value<ppxl::Polygon*>();
  for (int row = 0; row < p_polygonItem->rowCount(); ++row) {
    auto indexA = row;
    auto vertexLetterA = p_polygonItem->child(indexA, 0)->text();
    auto xa = p_polygonItem->child(indexA, 1)->data(Qt::DisplayRole).toString().toInt();
    auto ya = p_polygonItem->child(indexA, 2)->data(Qt::DisplayRole).toString().toInt();
    ppxl::Point A(xa, ya);
    DrawPoint(QPoint(xa, ya), color);
    ppxl::Vector shiftVector;

    if (p_polygonItem->rowCount() == 1) {
      if (p_isSelectedPolygon) {
        DrawText(A, vertexLetterA, PEN_WIDTH, ppxl::Vector(-1, -1));
      }
      continue;
    }

    int indexB = (row+1)%p_polygonItem->rowCount();
    auto vertexLetterB = p_polygonItem->child(indexB, 0)->text();
    auto xb = p_polygonItem->child(indexB, 1)->data(Qt::DisplayRole).toString().toInt();
    auto yb = p_polygonItem->child(indexB, 2)->data(Qt::DisplayRole).toString().toInt();
    ppxl::Point B(xb, yb);
    auto vertexLocation = QPoint(xb, yb);
    DrawPoint(vertexLocation, color);

    if (p_polygonItem->rowCount() > 2) {
      auto barycenter = polygon->Barycenter();
      auto barycenterX = static_cast<int>(barycenter.GetX());
      auto barycenterY = static_cast<int>(barycenter.GetY());
      DrawPoint(QPoint(barycenterX, barycenterY), color);

      shiftVector = ppxl::Vector(barycenter, B);
    }
    if (p_isSelectedPolygon) {
      DrawText(B, vertexLetterB, PEN_WIDTH, shiftVector.Normalize());
    }

    DrawLine(A, B, color);
  }
}

void CreateLevelScribblingView::DrawPolygonFromCore(QStandardItem* p_polygonItem, bool p_isSelectedPolygon) {
  QColor color = p_isSelectedPolygon?
    p_polygonItem->data(Qt::DecorationRole).value<QColor>():
    QColor(NOT_SELECTED_COLOR);

  auto polygon = p_polygonItem->data(CreateLevelModel::ePolygonRole).value<ppxl::Polygon*>();
  auto vertices = polygon->GetVertices();
  for (unsigned int vertexRow = 0; vertexRow < vertices.size(); ++vertexRow) {
    auto vertexLetterA = p_polygonItem->child(static_cast<int>(vertexRow), 0)->text();
    ppxl::Point A = vertices.at(vertexRow);
    DrawPoint(QPoint(static_cast<int>(A.GetX()), static_cast<int>(A.GetY())), color);
    ppxl::Vector shiftVector;

    if (vertices.size() == 1) {
      if (p_isSelectedPolygon) {
        DrawText(A, vertexLetterA, PEN_WIDTH, ppxl::Vector(-1, -1));
      }
      continue;
    }

    unsigned int indexB = (vertexRow+1)%vertices.size();
    auto vertexLetterB = p_polygonItem->child(static_cast<int>(indexB), 0)->text();
    ppxl::Point B = vertices.at(indexB);
    DrawPoint(QPoint(static_cast<int>(B.GetX()), static_cast<int>(B.GetY())), color);

    if (p_polygonItem->rowCount() > 2) {
      auto barycenter = polygon->Barycenter();
      auto barycenterX = static_cast<int>(barycenter.GetX());
      auto barycenterY = static_cast<int>(barycenter.GetY());
      DrawPoint(QPoint(barycenterX, barycenterY), color);

      shiftVector = ppxl::Vector(barycenter, B);
    }
    if (p_isSelectedPolygon) {
      DrawText(B, vertexLetterB, PEN_WIDTH, shiftVector.Normalize());
    }

    DrawLine(A, B, color);
  }
}

void CreateLevelScribblingView::InsertPolygon() {
  auto index = m_selectionModel->currentIndex();
  index = index.sibling(index.row(), 0);
  auto itemType = index.data(PolygonModel::eItemTypeRole).value<PolygonModel::ItemType>();
  switch(itemType) {
  case (CreateLevelModel::ePolygons): {
    index = m_model->index(m_model->rowCount(m_model->index(0, 0)), 0);
    break;
  } case (CreateLevelModel::ePolygon): {
    break;
  } case (CreateLevelModel::eVertex): {
    index = index.parent();
    break;
  }
  default: {
    break;
  }
  }

  Q_EMIT(PolygonInserted(index.row()+1, ppxl::Polygon()));
}

void CreateLevelScribblingView::Remove() {
  auto index = m_selectionModel->currentIndex();
  index = index.sibling(index.row(), 0);
  auto itemType = index.data(PolygonModel::eItemTypeRole).value<PolygonModel::ItemType>();
  switch(itemType) {
  case (CreateLevelModel::ePolygon): {
    Q_EMIT(PolygonRemoved(index.row()));
    break;
  } case (CreateLevelModel::eVertex): {
    Q_EMIT(VertexRemoved(index.parent().row(), index.row()));
    break;
  }
  case (CreateLevelModel::ePolygons):
  default: {
    break;
  }
  }
}

void CreateLevelScribblingView::DrawPoint(const QPoint& p_point, const QColor& p_color) {
  QPainter painter(&GetImage());
  painter.setPen(QPen(p_color, PEN_WIDTH*3, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));
  painter.drawPoint(p_point);

  update();
}

void CreateLevelScribblingView::mousePressEvent(QMouseEvent* p_event) {
  if (p_event->buttons() == Qt::LeftButton && (m_nextToVertex || m_nextToBarycenter)) {
    if (m_nextToVertex && !m_nextToBarycenter) {
      m_movingVertex = true;
      m_startShiftX = m_beforeMovingVertexX - p_event->pos().x();
      m_startShiftY = m_beforeMovingVertexY - p_event->pos().y();
      QPoint pos = mapToGlobal(QPoint(m_beforeMovingVertexX, m_beforeMovingVertexY));
      QCursor::setPos(pos);
    } else if (m_nextToBarycenter) {
      m_movingPolygon = true;
      m_startShiftX = m_beforeMovingPolygonX - p_event->pos().x();
      m_startShiftY = m_beforeMovingPolygonX - p_event->pos().y();
      QPoint pos = mapToGlobal(QPoint(m_beforeMovingPolygonX, m_beforeMovingPolygonY));
      QCursor::setPos(pos);
    }
    setCursor(Qt::ClosedHandCursor);
  }
}

void CreateLevelScribblingView::mouseMoveEvent(QMouseEvent* p_event) {
  // Condition is wrong, must take every polygon into acount
  if (m_model->rowCount(m_model->index(0, 0)) < 1) {
    QWidget::mouseMoveEvent(p_event);
    return;
  }

  if (p_event->buttons() == Qt::NoButton && m_model->rowCount() > 0) {
    ppxl::Point currPos(p_event->pos().x(), p_event->pos().y());

    QModelIndex currIndex = m_selectionModel->currentIndex();

    if (currIndex.column() != 0) {
      currIndex = currIndex.sibling(currIndex.row(), 0);
    }

    // This is a vertex
    auto itemType = currIndex.data(CreateLevelModel::eItemTypeRole).value<CreateLevelModel::ItemType>();
    switch(itemType) {
    case (CreateLevelModel::ePolygons): {
      currIndex = m_model->index(0, 0);
      break;
    } case (CreateLevelModel::ePolygon): {
      break;
    } case (CreateLevelModel::eVertex): {
      currIndex = currIndex.parent();
      break;
    }
    default: {
      break;
    }
    }

    auto const* polygon = currIndex.data(PolygonModel::ePolygonRole).value<ppxl::Polygon*>();
    auto vertices = polygon->GetVertices();
    m_nextToVertex = false;
    m_nextToBarycenter = false;
    ppxl::Point barycenter;
    if (m_model->rowCount(currIndex) > 2) {
      barycenter = polygon->Barycenter();
      m_nextToBarycenter = (ppxl::Point::Distance(currPos, barycenter) < 20);
    }
    for (unsigned int k = 0; k < vertices.size(); ++k) {
      ppxl::Point vertex = vertices.at(k);
      if (ppxl::Point::Distance(currPos, vertex) < 10 || m_nextToBarycenter) {
        m_nextToVertex = !m_nextToBarycenter;
        m_currPolygonRow = currIndex.row();
        m_currVertexRow = static_cast<int>(k);
        m_currOldX = p_event->pos().x();
        m_currOldY = p_event->pos().y();
        m_beforeMovingVertexX = static_cast<int>(vertex.GetX());
        m_beforeMovingPolygonX = static_cast<int>(barycenter.GetX());
        m_beforeMovingVertexY = static_cast<int>(vertex.GetY());
        m_beforeMovingPolygonY = static_cast<int>(barycenter.GetY());
        break;
      }
    }
  }

  if (!m_movingPolygon && !m_movingVertex) {
    if (m_nextToVertex || m_nextToBarycenter) {
      setCursor(Qt::OpenHandCursor);
    } else {
      setCursor(Qt::ArrowCursor);
    }
  }

  if (p_event->buttons() == Qt::LeftButton && !m_nextToVertex && m_nextToBarycenter) {
    int newX = p_event->pos().x();
    int newY = p_event->pos().y();
    Q_EMIT(PolygonMoved(m_currPolygonRow, ppxl::Vector(ppxl::Point(m_currOldX, m_currOldY), ppxl::Point(newX, newY)), false));
    m_currOldX = newX;
    m_currOldY = newY;
  }

  if (p_event->buttons() == Qt::LeftButton && m_nextToVertex && !m_nextToBarycenter) {
    int newX = p_event->pos().x();
    int newY = p_event->pos().y();
    Q_EMIT(VertexMoved(m_currPolygonRow, m_currVertexRow, ppxl::Vector(ppxl::Point(m_currOldX, m_currOldY), ppxl::Point(newX, newY)), false));
    m_currOldX = newX;
    m_currOldY = newY;
  }

  if (p_event->buttons() == Qt::LeftButton && p_event->modifiers() == Qt::ControlModifier) {
    QPoint globalPosition = QCursor::pos();

    int const oldX = globalPosition.x();
    int newX = oldX;
    int const oldY = globalPosition.y();
    int newY = oldY;
    int extraX = (p_event->pos().x()+m_startShiftX)%50;
    int extraY = (p_event->pos().y()+m_startShiftY)%50;
    int delta = m_isStuck? 0: 5;

    if (extraX < delta) {
      newX -= extraX;
    } else if (extraX > 50 - delta) {
      newX += 50-extraX;
    }

    if (extraY < delta) {
      newY -= extraY;
    } else if (extraY > 50 - delta) {
      extraY += 50-extraY;
    }

    m_isStuck = (newX != oldX || newY != oldY);

    QCursor::setPos(newX, newY);
  }
}

void CreateLevelScribblingView::mouseReleaseEvent(QMouseEvent* p_event) {
  ppxl::Point point(p_event->pos().x(), p_event->pos().y());

  if (!m_movingPolygon && !m_movingVertex) {
    if (m_model->GetPolygonsCount() == 0) {
      Q_EMIT(PolygonInserted(0, ppxl::Polygon()));
      Q_EMIT(VertexInserted(0, 0, point));
    } else {
      auto index = m_selectionModel->currentIndex();
      index = index.sibling(index.row(), 0);
      int polygonRow;
      int vertexRow;
      auto itemType = index.data(CreateLevelModel::eItemTypeRole).value<CreateLevelModel::ItemType>();
      switch (itemType) {
      case (CreateLevelModel::ePolygons): {
        auto polygonsItem = m_model->GetPolygonsItem();
        auto polygonItem = polygonsItem->child(polygonsItem->rowCount(), 0);
        polygonRow = polygonItem->row();
        vertexRow = polygonItem->rowCount();
        break;
      } case (CreateLevelModel::ePolygon): {
        auto polygonItem = m_model->itemFromIndex(index);
        polygonRow = polygonItem->row();
        vertexRow = polygonItem->rowCount();
        break;
      } case (CreateLevelModel::eVertex): {
        polygonRow = index.parent().row();
        vertexRow = index.row()+1;
        break;
      } case (CreateLevelModel::eX):
        case (CreateLevelModel::eY): {
        polygonRow = index.parent().parent().row();
        vertexRow = index.parent().row()+1;
        break;
      }
      }
      Q_EMIT(VertexInserted(polygonRow, vertexRow, point));
    }
  }

  if (m_movingVertex) {
    if (m_beforeMovingVertexX != p_event->pos().x() || p_event->pos().y() != m_beforeMovingVertexY) {
      Q_EMIT(VertexMoved(m_currPolygonRow, m_currVertexRow, ppxl::Vector(ppxl::Point(p_event->pos().x(), p_event->pos().y()), ppxl::Point(m_beforeMovingVertexX, m_beforeMovingVertexY)), false));
      Q_EMIT(VertexMoved(m_currPolygonRow, m_currVertexRow, ppxl::Vector(ppxl::Point(m_beforeMovingVertexX, m_beforeMovingVertexY), ppxl::Point(p_event->pos().x(), p_event->pos().y())), true));
    }
  } else if (m_movingPolygon) {
    auto const* polygon = m_model->GetPolygonsList().at(m_currPolygonRow);

    auto barycenter = polygon->Barycenter();
    auto barycenterX = static_cast<int>(barycenter.GetX());
    auto barycenterY = static_cast<int>(barycenter.GetY());
    if (barycenterX != m_beforeMovingPolygonX || barycenterY != m_beforeMovingPolygonY) {
      Q_EMIT(PolygonMoved(m_currPolygonRow, ppxl::Vector(ppxl::Point(barycenterX, barycenterY), ppxl::Point(m_beforeMovingPolygonX, m_beforeMovingPolygonY)), false));
      Q_EMIT(PolygonMoved(m_currPolygonRow, ppxl::Vector(ppxl::Point(m_beforeMovingPolygonX, m_beforeMovingPolygonY), ppxl::Point(barycenterX, barycenterY)), true));
    }
  }

  m_movingVertex = false;
  m_movingPolygon = false;
  m_beforeMovingPolygonX = -1;
  m_beforeMovingPolygonY = -1;

  if (m_nextToVertex || m_nextToBarycenter) {
    if (m_nextToVertex && !m_nextToBarycenter) {
      m_beforeMovingVertexX = p_event->pos().x();
      m_beforeMovingVertexY = p_event->pos().y();
    } else if (m_nextToBarycenter) {
      m_beforeMovingPolygonX = p_event->pos().x();
      m_beforeMovingPolygonY = p_event->pos().y();
    }
    setCursor(Qt::OpenHandCursor);
  }
}

void CreateLevelScribblingView::paintEvent(QPaintEvent* p_event) {
  QPainter painter(this);
  QRect dirtyRect = p_event->rect();
  painter.drawImage(dirtyRect, GetImage(), dirtyRect);
}
