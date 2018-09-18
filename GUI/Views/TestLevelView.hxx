#ifndef TESTLEVELVIEW_HXX
#define TESTLEVELVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class TestLevelView : public QWidget
{
  Q_OBJECT
public:
  explicit TestLevelView(QWidget* parent = nullptr);

signals:
  void Done();
  void AmendLevelRequested();
  void SaveLevelRequested();

public slots:

private:
  QLabel* m_testLevelLabel;
  QPushButton* m_menuButton;
  QPushButton* m_amendLevelButton;
  QPushButton* m_saveLevelButton;
};

#endif
