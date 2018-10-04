#include "PlayingScribblingView.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/GraphicsItem/GameOverItem.hxx"
#include "GUI/GraphicsItem/GameStartItem.hxx"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsEffect>

PlayingScribblingView::PlayingScribblingView(QWidget* p_parent):
  AbstractScribblingView2(p_parent),
  m_scribbling(false),
  m_cursorPosition(0, 0),
  m_gameOverItem(nullptr),
  m_gameStartItem(nullptr),
  m_overlayItem(nullptr),
  m_overlayBrush(QColor(0, 0, 0, 192)),
  m_levelNumber(-1),
  m_linesGoal(-1),
  m_partsGoal(-1),
  m_starsMax(-1)
{

  setFocusPolicy(Qt::StrongFocus);
}

void PlayingScribblingView::InitView() {
  AbstractScribblingView2::InitView();

  m_gameOverItem = new GameOverItem(0, 0, width()/3, 2*height()/3);
  m_gameStartItem = new GameStartItem(0, 0, width()/4, 2*height()/3, 20);

  connect(m_gameStartItem, &GameStartItem::StartLevelRequested, this, &PlayingScribblingView::FadeOutOverlay);
  connect(this, &PlayingScribblingView::FadeOutOverlayDone, this, &PlayingScribblingView::StartLevelRequested);
  connect(m_gameStartItem, &GameStartItem::CancelLevelRequested, this, &PlayingScribblingView::CancelLevelRequested);
}

PlayingScribblingView::~PlayingScribblingView() = default;

void PlayingScribblingView::DrawAreas(QList<double> const& p_areas) {
  auto polygons = GetModel()->GetPolygonsList();
  Q_ASSERT_X(p_areas.size() == polygons.size(), "PlayingScribblingView::DrawAreas", "Not as much areas as polygons.");

  for (int row = 0; row < polygons.size(); ++row) {
    auto polygon = polygons.at(row);
    auto area = QString::number(p_areas.at(row));
    DrawText(polygon->Barycenter(), area, 50);
  }
}

void PlayingScribblingView::AnimatePolygons(QList<ppxl::Vector> const& shiftVectors) {
  int i = 0;
  auto animationGroup = new QParallelAnimationGroup(this);
  for (auto* polygonItem: GetGraphicsPolygonList()) {
    auto shiftVector = shiftVectors.at(i);
    auto animation = new QPropertyAnimation(polygonItem, "pos", this);
    animation->setStartValue(polygonItem->pos());
    animation->setEndValue(QPointF(polygonItem->pos().x()+shiftVector.GetX(), polygonItem->pos().y()+shiftVector.GetY()));
    animation->setDuration(1000);
    animationGroup->addAnimation(animation);
    ++i;
  }

  connect(animationGroup, &QParallelAnimationGroup::finished, this, &PlayingScribblingView::PolygonsAnimationDone);
  animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayingScribblingView::SetLevelInfo(int p_levelNumber, int p_linesGoal, int p_partsGoal, int p_starsMax) {
  m_levelNumber = p_levelNumber;
  m_linesGoal = p_linesGoal;
  m_partsGoal = p_partsGoal;
  m_starsMax = p_starsMax;
}

void PlayingScribblingView::SetOverlayItem() {
  m_overlayItem = scene()->addRect(0, 0, scene()->width(), scene()->height(), QPen(Qt::NoPen), m_overlayBrush);
}

void PlayingScribblingView::DisplayGameStart() {
  SetOverlayItem();
  QPointF startTopLeftCorner(3.*width()/8., -2.*height()/3.);
  QPointF endStartLeftCorner(startTopLeftCorner.x(), height()/6.);

  m_gameStartItem->setPos(startTopLeftCorner);
  GetScene()->addItem(m_gameStartItem);
  m_gameStartItem->SetLevelInfo(m_levelNumber, m_linesGoal, m_partsGoal, m_starsMax);
  m_gameStartItem->Open(startTopLeftCorner, endStartLeftCorner);
  m_gameStartItem->setFocus();
}

void PlayingScribblingView::CloseGameStart() {
  m_gameStartItem->CloseToPlay();
}

void PlayingScribblingView::DisplayGameOver() {
  SetOverlayItem();
  m_gameOverItem->setPos(width()/3, -2*height()/3);
  GetScene()->addItem(m_gameOverItem);
  m_gameOverItem->Open(QPointF(width()/3, -2*height()/3), QPointF(width()/3, height()/6));
  m_gameOverItem->setFocus();
}

void PlayingScribblingView::FadeInOverlay() {
  auto effect = new QGraphicsOpacityEffect(this);
  m_overlayItem->setGraphicsEffect(effect);

  auto* animation = new QPropertyAnimation(effect, "opacity", this);

  animation->setStartValue(0.);
  animation->setEndValue(192./255.);
  animation->setDuration(200);

  connect(animation, &QPropertyAnimation::finished, this, &PlayingScribblingView::FadeInOverlayDone);

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayingScribblingView::FadeOutOverlay() {
  auto effect = new QGraphicsOpacityEffect(this);
  m_overlayItem->setGraphicsEffect(effect);

  auto* animation = new QPropertyAnimation(effect, "opacity", this);

  animation->setStartValue(192./255.);
  animation->setEndValue(0.);
  animation->setDuration(200);

  connect(animation, &QPropertyAnimation::finished, this, &PlayingScribblingView::FadeOutOverlayDone);

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayingScribblingView::keyPressEvent(QKeyEvent* p_event) {
  if (m_scribbling && p_event->key() == Qt::Key_Control)
  {
    Q_EMIT(ControlPressed(m_cursorPosition));
  }

  QWidget::keyPressEvent(p_event);
}

void PlayingScribblingView::keyReleaseEvent(QKeyEvent* p_event) {
  if (m_scribbling && p_event->key() == Qt::Key_Control)
  {
    Q_EMIT(ControlReleased(m_cursorPosition));
  }

  QWidget::keyReleaseEvent(p_event);
}

void PlayingScribblingView::mousePressEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && p_event->buttons() == Qt::LeftButton && !m_scribbling) {
    m_scribbling = true;
    m_cursorPosition = p_event->pos();
    Q_EMIT(Scribbling(p_event->pos()));
  }

  QGraphicsView::mousePressEvent(p_event);
}

void PlayingScribblingView::mouseMoveEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && m_scribbling) {
    m_cursorPosition = p_event->pos();
    Q_EMIT(Moving(p_event->pos()));
  }

  // Required to get hover events on graphics items
  QGraphicsView::mouseMoveEvent(p_event);
}

void PlayingScribblingView::mouseReleaseEvent(QMouseEvent* p_event) {
  if (GetCanScribble() && m_scribbling) {
    m_cursorPosition = p_event->pos();
    Q_EMIT(Slicing(p_event->pos()));
    m_scribbling = false;
  }

  QGraphicsView::mouseReleaseEvent(p_event);
}
