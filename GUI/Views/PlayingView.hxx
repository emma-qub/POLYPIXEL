#ifndef PLAYINGVIEW_HXX
#define PLAYINGVIEW_HXX

#include <QWidget>

#include "Core/Point.hxx"
#include "Core/Segment.hxx"

class QLabel;
class PolygonModel;
class PlayingScribblingView;

class PlayingView: public QWidget {
  Q_OBJECT

public:
  PlayingView(QWidget* p_parent = nullptr);

  void InitView();

  void SetModel(PolygonModel* p_playingModel);

  void DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void DrawFromModel();
  void SetAreasData(QList<double> const& p_areas, QList<ppxl::Vector> p_shiftVectors, ppxl::Point const& p_figureCenter);
  void ClearImage();

  void AnimatePolygons();

  void UpdateLinesCount(int p_linesCount, int p_linesGoal = -1);
  void UpdatePartsCount(int p_partsCount, int p_partsGoal = -1);

  void StartLevel();
  void SetEndLevelInfo(int m_linesCount, int m_linesGoal, int m_partsCount, int m_partsGoal, int m_stars);
  void EndLevel();
  void DisplayGameOver();

signals:
  void Scribbling(QPoint const& p_startPoint);
  void Moving(QPoint const& p_currentPoint);
  void Slicing(QPoint const& p_endPoint);
  void ControlPressed(QPoint const& p_cursorPosition);
  void ControlReleased(QPoint const& p_cursorPosition);
  void PolygonsAnimationDone();

protected:
  PlayingScribblingView* m_scribblingView;

private:
  int m_linesCount;
  int m_linesGoal;
  QLabel* m_linesCountLabel;
  int m_partsCount;
  int m_partsGoal;
  QLabel* m_partsCountLabel;
};

#endif
