#include "GameView.hxx"

#include "GUI/Models/GameModel.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ScriblingView
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ScriblingView::ScriblingView(QWidget* p_parent):
  QWidget(p_parent),
  m_myPenWidth(1),
  m_scribbling(false) {
}

ScriblingView::~ScriblingView() = default;

void ScriblingView::setModel(GameModel* p_gameModel) {
  m_gameModel = p_gameModel;
}

void ScriblingView::drawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  ppxl::Point startPoint(p_line.GetA());
  ppxl::Point endPoint(p_line.GetB());
  drawLine(startPoint, endPoint, p_color, p_penStyle);
}

void ScriblingView::drawFromModel() {
  clearImage();

  // Draw every polygon in model color
  auto polygonItems = m_gameModel->GetPolygonItemsList();
  for (auto polygonItem: polygonItems) {
    auto polygon = polygonItem->data(GameModel::ePolygonRole).value<ppxl::Polygon>();
    auto color = polygonItem->data(Qt::DecorationRole).value<QColor>();

    auto vertices = polygon.GetVertices();
    for (unsigned k = 0; k < vertices.size(); k++) {
      ppxl::Point A(vertices.at(k));
      ppxl::Point B(vertices.at((k+1)%vertices.size()));

      drawLine(A, B, color);
    }
  }
}

void ScriblingView::clearImage() {
  m_image.fill(Qt::white);
}

void ScriblingView::drawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle) {
  QPoint startPoint(static_cast<int>(p_startPoint.GetX()), static_cast<int>(p_startPoint.GetY()));
  QPoint endPoint(static_cast<int>(p_endPoint.GetX()), static_cast<int>(p_endPoint.GetY()));
  drawLine(startPoint, endPoint, p_color, p_penStyle);
}

void ScriblingView::drawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle) {
  QPainter painter(&m_image);
  painter.setPen(QPen(p_color, m_myPenWidth, p_penStyle, Qt::RoundCap, Qt::BevelJoin));
  painter.drawLine(p_startPoint, p_endPoint);

  update();
}

void ScriblingView::resizeImage(QImage* p_image, const QSize& p_newSize) {
  if (p_image->size() == p_newSize)
  {
    return;
  }

  QImage newImage(p_newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *p_image);
  *p_image = newImage;
}

void ScriblingView::mousePressEvent(QMouseEvent* p_event) {
  if (p_event->buttons() == Qt::LeftButton && !m_scribbling) {
    m_scribbling = true;
    Q_EMIT(Scribbling(p_event->pos()));
  }
}

void ScriblingView::mouseMoveEvent(QMouseEvent* p_event) {
  if (m_scribbling) {
    Q_EMIT(Moving(p_event->pos()));
  }
}

void ScriblingView::mouseReleaseEvent(QMouseEvent* p_event) {
  if (m_scribbling) {
    Q_EMIT(Slicing(p_event->pos()));
    m_scribbling = false;
  }
}

void ScriblingView::paintEvent(QPaintEvent* p_event) {
  QPainter painter(this);
  QRect dirtyRect = p_event->rect();
  painter.drawImage(dirtyRect, m_image, dirtyRect);
}

void ScriblingView::resizeEvent(QResizeEvent* p_event) {
  if (width() > m_image.width() || height() > m_image.height()) {
    int newWidth = qMax(width() + 128, m_image.width());
    int newHeight = qMax(height() + 128, m_image.height());
    resizeImage(&m_image, QSize(newWidth, newHeight));
    update();
  }
  QWidget::resizeEvent(p_event);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GameView
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GameView::GameView(QWidget* p_parent):
  QWidget(p_parent),
  m_gameLabel(new QLabel("Play")),
  m_pauseButton(new QPushButton("Pause")),
  m_gameModel(nullptr),
  m_scriblingView(new ScriblingView(p_parent)),
  m_linesCount(-1),
  m_linesGoal(-1),
  m_linesCountLabel(new QLabel),
  m_partsCount(-1),
  m_partsGoal(-1),
  m_partsCountLabel(new QLabel) {

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_gameLabel);
  menuLayout->addWidget(m_linesCountLabel);
  menuLayout->addWidget(m_partsCountLabel);
  menuLayout->addWidget(m_pauseButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_scriblingView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_scriblingView, 1);

  setLayout(mainLayout);

  connect(m_pauseButton, &QPushButton::clicked, this, &GameView::PauseRequested);
  connect(m_scriblingView, &ScriblingView::Scribbling, this, &GameView::Scribbling);
  connect(m_scriblingView, &ScriblingView::Moving, this, &GameView::Moving);
  connect(m_scriblingView, &ScriblingView::Slicing, this, &GameView::Slicing);
}

void GameView::SetModel(GameModel* p_gameModel) {
  m_gameModel = p_gameModel;
  m_scriblingView->setModel(p_gameModel);
}

void GameView::DrawLine(const ppxl::Segment& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  m_scriblingView->drawLine(p_line, p_color, p_penStyle);
}

void GameView::DrawFromModel() {
  m_scriblingView->drawFromModel();
}

void GameView::ClearImage() {
  m_scriblingView->clearImage();
}

void GameView::UpdateLinesCount(int p_linesCount, int p_linesGoal) {
  if (p_linesGoal != -1)
  {
    m_linesGoal = p_linesGoal;
  }

  m_linesCount = p_linesCount;
  m_linesCountLabel->setText(tr("Lines: %1/%2").arg(m_linesCount).arg(m_linesGoal));
}

void GameView::UpdatePartsCount(int p_partsCount, int p_partsGoal) {
  if (p_partsGoal != -1)
  {
    m_partsGoal = p_partsGoal;
  }

  m_partsCount = p_partsCount;
  m_partsCountLabel->setText(tr("Parts: %1/%2").arg(m_partsCount).arg(m_partsGoal));
}
