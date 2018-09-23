#include "Core/Vector.hxx"
#include "GameController.hxx"
#include "GUI/Views/GameView.hxx"
#include "GUI/Models/PolygonModel.hxx"
#include "Parser/Parser.hxx"
#include "Parser/Serializer.hxx"

#include <QMessageBox>
#include <QDoubleValidator>
#include <QMap>
#include <QDebug>

#include <algorithm>
#include <cmath>
#include <cassert>

GameController::GameController(PolygonModel* p_model, GameView* p_view, QObject* p_parent):
  QObject(p_parent),
  m_model(p_model),
  m_view(p_view),
  m_orientedAreaTotal(0.0),
  m_maxGapToWin(10),
  m_levelPath(""),
  m_gameInfo() {

  m_view->SetModel(m_model);
  connect(m_model, &PolygonModel::PolygonListChanged, this, &GameController::Redraw);
  connect(m_view, &GameView::Scribbling, this, &GameController::SetStartPoint);
  connect(m_view, &GameView::Moving, this, &GameController::ComputeSlicingLines);
  connect(m_view, &GameView::Slicing, this, &GameController::SliceIt);
}

GameController::~GameController() = default;

void GameController::PlayLevel(QString const& p_levelPath) {
  m_levelPath = p_levelPath;
  OpenLevel(p_levelPath);
}

void GameController::RestartLevel() {
  OpenLevel(m_levelPath);
}

void GameController::Redraw() {
  m_view->ClearImage();
  m_view->DrawFromModel();
}

void GameController::SetStartPoint(QPoint const& p_startPoint) {
  m_startPoint.SetX(static_cast<double>(p_startPoint.x()));
  m_startPoint.SetY(static_cast<double>(p_startPoint.y()));
}

QList<ppxl::Segment> GameController::ComputeSlicingLines(QPoint const& p_endPoint) {
  QList<ppxl::Segment> lines;

  ppxl::Point endPoint(static_cast<double>(p_endPoint.x()), static_cast<double>(p_endPoint.y()));
  lines << ppxl::Segment(m_startPoint, endPoint);

  Redraw();
  auto linesColor = GetLinesColor(lines);
  for (auto const& line: lines)
  {
    m_view->DrawLine(line, linesColor, Qt::DashLine);
  }

  return lines;
}

QColor GameController::GetLinesColor(QList<ppxl::Segment> const& p_lines) const {
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

GameController::LineType GameController::ComputeLinesType(QList<ppxl::Segment> const& p_lines) const {
  bool noCrossing = false;
  bool goodCrossing = false;
  bool badCrossing = false;

  auto polygonList = m_model->GetPolygonsList();
  for (const ppxl::Segment& line: p_lines) {
    for (auto const& polygon: polygonList) {
      if (!polygon->IsCrossing(line) && !polygon->IsPointInside(line.GetA())) {
        noCrossing = true;
      } else if (polygon->IsGoodSegment(line)) {
        goodCrossing = true;
      } else {
        badCrossing = true;
      }
    }
    // Handle tape here
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

void GameController::SliceIt(QPoint const& p_endPoint) {
  auto lines = ComputeSlicingLines(p_endPoint);

  if (ComputeLinesType(lines) == eGoodCrossing) {
    QList<ppxl::Polygon> newPolygonList;

    for (ppxl::Segment const& line: lines) {
      // Browse every polygon and slice it!
      ComputeNewPolygonList(newPolygonList, line);
      m_model->SetPolygonsList(newPolygonList);
    }

    ++m_gameInfo.m_linesCount;
    m_gameInfo.m_partsCount = m_model->GetPolygonsCount();
    UpdateViewFromGameInfo();

    CheckWinning();
  } else {
    Redraw();
  }
}

void GameController::UpdateViewFromGameInfo() {
  m_view->UpdateLinesCount(m_gameInfo.m_linesCount, m_gameInfo.m_linesGoal);
  m_view->UpdatePartsCount(m_gameInfo.m_partsCount, m_gameInfo.m_partsGoal);
}


void GameController::ComputeNewPolygonList(QList<ppxl::Polygon>& p_newPolygonList, ppxl::Segment const& p_line) const {
  for (auto const* polygon: m_model->GetPolygonsList()) {
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

void GameController::GetVerticesAndIntersections(ppxl::Segment const& line, ppxl::Polygon const& polygon,
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

void GameController::CleanIntersections(ppxl::Polygon const& polygon, std::vector<ppxl::Point*>& intersections) const {
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

std::vector<std::pair<ppxl::Point*, ppxl::Point*>> GameController::GetCuttingSegments(std::vector<ppxl::Point*> const& intersections) const {
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

bool GameController::StillHasBaseVertices(std::vector<ppxl::Point*> const& globalVertices, std::vector<ppxl::Point*> const& intersections) const {
  for (ppxl::Point* p: globalVertices) {
    if (std::find(intersections.begin(), intersections.end(), p) == intersections.end()) {
      return true;
    }
  }

  return false;
}

ppxl::Point* GameController::GetOtherBound(ppxl::Point const* intersection, std::vector<std::pair<ppxl::Point*, ppxl::Point*>> const& cuttingSegments) const {
  for (const std::pair<ppxl::Point*, ppxl::Point*>& cuttingSegment: cuttingSegments) {
    if (cuttingSegment.first == intersection) {
      return cuttingSegment.second;
    } else if (cuttingSegment.second == intersection) {
      return cuttingSegment.first;
    }
  }

  return nullptr;
}

void GameController::OpenLevel(QString const& p_levelPath) {
  m_view->ClearImage();

  Parser parser(p_levelPath);
  m_model->SetPolygonsList(parser.GetPolygonList());

//  TapeList tapeList(parser.createTapeList());
//  m_model->setTapeList(tapeList);

//  MirrorList mirrorList(parser.createMirrorList());
//  PortalList portalList(parser.createPortalList());
//  RefractorList refractorList(parser.createRefractorList());
//  DeviationList deviationsList;
//  for (const Mirror& mirror: mirrorList) {
//    deviationsList.push_back(new Mirror(mirror));
//  }
//  for (const Portal& portal: portalList) {
//    deviationsList.push_back(new Portal(portal));
//  }
//  for (const Refractor& refractor: refractorList) {
//    deviationsList.push_back(new Refractor(refractor));
//  }
//  m_model->setDeviationList(deviationsList);

  m_gameInfo = GameInfo(0, parser.GetLinesGoal(), m_model->GetPolygonsCount(), parser.GetPartsGoal(), 0, parser.GetStarsCount());
  m_maxGapToWin = parser.GetMaxGapToWin();
  UpdateViewFromGameInfo();

  for (auto const* polygon: m_model->GetPolygonsList()) {
    m_orientedAreaTotal += polygon->OrientedArea();
  }

//  m_levelRunning = true;
}

void GameController::CheckWinning() {
  if (m_gameInfo.m_linesCount >= m_gameInfo.m_linesGoal || m_gameInfo.m_partsCount >= m_gameInfo.m_partsGoal) {
    QList<double> orientedAreas;
    QList<ppxl::Vector> shiftVectors;
    double minArea = 100.;
    double maxArea = 0.;

    for (auto const* polygon: m_model->GetPolygonsList()) {
      double currArea = ComputePolygonPercentageArea(*polygon);

      orientedAreas << currArea;
      minArea = std::min(currArea, minArea);
      maxArea = std::max(currArea, maxArea);

      ppxl::Vector currShift(ComputeGlobalBarycenter(), polygon->Barycenter());
      double currShiftLength = currShift.Norm();
      currShift.Normalize();
      currShift *= 0.2*currShiftLength;
      shiftVectors << currShift;
    }

    TranslatePolygons(shiftVectors);

    double gap = std::abs(maxArea - minArea);

    int starsCount = 0;
    m_gameInfo.m_stars = starsCount;
    if (m_gameInfo.m_partsCount == m_gameInfo.m_partsGoal && gap <= m_maxGapToWin && m_gameInfo.m_partsCount == m_gameInfo.m_partsGoal) {
      starsCount = 4 - static_cast<int>(std::ceil(gap / static_cast<double>(m_maxGapToWin) * 3.));
      UpdateStarsMax(starsCount);
      m_gameInfo.m_stars = starsCount;
    }

    m_view->DrawAreas(orientedAreas);

    //m_levelRunning = false;
  }
}

ppxl::Point GameController::ComputeGlobalBarycenter() const {
  ppxl::Point globalBarycenter;
  unsigned int polygonCount = 0;

  for (auto const* polygon: m_model->GetPolygonsList()) {
    globalBarycenter += polygon->Barycenter();
    ++polygonCount;
  }

  assert(polygonCount > 0);
  globalBarycenter /= polygonCount;

  return globalBarycenter;
}

double GameController::ComputePolygonPercentageArea(ppxl::Polygon const& polygon) const {
  return qRound(10.*polygon.OrientedArea() * 100. / m_orientedAreaTotal) / 10.;
}

void GameController::TranslatePolygons(QList<ppxl::Vector> const& shiftVectors) {
  PolygonList newPolygons;

  auto polygons = m_model->GetPolygonsList();
  assert(polygons.size() == shiftVectors.size());

  int index = 0;
  for (auto const* polygon: polygons) {
    ppxl::Polygon newPolygon(*polygon);
    newPolygon.Translate(shiftVectors.at(index));
    newPolygons << newPolygon;
    ++index;
  }

  m_model->SetPolygonsList(newPolygons);
}

void GameController::UpdateStarsMax(int starsMaxCount) {
  Parser parser(m_levelPath);
  Serializer serializer(m_levelPath);
  int currentStarsCount = parser.GetStarsCount();

  if (currentStarsCount < starsMaxCount) {
    serializer.SetStarsCount(starsMaxCount);
    serializer.WriteXML();
  }
}


/*




void GameController::undoSliceIt() {
  if (m_polygonListPerTurn.size() > 1) {
    m_polygonListPerTurn.pop_back();
    m_model->setPolygonList(m_polygonListPerTurn.last());
    m_gameInfo->setLinesCount(m_gameInfo.m_linesCount-1);
    m_gameInfo->setPartsCount(m_model->getPolygonsCount());
    m_levelRunning = true;

    emit update();
  }
}

void GameController::replay() {
  openLevel(mm_levelPath);
}

void GameController::clearGame() {
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

Deviation* GameController::getNearestDeviation(const ppxl::Segment& line) const {
  DeviationList deviations = m_model->getDeviationList();

  double minDist = -1.;
  Deviation* nearestDeviation = nullptr;

  for (Deviation* deviation: deviations) {
    QList<ppxl::Segment> deviateLines = deviation->deviateLine(line);
    // If there is at least one reflected line
    if (deviateLines.size() > 1) {
      if (minDist == -1)
        deviateLines.at(0).length();
      else
        qMin(minDist, deviateLines.at(0).length());
      nearestDeviation = deviation;
    }
  }

  return nearestDeviation;
}

void GameController::computeDeviateLines(double firstLineLength, const ppxl::Segment& line, QList<ppxl::Segment>& lines) const {
  Deviation* nearestDeviation = getNearestDeviation(line);

  if (nearestDeviation) {
    QList<ppxl::Segment> deviateLines = nearestDeviation->deviateLine(line);
    // Init firstLineLength
    if (firstLineLength == -1.f)
      firstLineLength = deviateLines.at(0).length();

    // At least two line has to be here: the line drawn and its reflexion
    assert(deviateLines.size() > 1);

    // Erase previous line, since it go through current mirror
    if (lines.size() > 1)
      lines.erase(lines.end()-1);

    // Get the mirrorLined the length of the firstLine
    ppxl::Segment deviateLine = deviateLines.at(1);
    ppxl::Point A = deviateLine.getA();
    ppxl::Point B = deviateLine.getB();
    ppxl::Vector lu = firstLineLength*ppxl::Vector(A, B).normalize();
    ppxl::Point BB(lu.getX(), lu.getY());
    deviateLine.setB(A + BB);

    // Push line and its reflexion
    lines << deviateLines.at(0));
    lines << deviateLine);

    computeDeviateLines(firstLineLength, deviateLine, lines);
  } else {
    lines << line);
  }
}

*/


bool ComparePoints(const ppxl::Point* A, const ppxl::Point* B) {
  return *A < *B;
}
