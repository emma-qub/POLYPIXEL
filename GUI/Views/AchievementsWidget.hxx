#ifndef ACHIEVEMENTSWIDGET_HXX
#define ACHIEVEMENTSWIDGET_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class AchievementsWidget: public QWidget {
  Q_OBJECT

public:
  explicit AchievementsWidget(QWidget* parent = nullptr);

Q_SIGNALS:
  void Done();

private:
  QLabel* m_achievementsLabel;
  QPushButton* m_worldsButton;
};

#endif
