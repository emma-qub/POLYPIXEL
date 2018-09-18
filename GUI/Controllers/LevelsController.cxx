#include "LevelsController.hxx"

#include "GUI/Views/LevelsView.hxx"
#include "GUI/Models/LevelsModel.hxx"

#include <QItemSelectionModel>
#include <QDir>


#include <QDebug>


LevelsController::LevelsController(LevelsView* p_view, LevelsModel* p_model, QObject* p_parent):
  QObject(p_parent),
  m_view(p_view),
  m_model(p_model)
{
  m_view->SetModel(m_model);

  connect(m_view->GetSelectionModel(), &QItemSelectionModel::selectionChanged, this, &LevelsController::LevelSelected);
}

void LevelsController::SetLevelsPath(QString const& p_levelsPath) {
  QDir levelsdDir(p_levelsPath);
  m_model->clear();
  for (auto entry: levelsdDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
  {
    m_model->AppendLevel(entry);
  }
}

void LevelsController::LevelSelected(QItemSelection const& p_selected) {
  auto index = p_selected.indexes().first();
  Q_EMIT(m_view->PlayLevelRequested());
  Q_EMIT(PlayLevelRequested(index.data(LevelsModel::eLevelPathRole).toString()));
}
