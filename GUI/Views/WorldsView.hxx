#ifndef WORLDSVIEW_HXX
#define WORLDSVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;
class QListView;
class QItemSelectionModel;
class WorldsModel;

class WorldsView: public QWidget {
  Q_OBJECT

public:
  explicit WorldsView(QWidget* parent = nullptr);

  void SetModel(WorldsModel* p_model);
  QItemSelectionModel* GetSelectionModel() const;

signals:
  void Done();
  void SelectLevelsRequested();
  void AchievementsRequested();

private:
  QLabel* m_worldsLevelLabel;
  QListView* m_worldsListView;
  QPushButton* m_achievementsButton;
  QPushButton* m_menuButton;
  WorldsModel* m_model;
};

#endif
