#ifndef MAINMENUVIEW_HXX
#define MAINMENUVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class MainMenuView : public QWidget {
  Q_OBJECT

public:
  explicit MainMenuView(QWidget* parent = nullptr);

signals:
  void PlayRequested();
  void CreateLevelRequested();
  void OptionsRequested();

protected:
  void paintEvent(QPaintEvent* p_event) override;

private:
  QLabel* m_menuLabel;
  QPushButton* m_worldsButton;
  QPushButton* m_createLevelButton;
  QPushButton* m_optionsButton;
};

#endif
