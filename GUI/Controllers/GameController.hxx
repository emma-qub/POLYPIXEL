#ifndef GAMECONTROLLER_HXX
#define GAMECONTROLLER_HXX

#include <QObject>

class GameView;

class GameController: public QObject {
  Q_OBJECT

public:
  GameController(GameView* p_view, QObject* p_parent = nullptr);
  ~GameController() override;

  void PlayLevel(QString const& p_levelPath);
  void RestartLevel();

private:
  GameView* m_view;
};

#endif
