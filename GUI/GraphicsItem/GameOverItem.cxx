#include "GameOverItem.hxx"
#include "Core/Geometry/Polygon.hxx"
#include "GUI/GraphicsItem/GraphicsStarsItem.hxx"

#include <QBrush>
#include <QPropertyAnimation>
#include <QList>
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QGraphicsProxyWidget>

GameOverItem::GameOverItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, qreal p_radius, QGraphicsItem* p_parent):
  GraphicsRoundedRectItem(p_x, p_y, p_width, p_height, p_radius, p_radius, p_parent),
  m_polygonsList(),
  m_areas(),
  m_figureCenter(),
  m_overlayItem(nullptr),
  m_startPos(),
  m_endPos(),
  m_levelNumber(-1),
  m_linesCount(-1),
  m_linesGoal(-1),
  m_partsCount(-1),
  m_partsGoal(-1),
  m_starsMax(-1) {

  setBrush(QBrush(QColor("#ffffff")));
  setPen(Qt::NoPen);
}

GameOverItem::~GameOverItem() = default;

void GameOverItem::Open(QPointF const& p_startPos, QPointF const& p_endPos) {
  DrawResult();

  show();
  m_overlayItem->show();

  m_startPos = p_startPos;
  m_endPos = p_endPos;

  // Move
  auto moveAnimation = new QPropertyAnimation(this, "pos", this);
  moveAnimation->setStartValue(m_startPos);
  moveAnimation->setEndValue(m_endPos);
  moveAnimation->setDuration(500);
  moveAnimation->setEasingCurve(QEasingCurve::OutBack);

  // Fade in
  auto effect = new QGraphicsOpacityEffect(this);
  m_overlayItem->setGraphicsEffect(effect);
  auto* fadeInAnimation = new QPropertyAnimation(effect, "opacity", this);
  fadeInAnimation->setStartValue(0.);
  fadeInAnimation->setEndValue(0.5);
  fadeInAnimation->setDuration(200);

  // Group animation
  auto animation = new QParallelAnimationGroup(this);
  animation->addAnimation(moveAnimation);
  animation->addAnimation(fadeInAnimation);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameOverItem::SetAreasData(QList<ppxl::Polygon*> p_polygonsList, QList<double> const& p_areas, ppxl::Point const& p_figureCenter) {
  Q_ASSERT_X(p_areas.size() == p_polygonsList.size(), "PlayingScribblingView::DrawAreas", "Not as much areas as polygons.");

  m_polygonsList = p_polygonsList;
  m_areas = p_areas;
  m_figureCenter = p_figureCenter;
}

void GameOverItem::SetLevelData(int m_linesCount, int m_linesGoal, int m_partsCount, int m_partsGoal, int m_starsMax) {
  auto height = boundingRect().height();
  auto width = boundingRect().width();

  auto topRect = new GraphicsTopRoundedRectItem(0, 0, width, height/4., 20, 20, this);
  topRect->setBrush(QBrush(QColor("#993aef")));
  topRect->setPen(Qt::NoPen);

  QBrush whiteBrush(Qt::white);
  QFont font(":/fonts/PICOPIXEL.ttf", 18, 49);
  auto linesAndPartsItem = new QGraphicsSimpleTextItem(QString("%1/%2   %3/%4").arg(m_linesCount).arg(m_linesGoal).arg(m_partsCount).arg(m_partsGoal), this);
  linesAndPartsItem->setPen(Qt::NoPen);
  linesAndPartsItem->setBrush(whiteBrush);
  linesAndPartsItem->setFont(font);
  linesAndPartsItem->setPos(width/2. - linesAndPartsItem->boundingRect().center().x(), 10);

  auto starsItem = new GraphicsStarsItem(std::min(m_starsMax, 3), this);
  starsItem->setPos(width/2. - starsItem->boundingRect().center().x(), linesAndPartsItem->boundingRect().height()+20);

  if (m_starsMax == 4) {
    auto perfectItem = new QGraphicsSimpleTextItem("Perfect", this);
    perfectItem->setPen(Qt::NoPen);
    perfectItem->setBrush(whiteBrush);
    perfectItem->setFont(font);
    perfectItem->setPos(width - linesAndPartsItem->boundingRect().width() - 10, topRect->boundingRect().center().y());
  }

  QRectF closeRect(0, 3.*height/4., width/4., height/4.);
  QRectF playRect(width/4., 3.*height/4., 3.*width/4., height/4.);

  auto bottomCloseRect = new GraphicsBottomLeftRoundedRectItem(0, 0, width/4., height/4., 20, 20, this);
  bottomCloseRect->setBrush(QBrush(QColor("#c95489")));
  bottomCloseRect->setPen(Qt::NoPen);
  bottomCloseRect->setPos(closeRect.topLeft());

  auto bottomPlayRect = new GraphicsBottomRightRoundedRectItem(0, 0, 3.*width/4., height/4., 20, 20, this);
  bottomPlayRect->setBrush(QBrush(QColor("#ef3a89")));
  bottomPlayRect->setPen(Qt::NoPen);
  bottomPlayRect->setPos(playRect.topLeft());

  QString buttonStyle =
    "QPushButton {"
    "  font-family: ':/fonts/PICOPIXEL.ttf';"
    "  font-style: normal;"
    "  font-size: 36px;"
    "}";

  if (m_starsMax > 0) {
    auto continueButton = new QPushButton("Continue");
    continueButton->setFlat(true);
    continueButton->setStyleSheet(buttonStyle);
    continueButton->setFixedSize(static_cast<int>(2.*playRect.width()/3.), static_cast<int>(playRect.height()/2.));
    auto proxyContinueItem = new QGraphicsProxyWidget(this);
    proxyContinueItem->setWidget(continueButton);
    proxyContinueItem->setPos(playRect.center() - proxyContinueItem->rect().center());

    connect(continueButton, &QPushButton::clicked, this, [this](){CloseToMap(true);});

    auto replayButton = new QPushButton(QIcon(QPixmap(":/sprites/level/replay.svg").scaled(32, 32)), "");
    replayButton->setIconSize(QSize(32, 32));
    replayButton->setFlat(true);
    replayButton->setFixedSize(static_cast<int>(closeRect.height())/2, static_cast<int>(closeRect.height())/2);
    auto proxyReplayItem = new QGraphicsProxyWidget(this);
    proxyReplayItem->setWidget(replayButton);
    proxyReplayItem->setPos(closeRect.center() - proxyReplayItem->rect().center());

    connect(replayButton, &QPushButton::clicked, this, &GameOverItem::CloseToReplay);
  } else {
    auto replayButton = new QPushButton("Replay");
    replayButton->setFlat(true);
    replayButton->setStyleSheet(buttonStyle);
    replayButton->setFixedSize(static_cast<int>(2.*playRect.width()/3.), static_cast<int>(playRect.height()/2.));
    auto proxyReplayItem = new QGraphicsProxyWidget(this);
    proxyReplayItem->setWidget(replayButton);
    proxyReplayItem->setPos(playRect.center() - proxyReplayItem->rect().center());

    connect(replayButton, &QPushButton::clicked, this, &GameOverItem::CloseToReplay);

    auto mapButton = new QPushButton(QIcon(QPixmap(":/sprites/level/map.svg").scaled(32, 32)), "");
    mapButton->setIconSize(QSize(32, 32));
    mapButton->setFlat(true);
    mapButton->setFixedSize(static_cast<int>(closeRect.height())/2, static_cast<int>(closeRect.height())/2);
    auto proxyMapItem = new QGraphicsProxyWidget(this);
    proxyMapItem->setWidget(mapButton);
    proxyMapItem->setPos(closeRect.center() - proxyMapItem->rect().center());

    connect(mapButton, &QPushButton::clicked, this, [this](){CloseToMap(false);});
  }
}

void GameOverItem::DrawResult() {
  m_overlayItem = scene()->addRect(0, 0, scene()->width(), scene()->height(), QPen(Qt::NoPen), QBrush(Qt::black));
  m_overlayItem->setZValue(-1);
  m_overlayItem->hide();

  double xMin = scene()->width();
  double xMax = 0.;
  double yMin = scene()->height();
  double yMax = 0.;

  QList<QGraphicsPolygonItem*> polygonItemsList;
  QList<QPointF> barycentersList;

  for (int row = 0; row < m_polygonsList.size(); ++row) {
    auto polygon = m_polygonsList.at(row);

    auto barycenter = polygon->Barycenter();
    barycentersList << QPointF(barycenter.GetX(), barycenter.GetY());

    QVector<QPointF> vertices;
    for (auto const& vertex: polygon->GetVertices()) {
      vertices << QPointF(vertex.GetX(), vertex.GetY());
    }
    auto polygonItem = new QGraphicsPolygonItem(QPolygonF(vertices), this);
    polygonItemsList << polygonItem;

    auto boundingRect = polygonItem->boundingRect();
    xMin = std::min(xMin, boundingRect.left());
    xMax = std::max(xMin, boundingRect.right());
    yMin = std::min(xMin, boundingRect.top());
    yMax = std::max(xMin, boundingRect.bottom());
  }
  QRectF polygonsBoundingRect(xMin, yMin, xMax-xMin, yMax-yMin);

  int i = 0;
  for (auto* graphicPolygon: polygonItemsList) {
    auto barycenter = barycentersList.at(i);
    auto area = m_areas.at(i);
    auto areaItem = new QGraphicsSimpleTextItem(QString::number(area, 'f', 1), graphicPolygon);
    areaItem->setFont(QFont("Arial", 24, 50));
    areaItem->setPos(barycenter-areaItem->boundingRect().center());

    auto c1 = this->boundingRect().center();
    auto c2 = polygonsBoundingRect.center();
    auto c3 = graphicPolygon->transformOriginPoint();
    graphicPolygon->setTransform(QTransform().translate((c1 - c3).x(), (c1 - c3).y()).scale(0.5, 0.5).translate((c3 - c2).x(), (c3 - c2).y()));
    graphicPolygon->setPen(QPen(QBrush(QColor("#2ed03c")), 10));

    ++i;
  }
}

void GameOverItem::CloseToReplay() {
  auto animation = new QPropertyAnimation(this, "pos", this);
  animation->setStartValue(m_endPos);
  animation->setEndValue(QPointF(m_endPos.x(), scene()->height()));
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::InBack);

  connect(animation, &QPropertyAnimation::finished, this, &GameOverItem::ReplayRequested);
  connect(animation, &QPropertyAnimation::finished, this, [this](){hide();m_overlayItem->hide();});

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void GameOverItem::CloseToMap(bool p_moveToNextLevel) {
  // Fade Out dialog
  auto dialogEffect = new QGraphicsOpacityEffect(parent());
  this->setGraphicsEffect(dialogEffect);
  auto dialogAnimation = new QPropertyAnimation(dialogEffect, "opacity", this);
  dialogAnimation->setStartValue(1.);
  dialogAnimation->setEndValue(0.);
  dialogAnimation->setDuration(250);

  // Fade Out overlay
  auto overlayEffect = new QGraphicsOpacityEffect(this);
  m_overlayItem->setGraphicsEffect(overlayEffect);
  auto* overlayAnimation = new QPropertyAnimation(overlayEffect, "opacity", this);
  overlayAnimation->setStartValue(0.5);
  overlayAnimation->setEndValue(0.);
  overlayAnimation->setDuration(250);

  // Group animation
  auto animation = new QParallelAnimationGroup(this);
  animation->addAnimation(dialogAnimation);
  animation->addAnimation(overlayAnimation);
  animation->start(QAbstractAnimation::DeleteWhenStopped);
  connect(animation, &QPropertyAnimation::finished, this, [this, p_moveToNextLevel](){Q_EMIT(GoToMapRequested(p_moveToNextLevel));});
  connect(animation, &QPropertyAnimation::finished, this, [this](){hide();m_overlayItem->hide();});

  animation->start(QAbstractAnimation::DeleteWhenStopped);
}
