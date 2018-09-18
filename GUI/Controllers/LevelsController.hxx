#ifndef LEVELSCONTROLLER_HXX
#define LEVELSCONTROLLER_HXX

#include <QObject>

class LevelsModel;
class LevelsView;
class QItemSelection;

class LevelsController: public QObject {
  Q_OBJECT

public:
  explicit LevelsController(LevelsView* p_view, LevelsModel* p_model, QObject* p_parent = nullptr);

  void SetLevelsPath(QString const& p_levelsPath);

signals:
  void PlayLevelRequested(QString const& p_levelPath);

protected:
  void LevelSelected(QItemSelection const& p_selected);

private:
  LevelsView* m_view;
  LevelsModel* m_model;
};

#endif
