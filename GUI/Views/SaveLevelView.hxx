#ifndef SAVELEVELVIEW_HXX
#define SAVELEVELVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class SaveLevelView : public QWidget
{
  Q_OBJECT
public:
  explicit SaveLevelView(QWidget* parent = nullptr);

signals:
  void Done();
  void CreateNewLevelRequested();

public slots:

private:
  QLabel* m_saveLevelLabel;
  QPushButton* m_menuButton;
  QPushButton* m_createNewLevelButton;
};

#endif
