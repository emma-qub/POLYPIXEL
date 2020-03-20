#ifndef GAMEWIDGET_HXX
#define GAMEWIDGET_HXX

#include <QWidget>

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Segment.hxx"

class QLabel;
class QPushButton;
class PolygonModel;
class PlayingWidget;
class PlayingController;

class GameWidget: public QWidget {
  Q_OBJECT

public:
  explicit GameWidget(QWidget* p_parent = nullptr);

  void InitView();

  void SetModel(PolygonModel* p_gameModel);

  void PlayLevel(QString const& p_levelPath);
  void RestartLevel();

Q_SIGNALS:
  void PauseRequested();
  void GoToMapRequested(bool p_moveToNextLevel);

private:
  QLabel* m_gameLabel;
  QPushButton* m_pauseButton;
  PlayingWidget* m_playingWidget;
  PlayingController* m_playingController;
};

#endif
