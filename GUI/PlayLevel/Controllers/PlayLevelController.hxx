#ifndef PLAYLEVELCONTROLLER_HXX
#define PLAYLEVELCONTROLLER_HXX

#include <QObject>

class PlayLevelWidget;

class PlayLevelController: public QObject {
  Q_OBJECT

public:
  explicit PlayLevelController(PlayLevelWidget* p_playLevelWidget, QObject* p_parent = nullptr);

  void RestartLevel();

Q_SIGNALS:

};

#endif
