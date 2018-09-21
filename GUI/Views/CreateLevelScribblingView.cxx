#include "CreateLevelScribblingView.hxx"

#include "GUI/Models/CreateLevelModel.hxx"

#include <QPainter>
#include <QMouseEvent>
#include <QItemSelectionModel>
#include <QFontMetrics>



#include <QDebug>




const int CreateLevelScribblingView::PEN_WIDTH = 3;
const QColor CreateLevelScribblingView::NOT_SELECTED_COLOR = QColor("#CCCCCC");

CreateLevelScribblingView::CreateLevelScribblingView(QWidget* p_parent):
  AbstractScribblingView(p_parent),
  m_model(nullptr),
  m_selectionModel(nullptr),
  m_isMagnetic(false),
  m_isStuck(false),
  m_nearToVertex(false),
  m_nearToBarycenter(false),
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
  m_currOldY(-1) {

  setPenWidth(PEN_WIDTH);

  setMouseTracking(true);
}

CreateLevelScribblingView::~CreateLevelScribblingView() = default;

void CreateLevelScribblingView::SetModel(AbstractLevelModel* p_model) {
  AbstractScribblingView::SetModel(p_model);
  m_model = static_cast<CreateLevelModel*>(p_model);
}

void CreateLevelScribblingView::SetSelectionModel(QItemSelectionModel* p_selectionModel) {
  m_selectionModel = p_selectionModel;
}

void CreateLevelScribblingView::DrawPoint(const QPoint& p_point, const QColor& p_color) {
  QPainter painter(&GetImage());
  painter.setPen(QPen(p_color, PEN_WIDTH*3, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));
  painter.drawPoint(p_point);

  update();
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

void CreateLevelScribblingView::DrawFromModel() {
  DrawGrid();
  if (!m_model)
  {
    return;
  }

  // Draw every polygon in model color
  auto polygonsItem = m_model->GetPolygonsItem();
  for (int row = 0; row < polygonsItem->rowCount(); ++row) {
    auto polygonItem = polygonsItem->child(row, 0);
    auto color = polygonItem->data(Qt::DecorationRole).value<QColor>();
    auto const* polygon = polygonItem->data(CreateLevelModel::ePolygonRole).value<ppxl::Polygon*>();

    for (int row = 0; row < polygonItem->rowCount(); ++row) {
      auto indexA = row;
      auto vertexLetterA = polygonItem->child(indexA, 0)->text();
      auto xa = polygonItem->child(indexA, 1)->data(Qt::DisplayRole).toString().toInt();
      auto ya = polygonItem->child(indexA, 2)->data(Qt::DisplayRole).toString().toInt();
      ppxl::Point A(xa, ya);
      DrawPoint(QPoint(xa, ya), color);
      ppxl::Vector shiftVector;

      if (polygonItem->rowCount() == 1) {
        DrawText(A, vertexLetterA, PEN_WIDTH, ppxl::Vector(-1, -1));
        return;
      }

      int indexB = (row+1)%polygonItem->rowCount();
      auto vertexLetterB = polygonItem->child(indexB, 0)->text();
      auto xb = polygonItem->child(indexB, 1)->data(Qt::DisplayRole).toString().toInt();
      auto yb = polygonItem->child(indexB, 2)->data(Qt::DisplayRole).toString().toInt();
      ppxl::Point B(xb, yb);
      auto vertexLocation = QPoint(xb, yb);
      DrawPoint(vertexLocation, color);

      if (polygonItem->rowCount() > 2) {
        auto barycenter = polygon->Barycenter();
        auto barycenterX = static_cast<int>(barycenter.GetX());
        auto barycenterY = static_cast<int>(barycenter.GetY());
        DrawPoint(QPoint(barycenterX, barycenterY), color);

        shiftVector = ppxl::Vector(barycenter, B);
      }
      DrawText(B, vertexLetterB, PEN_WIDTH, shiftVector.Normalize());

      DrawLine(A, B, color);
    }
  }
}

void CreateLevelScribblingView::mousePressEvent(QMouseEvent* p_event) {
  if (p_event->buttons() == Qt::LeftButton && (m_nearToVertex || m_nearToBarycenter)) {
    if (m_nearToVertex && !m_nearToBarycenter) {
      QPoint pos = mapToGlobal(QPoint(m_beforeMovingVertexX, m_beforeMovingVertexY));
      QCursor::setPos(pos);
    } else if (m_nearToBarycenter) {
      QPoint pos = mapToGlobal(QPoint(m_beforeMovingPolygonX, m_beforeMovingPolygonY));
      QCursor::setPos(pos);
    }
    setCursor(Qt::ClosedHandCursor);
  }
}

void CreateLevelScribblingView::mouseMoveEvent(QMouseEvent* p_event) {
  m_movingVertex = (p_event->buttons() == Qt::LeftButton && m_nearToVertex);
  m_movingPolygon = (p_event->buttons() == Qt::LeftButton && m_nearToBarycenter);

  if (m_model->rowCount(m_model->index(0, 0)) < 1) return;

  if (p_event->buttons() == Qt::NoButton && m_model->rowCount() > 0) {
    ppxl::Point currPos(p_event->pos().x(), p_event->pos().y());

    QModelIndex currIndex = m_selectionModel->currentIndex();
    if (!currIndex.isValid()) {
      // Initialize
      m_selectionModel->setCurrentIndex(m_model->index(0, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
      currIndex = m_selectionModel->currentIndex();
    }

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

    auto const* polygon = currIndex.data(AbstractLevelModel::ePolygonRole).value<ppxl::Polygon*>();
    auto vertices = polygon->GetVertices();
    m_nearToVertex = false;
    m_nearToBarycenter = false;
    ppxl::Point barycenter;
    if (m_model->rowCount(currIndex) > 2) {
      barycenter = polygon->Barycenter();
      m_nearToBarycenter = (ppxl::Point::Distance(currPos, barycenter) < 20);
    }
    for (unsigned int k = 0; k < vertices.size(); ++k) {
      ppxl::Point vertex = vertices.at(k);
      if (ppxl::Point::Distance(currPos, vertex) < 10 || m_nearToBarycenter) {
        m_nearToVertex = !m_nearToBarycenter;
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
    if (m_nearToVertex || m_nearToBarycenter) {
      setCursor(Qt::OpenHandCursor);
    } else {
      setCursor(Qt::ArrowCursor);
    }
  }

  if (p_event->buttons() == Qt::LeftButton && !m_nearToVertex && m_nearToBarycenter) {
    int newX = p_event->pos().x();
    int newY = p_event->pos().y();
    Q_EMIT(PolygonMoved(m_currPolygonRow, ppxl::Vector(ppxl::Point(m_currOldX, m_currOldY), ppxl::Point(newX, newY)), false));
    m_currOldX = newX;
    m_currOldY = newY;
  }

  if (p_event->buttons() == Qt::LeftButton && m_nearToVertex && !m_nearToBarycenter) {
    int newX = p_event->pos().x();
    int newY = p_event->pos().y();
    Q_EMIT(VertexMoved(m_currPolygonRow, m_currVertexRow, ppxl::Vector(ppxl::Point(m_currOldX, m_currOldY), ppxl::Point(newX, newY)), false));
    m_currOldX = newX;
    m_currOldY = newY;
  }

//  if (p_event->buttons() == Qt::LeftButton && m_isMagnetic) {
//    QPoint globalPosition = QCursor::pos();

//    const int oldX = globalPosition.x();
//    int newX = oldX;
//    const int oldY = globalPosition.y();
//    int newY = oldY;

//    int extraX = p_event->pos().x()%50;
//    int extraY = p_event->pos().y()%50;

//    int delta = 0;
//    if (!m_isStuck) {
//      delta = 3;
//    }

//    if (extraX < delta) {
//      newX -= extraX - 5;
//    } else if (extraX > 50-delta) {
//      newX += 4 + 50-extraX;
//    }

//    if (extraY < delta) {
//      newY -= extraY - 5;
//    } else if (extraY > 50-delta) {
//      extraY += 4 + 50-extraY;
//    }

//    m_isStuck = (newX != oldX || newY != oldY);

//    QCursor::setPos(newX, newY);
//  }
}

void CreateLevelScribblingView::mouseReleaseEvent(QMouseEvent* p_event) {
  ppxl::Point point(p_event->pos().x(), p_event->pos().y());

  if (m_nearToVertex || m_nearToBarycenter) {
    if (m_nearToVertex && !m_nearToBarycenter) {
      m_beforeMovingVertexX = p_event->pos().x();
      m_beforeMovingVertexY = p_event->pos().y();
    } else if (m_nearToBarycenter) {
      m_beforeMovingPolygonX = p_event->pos().x();
      m_beforeMovingPolygonY = p_event->pos().y();
    }
    setCursor(Qt::OpenHandCursor);
  }

  if (!m_movingPolygon && !m_movingVertex) {
    if (m_model->GetPolygonsCount() == 0) {
      Q_EMIT(PolygonInserted(0, ppxl::Polygon()));
      Q_EMIT(VertexInserted(0, 0, point));
      auto vertexIndex = m_model->GetVertexIndex(0, 0);
      m_selectionModel->setCurrentIndex(vertexIndex, QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
    } else {
      auto index = m_selectionModel->currentIndex();
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
      auto vertexIndex = m_model->GetVertexIndex(polygonRow, vertexRow);
      m_selectionModel->setCurrentIndex(vertexIndex, QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
    }
  }
//  QModelIndex polygonIndex;
//  QModelIndex vertexIndex;
//  int vertexRow = -1;

//  QModelIndex currIndex = m_selectionModel->currentIndex();
//  if (!currIndex.isValid()) {
//    qDebug() << "right now";
//    return;
//  }

//  if (currIndex.column() != 0) {
//    currIndex = m_model->index(currIndex.row(), 0, currIndex.parent());
//  }

//  QString fstData = currIndex.data(Qt::DisplayRole).toString();

//  // This is a vertex
//  if (!fstData.startsWith("Polygon")) {
//    vertexIndex = currIndex;
//    polygonIndex = currIndex.parent();
//    vertexRow = vertexIndex.row()+1;
//    // This is the header
//  } else if (fstData == "Polygon") {
//    qDebug() << "There";
//    ///m_controller->addPolygon(m_model->rowCount(), ppxl::Polygon());
//    polygonIndex = m_model->index(m_model->rowCount()-1, 0);
//    vertexRow = 0;
//    // This is a polygon
//  } else if (fstData.startsWith("Polygon ")) {
//    polygonIndex = currIndex;
//    vertexRow = m_model->rowCount(polygonIndex);
//  } else {
//    qDebug() << "###" << fstData;
//    return;
//  }

//  if (m_movingVertex) {
//    if (m_beforeMovingVertexX != p_event->pos().x() || p_event->pos().y() != m_beforeMovingVertexY) {
//      Q_EMIT(VertexMoved(m_currPolygonRow, m_currVertexRow, ppxl::Vector(ppxl::Point(m_beforeMovingVertexX, m_beforeMovingVertexY), ppxl::Point(p_event->pos().x(), p_event->pos().y())), true));
//    }
//    m_beforeMovingVertexX = -1;
//    m_beforeMovingVertexY = -1;
//    return;
//  } else if (m_movingPolygon) {
//    auto const* polygon = m_model->GetPolygonsItemList().at(m_currPolygonRow)->data(AbstractLevelModel::ePolygonRole).value<ppxl::Polygon*>();
//    //        qDebug() << "Compute polygon barycenter from LevelDesignerScribbleView::mouseReleaseEvent";
//    ppxl::Point barycenter = polygon.Barycenter();
//    if (barycenter.GetX() != m_beforeMovingPolygonX || barycenter.GetY() != m_beforeMovingPolygonY) {
//      Q_EMIT(PolygonMoved(m_currPolygonRow, ppxl::Vector(ppxl::Point(barycenter.GetX(), barycenter.GetY()), ppxl::Point(m_beforeMovingPolygonX, m_beforeMovingPolygonY)), false));
//      Q_EMIT(PolygonMoved(m_currPolygonRow, ppxl::Vector(ppxl::Point(m_beforeMovingPolygonX, m_beforeMovingPolygonY), ppxl::Point(barycenter.GetX(), barycenter.GetY())), true));
//    }
//    m_beforeMovingPolygonX = -1;
//    m_beforeMovingPolygonY = -1;
//    return;
//  } else {
//    Q_EMIT(VertexInserted(m_currPolygonRow, m_currVertexRow, ppxl::Point(p_event->pos().x(), p_event->pos().y())));
//  }
}

void CreateLevelScribblingView::paintEvent(QPaintEvent* p_event) {
  QPainter painter(this);
  QRect dirtyRect = p_event->rect();
  painter.drawImage(dirtyRect, GetImage(), dirtyRect);
}
