#ifndef GAMECONTROLLER_HXX
#define GAMECONTROLLER_HXX

#include <QObject>

class GameWidget;

class GameController: public QObject {
  Q_OBJECT

public:
  GameController(GameWidget* p_view, QObject* p_parent = nullptr);
  ~GameController() override;

  void PlayLevel(QString const& p_levelPath);
  void RestartLevel();
  void Update();

private:
  GameWidget* m_view;
};

#endif
