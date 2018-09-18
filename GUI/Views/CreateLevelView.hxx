#ifndef CREATELEVELVIEW_HXX
#define CREATELEVELVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class CreateLevelView : public QWidget
{
  Q_OBJECT
public:
  explicit CreateLevelView(QWidget* parent = nullptr);

signals:
  void TestLevelRequested();
  void Done();

public slots:

private:
  QLabel* m_createLevelLabel;
  QPushButton* m_testLevelButton;
  QPushButton* m_menuButton;
};

#endif
