#include "TestLevelController.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Views/TestLevelView.hxx"

TestLevelController::TestLevelController(TestLevelView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_view(p_view) {
}

void TestLevelController::SetPolygonModel(PolygonModel* p_polygonModel) {
  m_view->SetPolygonModel(p_polygonModel);
}

void TestLevelController::SetObjectModelsList(QList<ObjectModel*> const& p_objectModelList) {
  m_view->SetObjectModelsList(p_objectModelList);
}

void TestLevelController::SetLinesGoal(int p_linesGoal) {
  m_view->SetLinesGoal(p_linesGoal);
}

void TestLevelController::SetPartsGoal(int p_partsGoal) {
  m_view->SetPartsGoal(p_partsGoal);
}

void TestLevelController::SetMaxGapToWin(int p_maxGapToWin) {
  m_view->SetMaxGapToWin(p_maxGapToWin);
}

void TestLevelController::SetTolerance(int p_tolerance) {
  m_view->SetTolerance(p_tolerance);
}

void TestLevelController::PlayLevel() {
  m_view->PlayLevel();
}
