#ifndef SLICER_HXX
#define SLICER_HXX

#include "Core/Geometry/Polygon.hxx"

#include <vector>

class Object;
class Deviation;

class Slicer {

public:
  enum LineType {
    eGoodCrossing,
    eBadCrossing,
    eNoCrossing,
    eUnknownCrossing
  };

  Slicer();
  virtual ~Slicer();

  /// INLINE GETTERS AND SETTERS
  inline void SetPolygonsList(std::vector<ppxl::Polygon> const& p_polygonsList) { m_polygonsList = p_polygonsList; }
  inline std::vector<ppxl::Polygon> GetPolygonsList() { return m_polygonsList; }
  inline void SetDeviationsList(std::vector<Object*> p_deviationsList) { m_deviationsList = p_deviationsList; }
  inline void SetMutablesList(std::vector<Object*> p_mutablesList) { m_mutablesList = p_mutablesList; }
  inline void SetObstaclesList(std::vector<Object*> p_obstaclesList) { m_obstaclesList = p_obstaclesList; }
  inline void SetStartPoint(ppxl::Point const& p_startPoint) { m_startPoint = p_startPoint; }
  inline void SetOrientedAreaTotal(double p_orientedAreaTotal) { m_orientedAreaTotal = p_orientedAreaTotal; }
  static inline bool ComparePoints(const ppxl::Point* A, const ppxl::Point* B) { return *A < *B; }
  void SeObjectsList(std::vector<Object*> const& p_objectsList);

  /// SLICING ALGORITHM
  bool SliceIt(ppxl::Point const& p_endPoint);
  std::vector<ppxl::Segment> ComputeSlicingLines(ppxl::Point const& p_endPoint);
  LineType ComputeLinesType(std::vector<ppxl::Segment> const& p_lines) const;
  void ComputeDeviatedLines(double firstLineLength, ppxl::Segment const& line, std::vector<ppxl::Segment>& lines, int& p_counter, Deviation** p_lastDeviation) const;
  Deviation* GetNearestDeviation(ppxl::Segment const& line) const;
  void ComputeNewPolygonList(std::vector<ppxl::Polygon>& p_newPolygonList, ppxl::Segment const& p_line) const;
  void GetVerticesAndIntersections(ppxl::Segment const& line, ppxl::Polygon const& polygon,
    std::vector<ppxl::Point*>& globalVertices, std::vector<ppxl::Point*>& intersections) const;
  void CleanIntersections(ppxl::Polygon const& polygon, std::vector<ppxl::Point*>& intersections) const;
  std::vector<std::pair<ppxl::Point*, ppxl::Point*>> GetCuttingSegments(std::vector<ppxl::Point*> const& intersections) const;
  bool StillHasBaseVertices(std::vector<ppxl::Point*> const& verticesGlobal, std::vector<ppxl::Point*> const& intersections) const;
  ppxl::Point* GetOtherBound(ppxl::Point const* intersection, std::vector<std::pair<ppxl::Point*, ppxl::Point*>> const& cuttingSegments) const;

  /// AREAS AND BARYCENTERS
  std::vector<double> ComputeAreas(double& p_minArea, double& p_maxArea);
  ppxl::Point ComputeGlobalBarycenter() const;
  std::vector<ppxl::Vector> ComputeShiftVectorsList(ppxl::Point const& p_globalBarycenter);
  void InitTotalOrientedArea();

private:
  std::vector<ppxl::Polygon> m_polygonsList;
  std::vector<Object*> m_deviationsList;
  std::vector<Object*> m_mutablesList;
  std::vector<Object*> m_obstaclesList;
  ppxl::Point m_startPoint;
  double m_orientedAreaTotal;
};

#endif
