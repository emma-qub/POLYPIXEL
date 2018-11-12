#include "PlayingController.hxx"

#include "Core/Geometry/Vector.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Models/ObjectModel.hxx"
#include "GUI/Views/PlayingView.hxx"
#include "Parser/Parser.hxx"
#include "Parser/Serializer.hxx"

#include <cmath>

PlayingController::PlayingController(PlayingView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_slicer(),
  m_polygonModel(new PolygonModel(this)),
  m_gameInfo(),
  m_levelPath(""),
  m_view(p_view) {

  m_view->SetModel(m_polygonModel);

  m_objectModelsList
    << new TapeModel(this)
    << new MirrorModel(this)
    << new OneWayModel(this)
    << new PortalModel(this);

  connect(m_view, &PlayingView::Scribbling, this, &PlayingController::SetStartPoint);
  connect(m_view, &PlayingView::Moving, this, &PlayingController::MoveLine);
  connect(m_view, &PlayingView::Slicing, this, &PlayingController::SliceIt);
  connect(m_view, &PlayingView::ControlPressed, this, &PlayingController::InvertScribbleLine);
  connect(m_view, &PlayingView::ControlReleased, this, &PlayingController::InvertScribbleLine);
  connect(m_view, &PlayingView::ReplayRequested, this, &PlayingController::RestartLevel);
  connect(m_view, &PlayingView::GoToMapRequested, this, &PlayingController::GoToMapRequested);
}

void PlayingController::InitView() {
  m_view->InitView();
}

void PlayingController::SetObjectModelsList(const QList<ObjectModel*>& p_objectModelsList) {
  QList<Object*> objectsList;
  for (auto const* objectModel: p_objectModelsList) {
    objectsList << objectModel->GetObjectsList();
  }
  m_view->SetObjectsList(objectsList);

  m_objectModelsList.clear();
  m_objectModelsList << p_objectModelsList;

  m_deviationsList
    << m_objectModelsList.at(ObjectModel::eMirrorModel)->GetObjectsList()
    << m_objectModelsList.at(ObjectModel::ePortalModel)->GetObjectsList();

  m_obstaclesList
    << m_objectModelsList.at(ObjectModel::eOneWayModel)->GetObjectsList()
    << m_objectModelsList.at(ObjectModel::eTapeModel)->GetObjectsList();

  m_slicer.SetDeviationsList(m_deviationsList.toVector().toStdVector());

  p_objectModelsList.at(ObjectModel::eMirrorModel);
}

void PlayingController::PlayLevel(QString const& p_levelPath) {
  m_polygonModel->InitColor();
  m_levelPath = p_levelPath;
  OpenLevel(p_levelPath);
}

void PlayingController::RestartLevel() {
  m_view->StartLevel();
  OpenLevel(m_levelPath);
}

void PlayingController::Redraw() {
  m_view->ClearImage();
  m_view->DrawFromModel();
  m_view->DrawObjects();
}

void PlayingController::SetStartPoint(QPoint const& p_startPoint) {
  m_startPoint.SetX(static_cast<double>(p_startPoint.x()));
  m_startPoint.SetY(static_cast<double>(p_startPoint.y()));
  m_slicer.SetStartPoint(m_startPoint);
}

void PlayingController::InvertScribbleLine(QPoint const& p_cursorPosition) {
  QPoint endPoint(static_cast<int>(m_startPoint.GetX()), static_cast<int>(m_startPoint.GetY()));
  QCursor::setPos(m_view->mapToGlobal(endPoint));
  SetStartPoint(p_cursorPosition);
}

QList<ppxl::Segment> PlayingController::MoveLine(QPoint const& p_endPoint) {
  auto slicingLines = m_slicer.ComputeSlicingLines(ppxl::Point(p_endPoint.x(), p_endPoint.y()));
  auto lines = QList<ppxl::Segment>::fromVector(QVector<ppxl::Segment>::fromStdVector(slicingLines));

  Redraw();
  auto linesColor = GetLinesColor(lines);
  for (auto const& line: lines)
  {
    m_view->DrawLine(line, linesColor, Qt::DashLine);
  }

  return lines;
}

QColor PlayingController::GetLinesColor(QList<ppxl::Segment> const& p_lines) const {
  auto lineType = m_slicer.ComputeLinesType(p_lines.toVector().toStdVector());
  QColor color;

  switch (lineType) {
  case Slicer::eNoCrossing: {
    color = QColor(0x626262);
    break;
  } case Slicer::eGoodCrossing: {
    color = QColor(0x5DBE14);
    break;
  } case Slicer::eBadCrossing: {
    color = QColor(0xC81214);
    break;
  } default: {
    break;
  }
  }

  return color;
}

void PlayingController::SliceIt(QPoint const& p_endPoint) {

  if (m_slicer.SliceIt(ppxl::Point(p_endPoint.x(), p_endPoint.y()))) {
    m_polygonModel->SetPolygonsList(QList<ppxl::Polygon>::fromVector(QVector<ppxl::Polygon>::fromStdVector(m_slicer.GetPolygonsList())));
    ++m_gameInfo.m_linesCount;
    m_gameInfo.m_partsCount = m_polygonModel->GetPolygonsCount();
    UpdateViewFromGameInfo();

    CheckWinning();
  } else {
    Redraw();
  }
}

void PlayingController::UpdateViewFromGameInfo() {
  m_view->UpdateLinesCount(m_gameInfo.m_linesCount, m_gameInfo.m_linesGoal);
  m_view->UpdatePartsCount(m_gameInfo.m_partsCount, m_gameInfo.m_partsGoal);
}

void PlayingController::OpenLevel(QString const& p_levelPath) {
  if (p_levelPath.isEmpty()) {
    return;
  }

  // Parser
  Parser parser(p_levelPath);

  // Polygon model
  auto polygonsList = parser.GetPolygonsList();
  m_polygonModel->SetPolygonsList(polygonsList);
  m_slicer.SetPolygonsList(polygonsList.toVector().toStdVector());

  // Level Info
  m_gameInfo = GameInfo(0, parser.GetLinesGoal(), m_polygonModel->GetPolygonsCount(), parser.GetPartsGoal(),
    0, parser.GetStarsCount(), parser.GetMaxGapToWin(), parser.GetTolerance());

  auto mirrorModel = static_cast<MirrorModel*>(m_objectModelsList.at(ObjectModel::eMirrorModel));
  auto portalModel = static_cast<PortalModel*>(m_objectModelsList.at(ObjectModel::ePortalModel));
  auto oneWayModel = static_cast<OneWayModel*>(m_objectModelsList.at(ObjectModel::eOneWayModel));
  auto tapeModel = static_cast<TapeModel*>(m_objectModelsList.at(ObjectModel::eTapeModel));

  // Object models
  for (auto const& mirror: parser.GetMirrorsList()) {
    mirrorModel->AddMirror(mirror);
  }
  m_deviationsList.append(mirrorModel->GetObjectsList());

  for (auto const& portal: parser.GetPortalsList()) {
    portalModel->AddPortal(portal);
  }
  m_deviationsList.append(portalModel->GetObjectsList());

  //for (auto const& countdown: parser.GetCountdownsList()) {
  //  m_countdownModel->AddCountdown(countdown);
  //}
  //m_mutablesList.append(m_countdownModel->GetCountdowsList());

  //for (auto const& disposable: parser.GetDisposablesList()) {
  //  m_disposableModel->AddDisposable(disposable);
  //}
  //m_mutablesList.append(m_disposableModel->GetDisposablesList());

  //for (auto const& _switch: parser.GetSwitchesList()) {
  //  m_switchModel->AddSwitch(_switch);
  //}
  //m_mutablesList.append(m_switchModel->GetSwitchesList());

  //for (auto const& transfer: parser.GetTransfersList()) {
  //  m_transferModel->AddTransfer(transfer);
  //}
  //m_mutablesList.append(m_transferModel->GetTransfersList());

  for (auto const& oneWay: parser.GetOneWaysList()) {
    oneWayModel->AddOneWay(oneWay);
  }
  m_obstaclesList.append(oneWayModel->GetObjectsList());

  for (auto const& tape: parser.GetTapesList()) {
    tapeModel->AddTape(tape);
  }
  m_obstaclesList.append(tapeModel->GetObjectsList());

  m_objectsList << m_deviationsList << m_mutablesList << m_obstaclesList;
  m_view->SetObjectsList(m_objectsList);

  m_slicer.SetDeviationsList(m_deviationsList.toVector().toStdVector());
  m_slicer.SetMutablesList(m_mutablesList.toVector().toStdVector());
  m_slicer.SetObstaclesList(m_obstaclesList.toVector().toStdVector());

  m_slicer.InitTotalOrientedArea();

  StartLevel();
}

void PlayingController::StartLevel() {
  m_view->StartLevel();
  m_view->ClearImage();

  UpdateViewFromGameInfo();

  Redraw();

  connect(m_polygonModel, &PolygonModel::PolygonListChanged, this, &PlayingController::Redraw);
}

int PlayingController::ComputeStarsCount(double p_gap) {
  int starsCount = 0;

  if (m_gameInfo.m_partsCount == m_gameInfo.m_partsGoal && m_gameInfo.m_linesCount == m_gameInfo.m_linesGoal) {
    auto gapRatio = p_gap / static_cast<double>(m_gameInfo.m_maxGapToWin);
    starsCount = std::max(starsCount, 4 - static_cast<int>(std::ceil(3. * gapRatio)));
    if (starsCount >= 3 && static_cast<int>(10. * p_gap / static_cast<double>(m_gameInfo.m_partsGoal)) <= m_gameInfo.m_tolerance) {
      starsCount = 4;
    }
  }

  return starsCount;
}

void PlayingController::CheckWinning() {
  if (m_gameInfo.m_linesCount >= m_gameInfo.m_linesGoal || m_gameInfo.m_partsCount >= m_gameInfo.m_partsGoal) {
    double minArea;
    double maxArea;
    auto globalBarycenter = m_slicer.ComputeGlobalBarycenter();
    auto shiftVectorsList = QList<ppxl::Vector>::fromVector(QVector<ppxl::Vector>::fromStdVector(m_slicer.ComputeShiftVectorsList(globalBarycenter)));
    auto areasList = QList<double>::fromVector(QVector<double>::fromStdVector(m_slicer.ComputeAreas(minArea, maxArea)));
    double gap = std::abs(maxArea - minArea);

    auto starsCount = ComputeStarsCount(gap);
    m_gameInfo.m_stars = starsCount;
    UpdateStarsMax(starsCount);

    if (starsCount == 4) {
      areasList.clear();
      double area = 100./m_gameInfo.m_partsGoal;
      for (int k = 0; k < m_gameInfo.m_partsCount; ++k) {
        areasList << area;
      }
    }

    m_view->SetEndLevelInfo(m_gameInfo.m_linesCount, m_gameInfo.m_linesGoal, m_gameInfo.m_partsCount, m_gameInfo.m_partsGoal, m_gameInfo.m_stars);
    m_view->SetAreasData(areasList, shiftVectorsList, globalBarycenter);

    m_view->AnimatePolygons();
    m_view->EndLevel();

    disconnect(m_polygonModel, &PolygonModel::PolygonListChanged, this, &PlayingController::Redraw);
  }
}

void PlayingController::UpdateStarsMax(int starsMaxCount) {
  Parser parser(m_levelPath);
  Serializer serializer(m_levelPath);
  int currentStarsCount = parser.GetStarsCount();

  if (currentStarsCount < starsMaxCount) {
    serializer.SetPolygonsList(parser.GetPolygonsList());
    serializer.SetLinesGoal(m_gameInfo.m_linesGoal);
    serializer.SetPartsGoal(m_gameInfo.m_partsGoal);
    serializer.SetMaxGapToWin(m_gameInfo.m_maxGapToWin);
    serializer.SetTolerance(m_gameInfo.m_tolerance);
    serializer.SetStarsCount(starsMaxCount);
    serializer.WriteXML();
  }
}

void PlayingController::DisplayGameOver() {
  m_view->DisplayGameOver();
}
