#ifndef GAMECONTROLLER_HXX
#define GAMECONTROLLER_HXX

#include <QObject>
#include <QPoint>

#include "Core/Polygon.hxx"

class GameView;
class GameModel;

namespace ppxl {
class Point;
}

struct GameInfo {
  GameInfo(int p_linesCount = -1, int p_linesGoal = 1, int p_partsCount = -1, int p_partsGoal = -1, int p_stars = -1, int p_starsMax = -1):
  m_linesCount(p_linesCount),
  m_linesGoal(p_linesGoal),
  m_partsCount(p_partsCount),
  m_partsGoal(p_partsGoal),
  m_stars(p_stars),
  m_starsMax(p_starsMax) {}

  int m_linesCount;
  int m_linesGoal;
  int m_partsCount;
  int m_partsGoal;
  int m_stars;
  int m_starsMax;
};

class GameController: public QObject {
  Q_OBJECT

public:
  enum LineType {
    eGoodCrossing,
    eBadCrossing,
    eNoCrossing,
    eUnknownCrossing
  };

  GameController(GameModel* p_model, GameView* p_view, QObject* p_parent = nullptr);
  ~GameController() override;

  void PlayLevel(QString const& p_levelPath);
  void RestartLevel();

protected:
  void Redraw();
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
  void OpenLevel(const QString& p_levelPath);
  void CheckWinning();
  double ComputePolygonPercentageArea(ppxl::Polygon const& polygon) const;
  ppxl::Point ComputeGlobalBarycenter() const;
  void TranslatePolygons(const QList<ppxl::Vector>& shiftVectors);
  void UpdateStarsMax(int starsMaxCount);


  //  Deviation* getNearestDeviation(const ppxl::Segment& line) const;
  //  void computeDeviateLines(float firstLineLength, const ppxl::Segment& line, std::vector<ppxl::Segment>& lines) const;
  //  virtual void saveLevel(const QString& fileName) { Q_UNUSED(fileName) }
  //  void replay();
  //  void undoSliceIt();
  //  void clearGame();


private:
  GameModel* m_model;
  GameView* m_view;
  double m_orientedAreaTotal;
  int m_maxGapToWin;
  QString m_levelPath;
  ppxl::Point m_startPoint;
  GameInfo m_gameInfo;
};

bool ComparePoints(const ppxl::Point* A, const ppxl::Point* B);

#endif
