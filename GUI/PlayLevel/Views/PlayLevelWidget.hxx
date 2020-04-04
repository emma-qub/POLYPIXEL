#ifndef PLAYLEVELWIDGET_HXX
#define PLAYLEVELWIDGET_HXX

#include <QWidget>

class PlayLevelWidget: public QWidget {
  Q_OBJECT

public:
  explicit PlayLevelWidget(QWidget* p_parent = nullptr);

  void InitView();

  void PlayLevel(QString const& p_currentLevel);

Q_SIGNALS:
  void PauseRequested();
  void ChooseLevelRequested();
};

#endif
