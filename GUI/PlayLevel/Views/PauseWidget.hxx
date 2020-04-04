#ifndef PAUSEWIDGET_HXX
#define PAUSEWIDGET_HXX

#include <QWidget>

class PauseWidget: public QWidget {
  Q_OBJECT

public:
  explicit PauseWidget(QWidget* p_parent = nullptr);

Q_SIGNALS:
  void ResumeRequested();
  void RestartRequested();
  void ChooseLevelRequested();

};

#endif
