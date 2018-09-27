#include "TestingController.hxx"

#include "GUI/Views/TestingView.hxx"
#include "GUI/Models/PolygonModel.hxx"

TestingController::TestingController(TestingView* p_view, QObject* p_parent):
  PlayingController(p_view, p_parent) {

  m_view = static_cast<TestingView*>(p_view);
}

void TestingController::SetPolygonsItem(PolygonModel* p_model) {
  QList<ppxl::Polygon> polygonsList;
  m_model->InitColor();

  for (auto* polygon: p_model->GetPolygonsList()) {
    polygonsList << ppxl::Polygon(*polygon);
  }

  m_model->SetPolygonsList(polygonsList);
  m_view->SetModel(m_model);
}

void TestingController::SetLinesGoal(int p_linesGoal) {
  m_gameInfo.m_linesCount = 0;
  m_gameInfo.m_linesGoal = p_linesGoal;
}

void TestingController::SetPartsGoal(int p_partsGoal) {
  m_gameInfo.m_partsCount = 0;
  m_gameInfo.m_partsGoal = p_partsGoal;
}

void TestingController::SetMaxGapToWin(int p_maxGapToWin) {
  m_gameInfo.m_maxGapToWin = p_maxGapToWin;
}

void TestingController::SetTolerance(int p_tolerance) {
  m_gameInfo.m_tolerance = p_tolerance;
}

void TestingController::PlayLevel() {
  m_levelPath = "";
  m_orientedAreaTotal = 0.;
  for (auto const* polygon: m_model->GetPolygonsList()) {
    m_orientedAreaTotal += polygon->OrientedArea();
  }
  UpdateViewFromGameInfo();
  m_view->StartLevel();
  Redraw();

  connect(m_model, &PolygonModel::PolygonListChanged, this, &TestingController::Redraw);
}

void TestingController::Redraw() {
  m_view->ClearImage();
  m_view->DrawFromModel();
}

void TestingController::CheckWinning() {
  if (m_gameInfo.m_linesCount >= m_gameInfo.m_linesGoal || m_gameInfo.m_partsCount >= m_gameInfo.m_partsGoal) {
    double minArea;
    double maxArea;
    auto areasList = ComputeAreas(minArea, maxArea);
    double gap = std::abs(maxArea - minArea);

    auto starsCount = ComputeStarsCount(gap);
    m_gameInfo.m_stars = starsCount;

    m_view->DrawAreas(areasList);
    m_view->EndLevel();

    disconnect(m_model, &PolygonModel::PolygonListChanged, this, &TestingController::Redraw);
  }
}
