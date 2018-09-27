#ifndef WORLDSCONTROLLER_HXX
#define WORLDSCONTROLLER_HXX

#include <QObject>

class WorldsView;
class WorldsModel;
class QItemSelection;

class WorldsController: public QObject {
  Q_OBJECT

public:
  explicit WorldsController(WorldsView* p_view, WorldsModel* p_model, QObject* p_parent = nullptr);

signals:
  void LevelsPathChanged(QString const& p_levelsPath);

protected:
  void WorldSelected(QItemSelection const& p_selected);

private:
  WorldsView* m_view;
  WorldsModel* m_model;
};

#endif
