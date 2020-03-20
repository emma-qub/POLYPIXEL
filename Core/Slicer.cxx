#include "Slicer.hxx"

#include "Core/Geometry/Vector.hxx"
#include "Core/Objects/Object.hxx"
#include "Core/Objects/Deviations/Deviation.hxx"
#include "Core/Objects/Obstacles/Obstacle.hxx"

#include <cmath>

Slicer::Slicer():
  m_startPoint(),
  m_orientedAreaTotal(0.) {
}

Slicer::~Slicer() = default;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLICING ALGORITHM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Slicer::SliceIt(ppxl::Point const& p_endPoint) {
  auto lines = ComputeSlicingLines(p_endPoint);

  if (ComputeLinesType(lines) == eGoodCrossing) {
    std::vector<ppxl::Polygon> newPolygonList;
    for (ppxl::Segment const& line: lines) {
      // Browse every polygon and slice it!
      ComputeNewPolygonList(newPolygonList, line);
      m_polygonsList = newPolygonList;
      newPolygonList.clear();
    }
    return true;
  }

  return false;
}

std::vector<ppxl::Segment> Slicer::ComputeSlicingLines(ppxl::Point const& p_endPoint) {
  std::vector<ppxl::Segment> lines;

  ppxl::Segment line(m_startPoint, p_endPoint);
  ComputeDeviatedLines(-1, line, lines);

  return lines;
}

Slicer::LineType Slicer::ComputeLinesType(std::vector<ppxl::Segment> const& p_lines) const {
  bool noCrossing = false;
  bool goodCrossing = false;
  bool badCrossing = false;

  auto polygonList = m_polygonsList;
  for (ppxl::Segment const& line: p_lines) {
    for (auto const& polygon: polygonList) {
      if (!polygon.IsCrossing(line) && !polygon.IsPointInside(line.GetA())) {
        noCrossing = true;
      } else if (polygon.IsGoodSegment(line)) {
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

void Slicer::ComputeDeviatedLines(double firstLineLength, ppxl::Segment const& line, std::vector<ppxl::Segment>& lines) const {
  Deviation* nearestDeviation = GetNearestDeviation(line);

  if (nearestDeviation) {
    std::vector<ppxl::Segment> deviateLines = nearestDeviation->DeviateLine(line);
    // Init firstLineLength
    if (firstLineLength < 0.) {
      firstLineLength = deviateLines.at(0).Length();
    }

    // At least two line has to be here: the line drawn and its reflexion
    assert(deviateLines.size() > 1);

    // Erase previous line, since it goes through current mirror
    if (lines.size() > 1) {
      lines.erase(lines.end()-1);
    }

    // Make the deviated line's length the same as the firstLine one
    ppxl::Segment deviateLine = deviateLines.at(1);
    ppxl::Point A = deviateLine.GetA();
    ppxl::Point B = deviateLine.GetB();
    ppxl::Vector lu = firstLineLength * ppxl::Vector(A, B).Normalize();
    ppxl::Point BB(lu.GetX(), lu.GetY());
    deviateLine.SetB(A + BB);

    // Push line and its reflexion
    lines.push_back(deviateLines.at(0));
    lines.push_back(deviateLine);

    ComputeDeviatedLines(firstLineLength, deviateLine, lines);
  } else {
    if (lines.size() > 1) {
      lines.erase(lines.end()-1);
    }
    lines.push_back(line);
  }
}

Deviation* Slicer::GetNearestDeviation(ppxl::Segment const& line) const {
  double minDist = -1.;
  Deviation* nearestDeviation = nullptr;

  for (auto* deviation: m_deviationsList) {
    auto deviationCast = static_cast<Deviation*>(deviation);
    std::vector<ppxl::Segment> deviateLines = deviationCast->DeviateLine(line);
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

void Slicer::ComputeNewPolygonList(std::vector<ppxl::Polygon>& p_newPolygonList, ppxl::Segment const& p_line) const {
  for (auto const& polygon: m_polygonsList) {
    std::vector<ppxl::Point*> globalVertices;
    std::vector<ppxl::Point*> intersections;
    GetVerticesAndIntersections(p_line, polygon, globalVertices, intersections);

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
        p_newPolygonList.push_back(newPolygon);
      }

      newVertices.clear();
    }
  }
}

void Slicer::GetVerticesAndIntersections(ppxl::Segment const& line, ppxl::Polygon const& polygon,
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

  std::sort(intersections.begin(), intersections.end(), Slicer::ComparePoints);

  CleanIntersections(polygon, intersections);
}

void Slicer::CleanIntersections(ppxl::Polygon const& polygon, std::vector<ppxl::Point*>& intersections) const {
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

std::vector<std::pair<ppxl::Point*, ppxl::Point*>> Slicer::GetCuttingSegments(std::vector<ppxl::Point*> const& intersections) const {
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

bool Slicer::StillHasBaseVertices(std::vector<ppxl::Point*> const& globalVertices, std::vector<ppxl::Point*> const& intersections) const {
  for (ppxl::Point* p: globalVertices) {
    if (std::find(intersections.begin(), intersections.end(), p) == intersections.end()) {
      return true;
    }
  }

  return false;
}

ppxl::Point* Slicer::GetOtherBound(ppxl::Point const* intersection, std::vector<std::pair<ppxl::Point*, ppxl::Point*>> const& cuttingSegments) const {
  for (std::pair<ppxl::Point*, ppxl::Point*> const& cuttingSegment: cuttingSegments) {
    if (cuttingSegment.first == intersection) {
      return cuttingSegment.second;
    } else if (cuttingSegment.second == intersection) {
      return cuttingSegment.first;
    }
  }

  return nullptr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AREAS AND BARYCENTERS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<double> Slicer::ComputeAreas(double& p_minArea, double& p_maxArea) {
  std::vector<double> orientedAreas;
  p_minArea = 100.;
  p_maxArea = 0.;

  double areaCumul = 0.;
  for (unsigned int row = 0; row < m_polygonsList.size(); ++row) {
    auto polygon = m_polygonsList.at(row);
    double currArea = 0.;

    if (row == m_polygonsList.size()-1) {
      currArea = 100. - areaCumul;
    } else {
      currArea = qRound(10.*polygon.OrientedArea() * 100. / m_orientedAreaTotal) / 10.;
      areaCumul += currArea;
    }

    orientedAreas.push_back(currArea);
    p_minArea = std::min(currArea, p_minArea);
    p_maxArea = std::max(currArea, p_maxArea);
  }

  return orientedAreas;
}

ppxl::Point Slicer::ComputeGlobalBarycenter() const {
  ppxl::Point globalBarycenter;
  unsigned int polygonCount = 0;

  for (auto const& polygon: m_polygonsList) {
    globalBarycenter += polygon.Barycenter();
    ++polygonCount;
  }

  assert(polygonCount > 0);
  globalBarycenter /= polygonCount;

  return globalBarycenter;
}

std::vector<ppxl::Vector> Slicer::ComputeShiftVectorsList(ppxl::Point const& p_globalBarycenter) {
  std::vector<ppxl::Vector> shiftVectors;

  for (auto const& polygon: m_polygonsList) {
    ppxl::Vector currShift(p_globalBarycenter, polygon.Barycenter());
    double currShiftLength = currShift.Norm();
    currShift.Normalize();
    currShift *= 0.2*currShiftLength;
    shiftVectors.push_back(currShift);
  }

  return shiftVectors;
}

void Slicer::InitTotalOrientedArea() {
  m_orientedAreaTotal = 0.;
  for (auto const& polygon: m_polygonsList) {
    m_orientedAreaTotal += polygon.OrientedArea();
  }
}
