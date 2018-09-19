#include "CreateLevelScribblingView.hxx"

#include "GUI/Models/CreateLevelModel.hxx"

#include <QPainter>
#include <QMouseEvent>
#include <QItemSelectionModel>

const int CreateLevelScribblingView::PEN_WIDTH = 7;
const QColor CreateLevelScribblingView::NOT_SELECTED_COLOR = QColor("#CCCCCC");

CreateLevelScribblingView::CreateLevelScribblingView(QWidget* p_parent):
  AbstractScribblingView(p_parent),
  m_model(static_cast<CreateLevelModel*>(GetModel())),
  m_selectionModel(nullptr) {
}

CreateLevelScribblingView::~CreateLevelScribblingView() = default;

void CreateLevelScribblingView::SetModel(AbstractLevelModel* p_model) {
  AbstractScribblingView::SetModel(p_model);
  m_selectionModel->setModel(p_model);
}

void CreateLevelScribblingView::DrawGrid() {
  QPainter painter(&GetImage());

  int margin = 10;
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
  AbstractScribblingView::DrawFromModel();
}

void CreateLevelScribblingView::mousePressEvent(QMouseEvent* p_event) {
  if (p_event->buttons() == Qt::LeftButton && m_nearToVertex) {
    QPoint pos = mapToGlobal(QPoint(m_beforeMovingVertexX, m_beforeMovingVertexY));
    QCursor::setPos(pos);
    setCursor(Qt::ClosedHandCursor);
  }
}

void CreateLevelScribblingView::mouseMoveEvent(QMouseEvent* p_event) {
  m_movingVertex = (p_event->buttons() == Qt::LeftButton && m_nearToVertex);
  m_movingPolygon = (p_event->buttons() == Qt::LeftButton && m_nearToBarycenter);

  if (p_event->buttons() == Qt::NoButton && m_model->rowCount() > 0) {
    ppxl::Point currPos(p_event->pos().x(), p_event->pos().y());

    QModelIndex currIndex = m_selectionModel->currentIndex();
    if (!currIndex.isValid()) {
      qDebug() << "now";
      m_selectionModel->setCurrentIndex(m_model->index(0, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
      currIndex = m_selectionModel->currentIndex();
    }

    if (currIndex.column() != 0)
      currIndex = m_model->index(currIndex.row(), 0, currIndex.parent());

    QString fstData = currIndex.data().toString();

    // This is a vertex
    if (!fstData.startsWith("Polygon")) {
      currIndex = currIndex.parent();
      // This is the header
    } else if (fstData == "Polygon") {
      currIndex = m_model->index(0, 0);
      // This is a polygon
    } else if (fstData.startsWith("Polygon ")) {
    } else {
      qDebug() << currIndex << fstData;
      return;
    }

    auto polygon = currIndex.data(AbstractLevelModel::ePolygonRole).value<ppxl::Polygon>();
    auto vertices = polygon.GetVertices();
    m_nearToVertex = false;
    m_nearToBarycenter = false;
    ppxl::Point barycenter;
    if (m_model->rowCount(currIndex) > 2) {
      //qDebug() << "Compute polygon barycenter from LevelDesignerScribbleView::mouseMoveEvent";
      barycenter = polygon.Barycenter();
      m_nearToBarycenter = (ppxl::Point::Distance(currPos, barycenter) < 20);
    }
    for (unsigned int k = 0; k < vertices.size(); ++k) {
      ppxl::Point vertex = vertices.at(k);
      if (ppxl::Point::Distance(currPos, vertex) < 10 || m_nearToBarycenter) {
        m_nearToVertex = !m_nearToBarycenter;
        m_currPolygonRow = currIndex.row();
        m_currVertexRow = k;
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
  if (m_nearToVertex || m_nearToBarycenter) {
    setCursor(Qt::OpenHandCursor);
  } else {
    setCursor(Qt::ArrowCursor);
  }

  if (p_event->buttons() == Qt::LeftButton && !m_nearToVertex && m_nearToBarycenter) {
    int newX = p_event->pos().x();
    int newY = p_event->pos().y();
    ///Q_EMIT(PolygonMoved(m_currPolygonRow, ppxl::Point(m_currOldX, m_currOldY), ppxl::Point(newX, newY), false));
    m_currOldX = newX;
    m_currOldY = newY;
  }

  if (p_event->buttons() == Qt::LeftButton && m_nearToVertex && !m_nearToBarycenter) {
    int newX = p_event->pos().x();
    int newY = p_event->pos().y();
    ///Q_EMIT(VertexMoved(m_currPolygonRow, m_currVertexRow, m_currOldX, m_currOldY, newX, newY, false));
    m_currOldX = newX;
    m_currOldY = newY;
  }

  if (p_event->buttons() == Qt::LeftButton && m_isMagnetic) {
    QPoint globalPosition = QCursor::pos();

    const int oldX = globalPosition.x();
    int newX = oldX;
    const int oldY = globalPosition.y();
    int newY = oldY;

    int extraX = p_event->pos().x()%50;
    int extraY = p_event->pos().y()%50;

    int delta = 0;
    if (!m_isStuck) {
      delta = 3;
    }

    if (extraX < delta) {
      newX -= extraX - 5;
    } else if (extraX > 50-delta) {
      newX += 4 + 50-extraX;
    }

    if (extraY < delta) {
      newY -= extraY - 5;
    } else if (extraY > 50-delta) {
      extraY += 4 + 50-extraY;
    }

    m_isStuck = (newX != oldX || newY != oldY);

    QCursor::setPos(newX, newY);
  }
}

void CreateLevelScribblingView::mouseReleaseEvent(QMouseEvent* p_event) {
//  if (m_model->rowCount() < 1)
//    return;

//  if (m_nearToVertex || m_nearToBarycenter)
//    setCursor(Qt::OpenHandCursor);

//  QModelIndex polygonIndex;
//  QModelIndex vertexIndex;
//  int vertexRow = -1;

//  QModelIndex currIndex = m_selectionModel->currentIndex();
//  if (!currIndex.isValid()) {
//    qDebug() << "right now";
//    return;
//  }

//  if (currIndex.column() != 0)
//    currIndex = m_model->index(currIndex.row(), 0, currIndex.parent());

//  QString fstData = m_model->itemData(currIndex).value(Qt::DisplayRole).toString();

//  // This is a vertex
//  if (!fstData.startsWith("Polygon")) {
//    vertexIndex = currIndex;
//    polygonIndex = currIndex.parent();
//    vertexRow = vertexIndex.row()+1;
//    // This is the header
//  } else if (fstData == "Polygon") {
//    qDebug() << "There";
//    m_controller->addPolygon(m_model->rowCount(), poc::Polygon());
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
//    if (m_beforeMovingVertexX != event->pos().x() || event->pos().y() != m_beforeMovingVertexY) {
//      m_controller->moveVertex(m_currPolygonRow, m_currVertexRow, m_beforeMovingVertexX, m_beforeMovingVertexY, event->pos().x(), event->pos().y());
//    }
//    m_beforeMovingVertexX = -1;
//    m_beforeMovingVertexY = -1;
//    return;
//  } else if (m_movingPolygon) {
//    poc::Polygon polygon = m_model->polygonFromIndex(polygonIndex);
//    //        qDebug() << "Compute polygon barycenter from LevelDesignerScribbleView::mouseReleaseEvent";
//    poc::Point barycenter = polygon.barycenter();
//    if (barycenter.getX() != m_beforeMovingPolygonX || barycenter.getY() != m_beforeMovingPolygonY) {
//      m_controller->movePolygon(m_currPolygonRow, barycenter.getX(), barycenter.getY(), m_beforeMovingPolygonX, m_beforeMovingPolygonY, false);
//      m_controller->movePolygon(m_currPolygonRow, m_beforeMovingPolygonX, m_beforeMovingPolygonY, barycenter.getX(), barycenter.getY());
//    }
//    m_beforeMovingPolygonX = -1;
//    m_beforeMovingPolygonY = -1;
//    return;
//  } else {
//    m_controller->addVertex(polygonIndex.row(), vertexRow, poc::Point(event->pos().x(), event->pos().y()));
//  }
}

void CreateLevelScribblingView::paintEvent(QPaintEvent* p_event) {
  QPainter painter(this);
  QRect dirtyRect = p_event->rect();
  painter.drawImage(dirtyRect, GetImage(), dirtyRect);
}
