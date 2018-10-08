#include "PlayingScribblingView.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/GraphicsItem/GameOverItem.hxx"
#include "GUI/GraphicsItem/GraphicsRoundedRectItem.hxx"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsEffect>
#include <QTimer>
#include <QObject>

PlayingScribblingView::PlayingScribblingView(QWidget* p_parent):
  AbstractScribblingView2(p_parent),
  m_scribbling(false),
  m_cursorPosition(0, 0),
  m_gameOverItem(nullptr),
  m_overlayItem(nullptr),
  m_overlayBrush(QColor(0, 0, 0, 192)),
  m_linesCount(-1),
  m_linesGoal(-1),
  m_partsCount(-1),
  m_partsGoal(-1),
  m_starsMax(-1),
  m_areas(),
  m_shiftVectors(),
  m_figureCenter() {

  setFocusPolicy(Qt::StrongFocus);
}

void PlayingScribblingView::InitView() {
  AbstractScribblingView2::InitView();

  m_gameOverItem = new GameOverItem(0, 0, width()/3, 700, 20.);
}

PlayingScribblingView::~PlayingScribblingView() = default;

void PlayingScribblingView::AnimatePolygons() {
  int i = 0;
  auto animationGroup = new QParallelAnimationGroup(this);
  for (auto* polygonItem: GetGraphicsPolygonList()) {
    auto shiftVector = m_shiftVectors.at(i);
    auto animation = new QPropertyAnimation(polygonItem, "pos", this);
    animation->setStartValue(polygonItem->pos());
    animation->setEndValue(QPointF(polygonItem->pos().x()+shiftVector.GetX(), polygonItem->pos().y()+shiftVector.GetY()));
    animation->setDuration(1000);
    animationGroup->addAnimation(animation);
    ++i;
  }

  connect(animationGroup, &QParallelAnimationGroup::finished, this, &PlayingScribblingView::DisplayWinOrFail);
  animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayingScribblingView::DisplayGameOver() {
  m_gameOverItem->setPos(width()/3, -2*height()/3);
  scene()->addItem(m_gameOverItem);
  m_gameOverItem->SetAreasData(GetModel()->GetPolygonsList(), m_areas, m_figureCenter);
  m_gameOverItem->SetLevelData(m_linesCount, m_linesGoal, m_partsCount, m_partsGoal, m_starsMax);
  m_gameOverItem->Open(QPointF(width()/3, -2*height()/3), QPointF(width()/3, height()/6));
  m_gameOverItem->setFocus();

  connect(m_gameOverItem, &GameOverItem::ReplayRequested, this, &PlayingScribblingView::ReplayRequested);
  connect(m_gameOverItem, &GameOverItem::GoToMapRequested, this, &PlayingScribblingView::GoToMapRequested);
}

void PlayingScribblingView::SetEndLevelInfo(int p_linesCount, int p_linesGoal, int p_partsCount, int p_partsGoal, int p_stars) {
  m_linesCount = p_linesCount;
  m_linesGoal = p_linesGoal;
  m_partsCount = p_partsCount;
  m_partsGoal = p_partsGoal;
  m_starsMax = p_stars;
}

void PlayingScribblingView::SetAreasData(QList<double> const& p_areas, QList<ppxl::Vector> p_shiftVectors, ppxl::Point const&  p_figureCenter) {
  m_areas = p_areas;
  m_shiftVectors = p_shiftVectors;
  m_figureCenter = p_figureCenter;
}

void PlayingScribblingView::DisplayWinOrFail() {
  QString endMessage;
  bool fail = m_starsMax == 0;
  bool perfect = m_starsMax == 4;
  if (fail) {
    endMessage = tr("Fail");
  } else {
    if (perfect) {
      endMessage = tr("PERFECT");
    } else {
      endMessage = tr("Awesome");
    }
  }

  auto endMessageRectItem = new GraphicsRoundedRectItem(0, 0, 400, 150, 20, 20);
  auto endMessageTextItem = new QGraphicsTextItem(endMessage, endMessageRectItem);
  endMessageTextItem->setFont(QFont("", 36, 99));
  endMessageTextItem->setPos(endMessageRectItem->boundingRect().center() - endMessageTextItem->boundingRect().center());
  endMessageTextItem->setDefaultTextColor(Qt::white);

  QPointF startPos(scene()->width()/2. - endMessageRectItem->boundingRect().center().x(), -endMessageRectItem->boundingRect().height());
  QPointF endPos = QPointF(scene()->width()/2., scene()->height()/2.) - endMessageRectItem->boundingRect().center();
  endMessageRectItem->setPos(startPos);

  scene()->addItem(endMessageRectItem);
  endMessageRectItem->setPos(startPos);
  endMessageRectItem->setPen(Qt::NoPen);
  endMessageRectItem->setBrush(QBrush(QColor("#3d3a4f")));

  if (fail) {
    AnimateFail(endMessageRectItem, startPos, endPos);
  } else {
    if (perfect) {
      endMessageRectItem->setPos(scene()->sceneRect().center() - endMessageRectItem->boundingRect().center());
      AnimatePerfect(endMessageRectItem, endMessageRectItem->rect());
    } else {
      AnimateWin(endMessageRectItem, startPos, endPos);
    }
  }
}

void PlayingScribblingView::AnimateFail(GraphicsRoundedRectItem* p_item, QPointF const& p_startPos, QPointF const& p_endPos) {
  auto animationIn = new QPropertyAnimation(p_item, "pos", this);
  animationIn->setStartValue(p_startPos);
  animationIn->setEndValue(p_endPos);
  animationIn->setDuration(500);
  animationIn->setEasingCurve(QEasingCurve::OutBounce);

  auto effect = new QGraphicsOpacityEffect(this);
  p_item->setGraphicsEffect(effect);
  p_item->setOpacity(1.);
  auto animationOut = new QPropertyAnimation(effect, "opacity", this);
  animationOut->setStartValue(1.);
  animationOut->setEndValue(0.);
  animationOut->setDuration(200);

  auto animation = new QSequentialAnimationGroup(this);
  animation->addAnimation(animationIn);
  animation->addPause(1000);
  animation->addAnimation(animationOut);
  connect(animation, &QSequentialAnimationGroup::finished, this, &PlayingScribblingView::DisplayGameOver);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayingScribblingView::AnimateWin(QObject* p_item, QPointF const& p_startPos, QPointF const& p_endPos) {
  auto animationIn = new QPropertyAnimation(p_item, "pos", this);
  animationIn->setStartValue(p_startPos);
  animationIn->setEndValue(p_endPos);
  animationIn->setDuration(500);
  animationIn->setEasingCurve(QEasingCurve::OutBack);

  auto animationOut = new QPropertyAnimation(p_item, "pos", this);
  animationOut->setStartValue(p_endPos);
  animationOut->setEndValue(p_startPos);
  animationOut->setDuration(500);
  animationOut->setEasingCurve(QEasingCurve::InBack);

  auto animation = new QSequentialAnimationGroup(this);
  animation->addAnimation(animationIn);
  animation->addPause(1000);
  animation->addAnimation(animationOut);
  connect(animation, &QSequentialAnimationGroup::finished, this, &PlayingScribblingView::DisplayGameOver);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayingScribblingView::AnimatePerfect(QObject* p_rectItem, QRectF const& p_endRect) {
  auto animationInScale = new QPropertyAnimation(p_rectItem, "scale", this);
  animationInScale->setStartValue(0.);
  animationInScale->setEndValue(1.);
  animationInScale->setDuration(250);
  animationInScale->setEasingCurve(QEasingCurve::InExpo);

  auto animationInPos = new QPropertyAnimation(p_rectItem, "pos", this);
  animationInPos->setStartValue(sceneRect().center());
  animationInPos->setEndValue(QPointF(sceneRect().center().x() - p_endRect.width()/2., sceneRect().center().y() - p_endRect.height()/2.));
  animationInPos->setDuration(250);
  animationInPos->setEasingCurve(QEasingCurve::InExpo);

  auto animationIn = new QParallelAnimationGroup(this);
  animationIn->addAnimation(animationInScale);
  animationIn->addAnimation(animationInPos);

  auto animation = new QSequentialAnimationGroup(this);
  animation->addAnimation(animationIn);
  animation->addPause(1000);
  connect(animation, &QParallelAnimationGroup::finished, this, &PlayingScribblingView::DisplayGameOver);
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
