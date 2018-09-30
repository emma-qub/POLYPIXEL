#ifndef MAPVIEW_HXX
#define MAPVIEW_HXX

#include <QGraphicsView>

class Player;

class MapView: public QGraphicsView {
public:
  explicit MapView(QWidget* p_parent = nullptr);

private:
  QGraphicsScene* m_scene;
  Player* m_player;
};

#endif
