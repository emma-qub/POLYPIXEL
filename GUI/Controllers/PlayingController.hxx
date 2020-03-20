#ifndef PLAYINGCONTROLLER_HXX
#define PLAYINGCONTROLLER_HXX

#include <QObject>
#include <QPoint>

#include "Core/Geometry/Polygon.hxx"
#include "Core/Slicer.hxx"

class PolygonModel;
class ObjectModel;
class PlayingWidget;
class Deviation;
class Mutable;
class Obstacle;
class Object;

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
  PlayingController(PlayingWidget* p_view, QObject* p_parent = nullptr);

  void InitView();

  void SetObjectModelsList(QList<ObjectModel*> const& p_objectModelsList);

  void PlayLevel(QString const& p_levelPath);
  void RestartLevel();
  void Redraw();

signals:
  void GoToMapRequested(bool p_moveToNextLevel);

protected:
  virtual void UpdateViewFromGameInfo();

  void SetStartPoint(QPoint const& p_startPoint);
  ppxl::Point* GetOtherBound(ppxl::Point const* intersection, std::vector<std::pair<ppxl::Point*, ppxl::Point*>> const& cuttingSegments) const;

  QList<ppxl::Segment> MoveLine(QPoint const& p_endPoint);
  void SliceIt(QPoint const& p_endPoint);

  virtual void CheckWinning();

  void InvertScribbleLine(const QPoint& p_cursorPosition);
  QColor GetLinesColor(QList<ppxl::Segment> const& p_lines) const;

  void OpenLevel(QString const& p_levelPath);
  void StartLevel();

  int ComputeStarsCount(double p_gap);
  void UpdateStarsMax(int starsMaxCount);

  void DisplayGameOver();

protected:
  Slicer m_slicer;
  PolygonModel* m_polygonModel;
  QList<ObjectModel*> m_objectModelsList;
  QList<Object*> m_objectsList;
  QList<Object*> m_deviationsList;
  QList<Object*> m_mutablesList;
  QList<Object*> m_obstaclesList;
  GameInfo m_gameInfo;
  QString m_levelPath;

private:
  PlayingWidget* m_view;
  ppxl::Point m_startPoint;
  QPoint m_startQPoint;
};

#endif
