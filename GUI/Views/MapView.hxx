#ifndef MAPVIEW_HXX
#define MAPVIEW_HXX

#include <QGraphicsView>
#include <QMap>

class Player;

class MapView: public QGraphicsView {
public:
  explicit MapView(QWidget* p_parent = nullptr);

protected:
  void OpenMessageBoxTest();

private:
  QGraphicsScene* m_scene;
  Player* m_player;
};

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
  Path(Level* p_levelStart, QList<QPoint> const& p_stepsList, Level* p_levelEnd):
    m_levelStart(p_levelStart),
    m_stepsList(p_stepsList),
    m_levelEnd(p_levelEnd) {}

  Level* FindDestination(Level* p_level) {
    if (p_level == m_levelStart) {
      return m_levelEnd;
    } else if (p_level == m_levelEnd) {
      return m_levelStart;
    } else {
      return nullptr;
    }
  }

Level* m_levelStart;
QList<QPoint> m_stepsList;
Level* m_levelEnd;
};

struct World {
  World();

int m_worldNumber;
QString m_worldName;
QMap<QPoint, Level*> m_levelsPosition;
QList<Level*> m_levelsList;
QList<Path> m_pathsList;
};

#endif
