#ifndef PAUSEWIDGET_HXX
#define PAUSEWIDGET_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class PauseWidget: public QWidget {
  Q_OBJECT

public:
  explicit PauseWidget(QWidget* parent = nullptr);

Q_SIGNALS:
  void ResumeRequested();
  void RestartRequested();
  void LevelsRequested();

private:
  QLabel* m_pauseLabel;
  QPushButton* m_resumeButton;
  QPushButton* m_restartButton;
  QPushButton* m_levelsButton;
};

#endif
