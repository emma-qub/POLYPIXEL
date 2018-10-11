#include "PlayingController.hxx"

#include "Core/Vector.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Models/ObjectModel.hxx"
#include "GUI/Views/PlayingView.hxx"
#include "Parser/Parser.hxx"
#include "Parser/Serializer.hxx"

#include <cmath>

PlayingController::PlayingController(PlayingView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_polygonModel(new PolygonModel(this)),
  m_mirrorModel(new MirrorModel(this)),
  m_portalModel(new PortalModel(this)),
  m_countdownModel(new CountdownModel(this)),
  m_disposableModel(new DisposableModel(this)),
  m_switchModel(new SwitchModel(this)),
  m_transferModel(new TransferModel(this)),
  m_oneWayModel(new OneWayModel(this)),
  m_tapeModel(new TapeModel(this)),
  m_gameInfo(),
  m_levelPath(""),
  m_orientedAreaTotal(0.0),
  m_view(p_view) {

  m_view->SetModel(m_polygonModel);

  connect(m_view, &PlayingView::Scribbling, this, &PlayingController::SetStartPoint);
  connect(m_view, &PlayingView::Moving, this, &PlayingController::ComputeSlicingLines);
  connect(m_view, &PlayingView::Slicing, this, &PlayingController::SliceIt);
  connect(m_view, &PlayingView::ControlPressed, this, &PlayingController::InvertScribbleLine);
  connect(m_view, &PlayingView::ControlReleased, this, &PlayingController::InvertScribbleLine);
  connect(m_view, &PlayingView::ReplayRequested, this, &PlayingController::RestartLevel);
  connect(m_view, &PlayingView::GoToMapRequested, this, &PlayingController::GoToMapRequested);
}

void PlayingController::InitView() {
  m_view->InitView();
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
}

void PlayingController::InvertScribbleLine(QPoint const& p_cursorPosition) {
  QPoint endPoint(static_cast<int>(m_startPoint.GetX()), static_cast<int>(m_startPoint.GetY()));
  QCursor::setPos(m_view->mapToGlobal(endPoint));
  SetStartPoint(p_cursorPosition);
  ComputeSlicingLines(endPoint);
}

QList<ppxl::Segment> PlayingController::ComputeSlicingLines(QPoint const& p_endPoint) {
  QList<ppxl::Segment> lines;

  ppxl::Point endPoint(static_cast<double>(p_endPoint.x()), static_cast<double>(p_endPoint.y()));
  ppxl::Segment scribbledLine(m_startPoint, endPoint);
  ComputeDeviateLines(-1, scribbledLine, lines);

  Redraw();
  auto linesColor = GetLinesColor(lines);
  for (auto const& line: lines)
  {
    m_view->DrawLine(line, linesColor, Qt::DashLine);
  }

  return lines;
}

QColor PlayingController::GetLinesColor(QList<ppxl::Segment> const& p_lines) const {
  auto lineType = ComputeLinesType(p_lines);
  QColor color;

  switch (lineType) {
  case eNoCrossing: {
    color = QColor(0x626262);
    break;
  } case eGoodCrossing: {
    color = QColor(0x5DBE14);
    break;
  } case eBadCrossing: {
    color = QColor(0xC81214);
    break;
  } default: {
    break;
  }
  }

  return color;
}

PlayingController::LineType PlayingController::ComputeLinesType(QList<ppxl::Segment> const& p_lines) const {
  bool noCrossing = false;
  bool goodCrossing = false;
  bool badCrossing = false;

  auto polygonList = m_polygonModel->GetPolygonsList();
  for (ppxl::Segment const& line: p_lines) {
    for (auto const& polygon: polygonList) {
      if (!polygon->IsCrossing(line) && !polygon->IsPointInside(line.GetA())) {
        noCrossing = true;
      } else if (polygon->IsGoodSegment(line)) {
        goodCrossing = true;
      } else {
        badCrossing = true;
      }
    }

    for (auto const* obstacle: m_obstaclesList) {
      if (static_cast<Obstacle const*>(obstacle)->Crossing(line)) {
        badCrossing = true;
      }
    }
  }

  if (badCrossing) {
    return eBadCrossing;
  } else if (goodCrossing) {
    return eGoodCrossing;
  } else if (noCrossing) {
    return eNoCrossing;
  } else {
    return eUnknownCrossing;
  }
}

void PlayingController::SliceIt(QPoint const& p_endPoint) {
  auto lines = ComputeSlicingLines(p_endPoint);

  if (ComputeLinesType(lines) == eGoodCrossing) {
    QList<ppxl::Polygon> newPolygonList;

    for (ppxl::Segment const& line: lines) {
      // Browse every polygon and slice it!
      ComputeNewPolygonList(newPolygonList, line);
      m_polygonModel->SetPolygonsList(newPolygonList);
    }

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


void PlayingController::ComputeNewPolygonList(QList<ppxl::Polygon>& p_newPolygonList, ppxl::Segment const& p_line) const {
  for (auto const* polygon: m_polygonModel->GetPolygonsList()) {
    std::vector<ppxl::Point*> globalVertices;
    std::vector<ppxl::Point*> intersections;
    GetVerticesAndIntersections(p_line, *polygon, globalVertices, intersections);

    std::vector<ppxl::Point> newVertices;
    std::vector<std::pair<ppxl::Point*, ppxl::Point*>> cuttingSegments = GetCuttingSegments(intersections);

    while (StillHasBaseVertices(globalVertices, intersections)) {
      // We really don't want the first point to be an intersection. Trust me.
      ppxl::Point* p = globalVertices.at(0);
      while (std::find(intersections.begin(), intersections.end(), p) != intersections.end()) {
        globalVertices.erase(globalVertices.begin());
        globalVertices.push_back(p);
        p = globalVertices.at(0);
      }
      std::vector<ppxl::Point*> globalVerticesCopy(globalVertices);

      bool lookingForOtherBound = false;
      ppxl::Point* otherBound = nullptr;
      for (ppxl::Point* currVerrtex: globalVerticesCopy) {
        if (lookingForOtherBound) {
          if (otherBound == currVerrtex) {
            newVertices.push_back(*currVerrtex);
            lookingForOtherBound = false;
          }
        } else {
          if (std::find(intersections.begin(), intersections.end(), currVerrtex) != intersections.end()) {
            // If the intersection is not equal to the last point, we add it
            if (newVertices.size() > 0 && (std::find(newVertices.begin(), newVertices.end(), *currVerrtex) == newVertices.end())) {
              newVertices.push_back(*currVerrtex);
            }
            otherBound = GetOtherBound(currVerrtex, cuttingSegments);
            lookingForOtherBound = true;
          } else {
            newVertices.push_back(*currVerrtex);
            auto it = std::find(globalVertices.begin(), globalVertices.end(), currVerrtex);
            assert(it != globalVertices.end());
            globalVertices.erase(it);
          }
        }
      }
      globalVerticesCopy.clear();
      globalVerticesCopy = globalVertices;

      ppxl::Polygon newPolygon(newVertices);
      // Don't add the new polygon if its area is less than 0.1% of the total area.
      // This allows users to draw several lines that pass near a point,
      // but not exactly on this point, since it's quite difficult to achieve.
      if (std::round(10.0*newPolygon.OrientedArea() * 100.0 / m_orientedAreaTotal)/10.0 >= 0.1) {
        p_newPolygonList << newPolygon;
      }

      newVertices.clear();
    }
  }
}

void PlayingController::GetVerticesAndIntersections(ppxl::Segment const& line, ppxl::Polygon const& polygon,
  std::vector<ppxl::Point*>& globalVertices, std::vector<ppxl::Point*>& intersections) const {

  std::vector<ppxl::Point> baseVertices = polygon.GetVertices();

  for (unsigned int k = 0; k < baseVertices.size(); ++k) {
    auto A = new ppxl::Point(baseVertices.at(k));
    auto B = new ppxl::Point(baseVertices.at((k+1)%baseVertices.size()));
    ppxl::Segment AB(*A, *B);

    globalVertices.push_back(A);

    ppxl::Segment::Intersection intersectionType = AB.ComputeIntersection(line);
    switch (intersectionType) {
    case ppxl::Segment::Regular:
    {
      auto intersection = new ppxl::Point(ppxl::Segment::IntersectionPoint(AB, line));
      globalVertices.push_back(intersection);
      intersections.push_back(intersection);
      break;
    }
    case ppxl::Segment::FirstVertexRegular:
    {
      auto intersection = new ppxl::Point(baseVertices.at(k));
      globalVertices.push_back(intersection);
      intersections.push_back(intersection);
      break;
    }
    default:
      break;
    }
  }

  std::sort(intersections.begin(), intersections.end(), ComparePoints);

  CleanIntersections(polygon, intersections);
}

void PlayingController::CleanIntersections(ppxl::Polygon const& polygon, std::vector<ppxl::Point*>& intersections) const {
  if (intersections.size() < 2) {
    intersections.clear();
    return;
  }

  std::vector<ppxl::Point*> realIntersection;
  bool inside = false;

  for (unsigned int k = 0; k < intersections.size()-1; ++k) {
    ppxl::Segment AB(*intersections.at(k), *intersections.at(k+1));
    if (polygon.IsPointInside(AB.GetCenter())) {
      if (!inside) {
        realIntersection.push_back(intersections.at(k));
        inside = true;
      }
    } else if (!polygon.IsPointInside(AB.GetCenter())) {
      if (inside) {
        realIntersection.push_back(intersections.at(k));
        inside = false;
      }
    }
  }

  // Handle last vertex
  if (inside)
  {
    realIntersection.push_back(intersections.at(intersections.size()-1));
  }

  intersections.clear();
  intersections = realIntersection;
}

std::vector<std::pair<ppxl::Point*, ppxl::Point*>> PlayingController::GetCuttingSegments(std::vector<ppxl::Point*> const& intersections) const {
  assert(intersections.size()%2 == 0);

  std::vector<std::pair<ppxl::Point*, ppxl::Point*>> cuttingSegments;
  if (intersections.size() == 0) {
    return cuttingSegments;
  }

  for (unsigned int k = 0; k < intersections.size()-1; k += 2) {
    ppxl::Segment AB(*intersections.at(k), *intersections.at(k+1));
    cuttingSegments.push_back(std::pair<ppxl::Point*, ppxl::Point*>(intersections.at(k), intersections.at((k+1)%intersections.size())));
  }
  return cuttingSegments;
}

bool PlayingController::StillHasBaseVertices(std::vector<ppxl::Point*> const& globalVertices, std::vector<ppxl::Point*> const& intersections) const {
  for (ppxl::Point* p: globalVertices) {
    if (std::find(intersections.begin(), intersections.end(), p) == intersections.end()) {
      return true;
    }
  }

  return false;
}

ppxl::Point* PlayingController::GetOtherBound(ppxl::Point const* intersection, std::vector<std::pair<ppxl::Point*, ppxl::Point*>> const& cuttingSegments) const {
  for (std::pair<ppxl::Point*, ppxl::Point*> const& cuttingSegment: cuttingSegments) {
    if (cuttingSegment.first == intersection) {
      return cuttingSegment.second;
    } else if (cuttingSegment.second == intersection) {
      return cuttingSegment.first;
    }
  }

  return nullptr;
}

void PlayingController::OpenLevel(QString const& p_levelPath) {
  if (p_levelPath.isEmpty()) {
    return;
  }

  // Parser
  Parser parser(p_levelPath);
  
  // Polygon model
  m_polygonModel->SetPolygonsList(parser.GetPolygonsList());
  
  // Level Info
  m_gameInfo = GameInfo(0, parser.GetLinesGoal(), m_polygonModel->GetPolygonsCount(), parser.GetPartsGoal(),
    0, parser.GetStarsCount(), parser.GetMaxGapToWin(), parser.GetTolerance());

  // Object models
  for (auto const& mirror: parser.GetMirrorsList()) {
    m_mirrorModel->AddMirror(mirror);
  }
  m_deviationsList.append(m_mirrorModel->GetMirrorsList());

  for (auto const& portal: parser.GetPortalsList()) {
    m_portalModel->AddPortal(portal);
  }
  m_deviationsList.append(m_portalModel->GetPortalsList());

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
    m_oneWayModel->AddOneWay(oneWay);
  }
  m_obstaclesList.append(m_oneWayModel->GetOneWaysList());

  for (auto const& tape: parser.GetTapesList()) {
    m_tapeModel->AddTape(tape);
  }
  m_obstaclesList.append(m_tapeModel->GetTapesList());

  m_objectsList << m_deviationsList << m_mutablesList << m_obstaclesList;
  m_view->SetObjectsList(m_objectsList);

  StartLevel();
}

void PlayingController::StartLevel() {
  m_view->StartLevel();
  m_view->ClearImage();
  m_orientedAreaTotal = 0.;

  UpdateViewFromGameInfo();

  for (auto const* polygon: m_polygonModel->GetPolygonsList()) {
    m_orientedAreaTotal += polygon->OrientedArea();
  }
  Redraw();

  connect(m_polygonModel, &PolygonModel::PolygonListChanged, this, &PlayingController::Redraw);
}

QList<double> PlayingController::ComputeAreas(double& p_minArea, double& p_maxArea) {
  QList<double> orientedAreas;
  p_minArea = 100.;
  p_maxArea = 0.;

  double areaCumul = 0.;
  auto polygonsList = m_polygonModel->GetPolygonsList();
  for (int row = 0; row < polygonsList.size(); ++row) {
    auto polygon = polygonsList.at(row);
    double currArea = 0.;

    if (row == polygonsList.size()-1) {
      currArea = 100. - areaCumul;
    } else {
      currArea = ComputePolygonPercentageArea(*polygon);
      areaCumul += currArea;
    }

    orientedAreas << currArea;
    p_minArea = std::min(currArea, p_minArea);
    p_maxArea = std::max(currArea, p_maxArea);
  }

  return orientedAreas;
}

QList<ppxl::Vector> PlayingController::ComputeShiftVectorsList(ppxl::Point const& p_globalBarycenter) {
  QList<ppxl::Vector> shiftVectors;

  for (auto const* polygon: m_polygonModel->GetPolygonsList()) {
    ppxl::Vector currShift(p_globalBarycenter, polygon->Barycenter());
    double currShiftLength = currShift.Norm();
    currShift.Normalize();
    currShift *= 0.2*currShiftLength;
    shiftVectors << currShift;
  }

  return shiftVectors;
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
    auto globalBarycenter = ComputeGlobalBarycenter();
    auto shiftVectorsList = ComputeShiftVectorsList(globalBarycenter);
    auto areasList = ComputeAreas(minArea, maxArea);
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

ppxl::Point PlayingController::ComputeGlobalBarycenter() const {
  ppxl::Point globalBarycenter;
  unsigned int polygonCount = 0;

  for (auto const* polygon: m_polygonModel->GetPolygonsList()) {
    globalBarycenter += polygon->Barycenter();
    ++polygonCount;
  }

  assert(polygonCount > 0);
  globalBarycenter /= polygonCount;

  return globalBarycenter;
}

double PlayingController::ComputePolygonPercentageArea(ppxl::Polygon const& polygon) const {
  return qRound(10.*polygon.OrientedArea() * 100. / m_orientedAreaTotal) / 10.;
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

/*
void PlayingController::undoSliceIt() {
  if (m_polygonListPerTurn.size() > 1) {
    m_polygonListPerTurn.pop_back();
    m_model->setPolygonList(m_polygonListPerTurn.last());
    m_gameInfo->setLinesCount(m_gameInfo.m_linesCount-1);
    m_gameInfo->setPartsCount(m_model->getPolygonsCount());
    m_levelRunning = true;

    emit update();
  }
}

void PlayingController::clearGame() {
  m_model->clearPolygons();
  m_model->clearTapes();
  m_model->clearMirrors();
  m_model->clearPortals();
  m_model->clearRefractors();
  m_model->clearDeviations();
  m_gameInfo->setLinesCount(0);
  m_gameInfo->setPartsCount(0);
  m_gameInfo->setPartsGoal(0);
  m_gameInfo->setStars(-1);
  m_gameInfo->setStarsMax(-1);
  m_orientedAreaTotal = 0.0;
  m_maxGapToWin = 0.0;
  m_levelRunning = false;
  m_polygonListPerTurn.clear();

  emit update();
}
*/

Deviation* PlayingController::GetNearestDeviation(ppxl::Segment const& line) const {
  double minDist = -1.;
  Deviation* nearestDeviation = nullptr;

  for (auto* deviation: m_deviationsList) {
    auto deviationCast = static_cast<Deviation*>(deviation);
    QList<ppxl::Segment> deviateLines = deviationCast->DeviateLine(line);
    // If there is at least one reflected line
    if (deviateLines.size() > 1) {
      if (minDist < 0.) {
        deviateLines.at(0).Length();
      } else {
        qMin(minDist, deviateLines.at(0).Length());
      }
      nearestDeviation = deviationCast;
    }
  }

  return nearestDeviation;
}

void PlayingController::ComputeDeviateLines(double firstLineLength, ppxl::Segment const& line, QList<ppxl::Segment>& lines) const {
  Deviation* nearestDeviation = GetNearestDeviation(line);

  if (nearestDeviation) {
    QList<ppxl::Segment> deviateLines = nearestDeviation->DeviateLine(line);
    // Init firstLineLength
    if (firstLineLength < 0.) {
      firstLineLength = deviateLines.at(0).Length();
    }

    // At least two line has to be here: the line drawn and its reflexion
    assert(deviateLines.size() > 1);

    // Erase previous line, since it go through current mirror
    if (lines.size() > 1) {
      lines.erase(lines.end()-1);
    }

    // Get the mirrorLined the length of the firstLine
    ppxl::Segment deviateLine = deviateLines.at(1);
    ppxl::Point A = deviateLine.GetA();
    ppxl::Point B = deviateLine.GetB();
    ppxl::Vector lu = firstLineLength*ppxl::Vector(A, B).Normalize();
    ppxl::Point BB(lu.GetX(), lu.GetY());
    deviateLine.setB(A + BB);

    // Push line and its reflexion
    lines << deviateLines.at(0);
    lines << deviateLine;

    ComputeDeviateLines(firstLineLength, deviateLine, lines);
  } else {
    lines << line;
  }
}


bool ComparePoints(const ppxl::Point* A, const ppxl::Point* B) {
  return *A < *B;
}
