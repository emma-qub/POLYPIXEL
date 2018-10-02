#ifndef MAPVIEW_HXX
#define MAPVIEW_HXX

#include "GUI/GraphicsItem/PlayerItem.hxx"

#include <QGraphicsView>
#include <QMap>
#include <memory>

class PlayerItem;

struct Level {
  Level(int p_levelNumber, QString const& p_levelName, QString const& p_levelPath):
    m_levelNumber(p_levelNumber),
    m_levelName(p_levelName),
    m_levelPath(p_levelPath) {}

int m_levelNumber;
QString m_levelName;
QString m_levelPath;
};

struct Path {
  enum Directions {
    eRight,
    eUp,
    eLeft,
    eDown
  };

  Path(Level* p_levelStart, QList<Directions> const& p_stepsList, Level* p_levelEnd):
    m_levelStart(p_levelStart),
    m_directionsList(p_stepsList),
    m_endLevel(p_levelEnd) {}

  Level* FindDestination(Level* p_level) {
    if (p_level == m_levelStart) {
      return m_endLevel;
    } else if (p_level == m_endLevel) {
      return m_levelStart;
    } else {
      return nullptr;
    }
  }

Level* m_levelStart;
QList<Directions> m_directionsList;
Level* m_endLevel;
};

struct World {
  World(int p_worldNumber, QString const& p_worldName);
  virtual ~World();

  void AppendLevel(QPointF const& p_position, QString const& p_levelName, QList<Path::Directions> const& p_stepsList = QList<Path::Directions>());
  Path GetPathFromStartLevel(Level* p_startLevel) const;

int m_worldNumber;
QString m_worldName;
QMap<Level*, QPointF> m_positionByLevel;
QList<Level*> m_levelsList;
QList<Path> m_pathsList;
Level* m_previousLevel;
Level* m_nextLevel;
};

class MapView: public QGraphicsView {
  Q_OBJECT

public:
  explicit MapView(QWidget* p_parent = nullptr);
  ~MapView() override;

  void InitView();

signals:
  void PlayerReadchedDestination();
  void PlayLevelRequested(QString const& p_levelPath);

protected:
  void OpenMessageBoxTest();
  void InitWorld1();
  void MovePlayer(PlayerItem::Direction p_direction);
  void AnimatePlayer();
  void PlayLevel();

private:
  QGraphicsScene* m_scene;
  PlayerItem* m_player;
  QList<World*> m_worlds;
  Level* m_currentLevel;
  World* m_currentWorld;
  QList<Path::Directions> m_currentDirections;
  bool m_playerIsMoving;
  bool viewInitialized;
};

#endif
