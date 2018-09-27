#ifndef PAUSEVIEW_HXX
#define PAUSEVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class PauseView : public QWidget {
  Q_OBJECT

public:
  explicit PauseView(QWidget* parent = nullptr);

signals:
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
