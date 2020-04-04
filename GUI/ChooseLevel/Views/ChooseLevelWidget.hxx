#ifndef CHOOSELEVELWIDGET_HXX
#define CHOOSELEVELWIDGET_HXX

#include <QWidget>

class ChooseLevelWidget: public QWidget {
  Q_OBJECT

public:
  explicit ChooseLevelWidget(QWidget* p_parent = nullptr);

  void InitView();

Q_SIGNALS:
  void PlayLevelRequested(QString const& p_levelFileName);

};

#endif
