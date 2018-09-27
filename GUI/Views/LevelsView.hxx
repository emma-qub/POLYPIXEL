#ifndef LEVELSVIEW_HXX
#define LEVELSVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;
class QListView;
class QItemSelectionModel;
class LevelsModel;

class LevelsView: public QWidget {
  Q_OBJECT

public:
  explicit LevelsView(QWidget* parent = nullptr);

  void SetModel(LevelsModel* p_model);

  QItemSelectionModel* GetSelectionModel() const;

signals:
  void PlayLevelRequested();
  void WorldsSelectionRequested();
  void Done();

private:
  QLabel* m_levelsLabel;
  QListView* m_levelsListView;
  QPushButton* m_selectWorldButton;
  QPushButton* m_menuButton;
  LevelsModel* m_model;
};

#endif
