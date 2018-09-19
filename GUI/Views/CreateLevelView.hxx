#ifndef CREATELEVELVIEW_HXX
#define CREATELEVELVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;
class CreateLevelModel;
class CreateLevelScribblingView;

class CreateLevelView : public QWidget
{
  Q_OBJECT
public:
  explicit CreateLevelView(QWidget* parent = nullptr);

  void SetModel(CreateLevelModel* p_model);

  void DrawFromModel();
  void ClearImage();
  void Redraw();

signals:
  void TestLevelRequested();
  void Done();

public slots:

private:
  QLabel* m_createLevelLabel;
  QPushButton* m_testLevelButton;
  QPushButton* m_menuButton;
  CreateLevelModel* m_model;
  CreateLevelScribblingView* m_scribblingView;
};

#endif
