#ifndef ACHIEVEMENTSVIEW_HXX
#define ACHIEVEMENTSVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class AchievementsView: public QWidget {
  Q_OBJECT

public:
  explicit AchievementsView(QWidget* parent = nullptr);

signals:
  void Done();

private:
  QLabel* m_achievementsLabel;
  QPushButton* m_worldsButton;
};

#endif
