#ifndef MAINMENUVIEW_HXX
#define MAINMENUVIEW_HXX

#include <QWidget>

class QLabel;
class QtMaterialContainedButton;

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
  QtMaterialContainedButton* m_worldsButton;
  QtMaterialContainedButton* m_createLevelButton;
  QtMaterialContainedButton* m_optionsButton;
};

#endif
