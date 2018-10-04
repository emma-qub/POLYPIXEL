#include "MapView.hxx"

#include "GUI/GraphicsItem/GraphicsMessageBoxItem.hxx"

#include <QAction>
#include <QPropertyAnimation>
#include <QFile>
#include <QTextStream>



#include <QDebug>



MapView::MapView(QWidget* p_parent):
  QGraphicsView(p_parent),
  m_scene(nullptr),
  m_player(nullptr),
  m_worlds(),
  m_currentLevel(nullptr),
  m_playerIsMoving(false),
  m_viewInitialized(false) {
}

MapView::~MapView() {
  for (auto* world: m_worlds) {
    delete world;
  }
}

void MapView::InitView() {
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
    auto startLevel = path.m_startLevel;
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

  m_viewInitialized = true;
}

void MapView::InitWorld1() {
  // Read world information from map file
  QFile world1MapFile(":/maps/world1.map");
  Q_ASSERT_X(world1MapFile.open(QIODevice::ReadOnly), "MapView::InitWorld1", QString("Cannot open Map file: %1").arg(world1MapFile.errorString()).toLatin1());

  // Create world
  auto world1 = new World(1, "Classic 80's");
  // Create 20 levels
  for (int levelNumber = 1; levelNumber <= 20; ++levelNumber) {
    QString levelNumberString = levelNumber < 10 ? "0"+QString::number(levelNumber) : QString::number(levelNumber);
    auto level = new Level(levelNumber, "", QString(":/levels/W%1_L%2.ppxl").arg(world1->m_worldNumber).arg(levelNumberString));
    world1->m_levelsList << level;
  }
  // Init first level position
  auto firstLevelPosition = QPointF(10*width()/100, 80*height()/100);
  world1->SetFirstLevelPosition(firstLevelPosition);
  world1->m_positionByLevel[world1->m_levelsList.at(0)] = firstLevelPosition;

  // Set levels and paths
  QTextStream inFile(&world1MapFile);
  do {
    auto line = inFile.readLine();
    if (line.isEmpty() || line.startsWith("#")) {
      continue;
    }

    auto pathsList = line.split("|");
    auto startLevelNumber = pathsList.takeFirst().toInt();
    for (auto const& path: pathsList) {
      auto pathInfo = path.split(";");
      auto directionsStringList = pathInfo.at(0).split(",");
      auto endLevelNumber = pathInfo.at(1).toInt();
      double relativeX = 0;
      double relativeY = 0;
      QList<Path::Directions> directionsList;
      QList<Path::Directions> reverDirectionsList;
      for (auto directionLetter: directionsStringList) {
        if (directionLetter == "r") {
          directionsList.append(Path::eRight);
          reverDirectionsList.prepend(Path::eLeft);
          relativeX += width()/10;
        } else if (directionLetter == "u") {
          directionsList.append(Path::eUp);
          reverDirectionsList.prepend(Path::eDown);
          relativeY -= height()/10;
        } else if (directionLetter == "l") {
          directionsList.append(Path::eLeft);
          reverDirectionsList.prepend(Path::eRight);
          relativeX -= width()/10;
        } else if (directionLetter == "d") {
          directionsList.append(Path::eDown);
          reverDirectionsList.prepend(Path::eUp);
          relativeY += height()/10;
        }
      }
      world1->AppendPath(Path(world1->m_levelsList.at(startLevelNumber-1), directionsList, world1->m_levelsList.at(endLevelNumber-1)));
      world1->AppendPath(Path(world1->m_levelsList.at(endLevelNumber-1), reverDirectionsList, world1->m_levelsList.at(startLevelNumber-1)));
      auto startLevelPosition = world1->m_positionByLevel[world1->m_levelsList.at(startLevelNumber-1)];
      auto endLevelPosition = startLevelPosition;
      endLevelPosition.rx() += relativeX;
      endLevelPosition.ry() += relativeY;
      world1->m_positionByLevel[world1->m_levelsList.at(endLevelNumber-1)] = endLevelPosition;
    }

  } while (!inFile.atEnd());

  world1MapFile.close();

  m_worlds << world1;
}

void MapView::MovePlayer(PlayerItem::Direction p_direction) {
  if (m_playerIsMoving) {
    return;
  }

  // Get path according to level
  auto pathsList = m_currentWorld->GetPathsFromStartLevel(m_currentLevel);
  Q_ASSERT_X(pathsList.isEmpty() == false, "MapView::MovePlayer", "Player is soft locked.");

  for (auto const& path: pathsList) {
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

  animation->start(QAbstractAnimation::DeleteWhenStopped);
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

void World::SetFirstLevelPosition(const QPointF& p_position) {
  m_firstLevelPosition = p_position;
}

void World::AppendPath(const Path& p_path) {
  m_pathsList << p_path;
}

QList<Path> World::GetPathsFromStartLevel(Level* p_startLevel) const
{
  QList<Path> paths;
  for (auto const& path: m_pathsList) {
    if (path.m_startLevel == p_startLevel) {
      paths << path;
    }
  }

  return paths;
}
