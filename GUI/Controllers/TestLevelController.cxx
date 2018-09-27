#include "TestLevelController.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Views/TestLevelView.hxx"

TestLevelController::TestLevelController(TestLevelView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_view(p_view) {
}

void TestLevelController::SetModel(PolygonModel* p_model) {
  m_view->SetModel(p_model);
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
