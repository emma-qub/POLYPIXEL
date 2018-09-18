#include "WorldsController.hxx"

#include "GUI/Views/WorldsView.hxx"
#include "GUI/Models/WorldsModel.hxx"

#include <QItemSelectionModel>
#include <QDir>

WorldsController::WorldsController(WorldsView* p_view, WorldsModel* p_model, QObject *parent):
  QObject(parent),
  m_view(p_view),
  m_model(p_model) {

  QDir worldsDir("../Polygon/worlds");
  for (auto entry: worldsDir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot))
  {
    m_model->AppendWorld(entry);
  }
  m_view->SetModel(m_model);

  connect(m_view->GetSelectionModel(), &QItemSelectionModel::selectionChanged, this, &WorldsController::WorldSelected);
}

void WorldsController::WorldSelected(QItemSelection const& p_selected) {
  auto index = p_selected.indexes().first();
  Q_EMIT(LevelsPathChanged(index.data(WorldsModel::eLevelsPathRole).toString()));
  Q_EMIT(m_view->SelectLevelsRequested());
}
