#include "MapView.hxx"

#include "GUI/GraphicsItem/GraphicsMessageBoxItem.hxx"

#include <QAction>
#include <QPropertyAnimation>



#include <QDebug>



MapView::MapView(QWidget* p_parent):
  QGraphicsView(p_parent),
  m_scene(nullptr),
  m_player(nullptr),
  m_worlds(),
  m_currentLevel(nullptr),
  m_playerIsMoving(false),
  viewInitialized(false) {
}

MapView::~MapView() {
  for (auto* world: m_worlds) {
    delete world;
  }
}

void MapView::InitView() {
  if (viewInitialized) {
    return;
  }

  // Hide scrollbars
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Set scene
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);
  m_scene->setSceneRect(0, 0, width(), height());
  m_scene->setBackgroundBrush(QBrush(QColor("#f222ff")));

  // Open message action test
  auto openMessageAction = new QAction(this);
  addAction(openMessageAction);
  openMessageAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  connect(openMessageAction, &QAction::triggered, this, &MapView::OpenMessageBoxTest);

  // Init World 1
  InitWorld1();

  // Get World 1
  m_currentWorld = m_worlds.at(0);

  // Draw paths
  int horizontalLineLength = width() / 10;
  int verticalLineLength = height() / 10;

  for (auto path: m_currentWorld->m_pathsList) {
    auto startLevel = path.m_levelStart;
    QPointF startPosition(m_currentWorld->m_positionByLevel[startLevel]);
    for (auto direction: path.m_directionsList) {
      auto line = new QGraphicsLineItem;
      switch (direction) {
      case Path::eRight: {
        QPointF endPosition(startPosition.x()+horizontalLineLength, startPosition.y());
        line->setLine(QLineF(startPosition, endPosition));
        startPosition = endPosition;
        break;
      }
      case Path::eUp: {
        QPointF endPosition(startPosition.x(), startPosition.y()-verticalLineLength);
        line->setLine(QLineF(startPosition, endPosition));
        startPosition = endPosition;
        break;
      }
      case Path::eLeft: {
        QPointF endPosition(startPosition.x()-horizontalLineLength, startPosition.y());
        line->setLine(QLineF(startPosition, endPosition));
        startPosition = endPosition;
        break;
      }
      case Path::eDown: {
        QPointF endPosition(startPosition.x(), startPosition.y()+verticalLineLength);
        line->setLine(QLineF(startPosition, endPosition));
        startPosition = endPosition;
        break;
      }
      }
      m_scene->addItem(line);
    }
  }


  // Draw Levels
  int levelSemiSide = 10;
  int levelSide = 2*levelSemiSide;

  for (auto* level: m_currentWorld->m_levelsList) {
    auto levelPosition = m_currentWorld->m_positionByLevel[level];
    auto levelItem = new QGraphicsRectItem(levelPosition.x()-levelSemiSide, levelPosition.y()-levelSemiSide, levelSide, levelSide);
    levelItem->setBrush(QBrush(QColor("#ffffff")));
    m_scene->addItem(levelItem);
  }

  // Create player
  m_player = new PlayerItem;
  m_scene->addItem(m_player);
  m_player->setFocus();
  m_currentLevel = m_currentWorld->m_levelsList.at(0);
  auto firstLevelPosition = m_currentWorld->m_positionByLevel[m_currentLevel];
  auto playerBoundungRect = m_player->boundingRect();
  m_player->setPos(firstLevelPosition.x()-playerBoundungRect.width()/2, firstLevelPosition.y()-playerBoundungRect.height()/2);

  connect(m_player, &PlayerItem::Move, this, &MapView::MovePlayer);
  connect(m_player, &PlayerItem::Enter, this, &MapView::PlayLevel);

  viewInitialized = true;
}

void MapView::InitWorld1() {
  auto world1 = new World(1, "Classic 80's");
  world1->AppendLevel(QPointF(10*width()/100, 80*height()/100), "");
  world1->AppendLevel(QPointF(20*width()/100, 60*height()/100), "", {Path::eRight, Path::eUp, Path::eUp});
  world1->AppendLevel(QPointF(20*width()/100, 50*height()/100), "", {Path::eUp});

  m_worlds << world1;
}

void MapView::MovePlayer(PlayerItem::Direction p_direction) {
  if (m_playerIsMoving) {
    return;
  }

  // Get path according to level
  auto path = m_currentWorld->GetPathFromStartLevel(m_currentLevel);
  Q_ASSERT_X(path.m_directionsList.isEmpty() == false, "MapView::MovePlayer", "Player is soft locked.");

  // Get first direction
  auto firstDirection = path.m_directionsList.first();

  // If first direction and player's direction are the same
  if (static_cast<int>(firstDirection) == static_cast<int>(p_direction)) {
    m_currentDirections = path.m_directionsList;
    m_playerIsMoving = true;
    AnimatePlayer();
    connect(this, &MapView::PlayerReadchedDestination, this, [this, path](){
      m_playerIsMoving = false;
      m_currentLevel = path.m_endLevel;
    });
  }
}

void MapView::AnimatePlayer() {
  if (m_currentDirections.isEmpty()) {
    Q_EMIT(PlayerReadchedDestination());
    return;
  }

  auto animation = new QPropertyAnimation(m_player, "pos", this);
  auto startValue = m_player->pos();
  animation->setStartValue(startValue);
  QPointF endValue(animation->startValue().toPointF());
  switch (m_currentDirections.first()) {
  case Path::eRight: {
    endValue.rx() += width()/10;
    break;
  }
  case Path::eUp: {
    endValue.ry() -= height()/10;
    break;
  }
  case Path::eLeft: {
    endValue.rx() -= width()/10;
    break;
  }
  case Path::eDown: {
    endValue.ry() += height()/10;
    break;
  }
  }
  m_currentDirections.pop_front();
  animation->setEndValue(endValue);
  animation->setDuration(500);
  //animation->setEasingCurve(QEasingCurve::InOutQuint);
  connect(animation, &QPropertyAnimation::finished, this, &MapView::AnimatePlayer);
  connect(animation, &QPropertyAnimation::finished, animation, &QPropertyAnimation::deleteLater);
  animation->start();

}

void MapView::PlayLevel() {
  Q_EMIT(PlayLevelRequested(m_currentLevel->m_levelPath));
}

void MapView::OpenMessageBoxTest() {
  auto messageBoxItem = new GraphicsMessageBoxItem(width()-200, 200,
    "BIENVENUE DANS NOTRE ROYAUME, HUMAIN. VOTRE MISSION, SI VOUS L'ACCEPTEZ,"
    " CONSISTERA A DECOUPER DES POLYGONES SELON CERTAINES REGLES, QUE JE VOUS"
    " EXPLIQUERAI AU FUR ET A MESURE DE VOTRE AVENTURE.");
  m_scene->addItem(messageBoxItem);
  messageBoxItem->setPos(-50, 80);
  messageBoxItem->setFocus();
  messageBoxItem->Open();
  connect(messageBoxItem, &GraphicsMessageBoxItem::Done, this, [this, messageBoxItem](){
    m_scene->removeItem(messageBoxItem);
    delete messageBoxItem;
    m_player->setFocus();
  });
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// World
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

World::World(int p_worldNumber, const QString& p_worldName):
  m_worldNumber(p_worldNumber),
  m_worldName(p_worldName),
  m_positionByLevel(),
  m_levelsList(),
  m_pathsList(),
  m_previousLevel(nullptr),
  m_nextLevel(nullptr) {
}

World::~World() {
  for (auto* level: m_levelsList) {
    delete level;
  }
}

void World:: AppendLevel(const QPointF& p_position, QString const& p_levelName, const QList<Path::Directions>& p_stepsList) {
  auto levelNumber = m_levelsList.size()+1;
  QString levelNumberString = levelNumber < 10 ? "0"+QString::number(levelNumber) : QString::number(levelNumber);
  auto level = new Level(levelNumber, p_levelName, QString(":/levels/W%1_L%2.ppxl").arg(m_worldNumber).arg(levelNumberString));
  m_levelsList << level;
  if (!p_stepsList.isEmpty() && levelNumber > 1) {
    m_pathsList << Path(m_levelsList.at(levelNumber-2), p_stepsList, m_levelsList.at(levelNumber-1));
  }
  m_positionByLevel[level] = p_position;

}

Path World::GetPathFromStartLevel(Level* p_startLevel) const
{
  for (auto const& path: m_pathsList) {
    if (path.m_levelStart == p_startLevel) {
      return path;
    }
  }

  return Path(nullptr, QList<Path::Directions>(), nullptr);
}
