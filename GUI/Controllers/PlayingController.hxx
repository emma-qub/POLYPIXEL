#ifndef PLAYINGCONTROLLER_HXX
#define PLAYINGCONTROLLER_HXX

#include <QObject>
#include <QPoint>

#include "Core/Polygon.hxx"

class PolygonModel;
class PlayingView;

struct GameInfo {
  GameInfo(int p_linesCount = -1, int p_linesGoal = 1, int p_partsCount = -1, int p_partsGoal = -1,
    int p_stars = -1, int p_starsMax = -1, int p_maxGapToWin = -1, int p_tolerance = -1):
  m_linesCount(p_linesCount),
  m_linesGoal(p_linesGoal),
  m_partsCount(p_partsCount),
  m_partsGoal(p_partsGoal),
  m_stars(p_stars),
  m_starsMax(p_starsMax),
  m_maxGapToWin(p_maxGapToWin),
  m_tolerance(p_tolerance) {}

  int m_linesCount;
  int m_linesGoal;
  int m_partsCount;
  int m_partsGoal;
  int m_stars;
  int m_starsMax;
  int m_maxGapToWin;
  int m_tolerance;
};

class PlayingController: public QObject {
  Q_OBJECT

public:
  enum LineType {
    eGoodCrossing,
    eBadCrossing,
    eNoCrossing,
    eUnknownCrossing
  };

  PlayingController(PlayingView* p_view, QObject* p_parent = nullptr);

  void PlayLevel(QString const& p_levelPath);
  void RestartLevel();
  void Redraw();

protected:
  void UpdateViewFromGameInfo();
  void SetStartPoint(QPoint const& p_startPoint);
  QList<ppxl::Segment> ComputeSlicingLines(QPoint const& p_endPoint);
  QColor GetLinesColor(QList<ppxl::Segment> const& p_lines) const;
  LineType ComputeLinesType(QList<ppxl::Segment> const& p_lines) const;
  void SliceIt(QPoint const& p_endPoint);
  void ComputeNewPolygonList(QList<ppxl::Polygon>& p_newPolygonList, ppxl::Segment const& p_line) const;
  void GetVerticesAndIntersections(ppxl::Segment const& line, ppxl::Polygon const& polygon,
    std::vector<ppxl::Point*>& globalVertices, std::vector<ppxl::Point*>& intersections) const;
  void CleanIntersections(ppxl::Polygon const& polygon, std::vector<ppxl::Point*>& intersections) const;
  std::vector<std::pair<ppxl::Point*, ppxl::Point*>> GetCuttingSegments(std::vector<ppxl::Point*> const& intersections) const;
  bool StillHasBaseVertices(std::vector<ppxl::Point*> const& verticesGlobal, std::vector<ppxl::Point*> const& intersections) const;
  ppxl::Point* GetOtherBound(ppxl::Point const* intersection, std::vector<std::pair<ppxl::Point*, ppxl::Point*>> const& cuttingSegments) const;
  void OpenLevel(QString const& p_levelPath);
  QList<ppxl::Vector> ComputeShiftVectorsList();
  QList<double> ComputeAreas(double& p_minArea, double& p_maxArea);
  int ComputeStarsCount(double p_gap);
  virtual void CheckWinning();
  double ComputePolygonPercentageArea(ppxl::Polygon const& polygon) const;
  ppxl::Point ComputeGlobalBarycenter() const;
  void TranslatePolygons(QList<ppxl::Vector> const& shiftVectors);
  void UpdateStarsMax(int starsMaxCount);

  //  Deviation* getNearestDeviation(ppxl::Segment const& line) const;
  //  void computeDeviateLines(double firstLineLength, const ppxl::Segment& line, QList<ppxl::Segment>& lines) const;
  //  virtual void saveLevel(QString const& fileName) { Q_UNUSED(fileName) }
  //  void replay();
  //  void undoSliceIt();
  //  void clearGame();

protected:
  PolygonModel* m_model;
  GameInfo m_gameInfo;
  QString m_levelPath;
  double m_orientedAreaTotal;

private:
  PlayingView* m_view;
  ppxl::Point m_startPoint;
};

bool ComparePoints(const ppxl::Point* A, const ppxl::Point* B);

#endif
