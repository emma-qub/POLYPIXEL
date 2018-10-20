#ifndef GAMEVIEW_HXX
#define GAMEVIEW_HXX

#include <QWidget>

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Segment.hxx"

class QLabel;
class QPushButton;
class PolygonModel;
class PlayingView;
class PlayingController;

class GameView: public QWidget {
  Q_OBJECT

public:
  explicit GameView(QWidget* p_parent = nullptr);

  void InitView();

  void SetModel(PolygonModel* p_gameModel);

  void PlayLevel(QString const& p_levelPath);
  void RestartLevel();

signals:
  void PauseRequested();
  void GoToMapRequested(bool p_moveToNextLevel);

private:
  QLabel* m_gameLabel;
  QPushButton* m_pauseButton;
  PlayingView* m_playingView;
  PlayingController* m_playingController;
};

#endif
