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

  void SetModel(PolygonModel* p_playingModel);

  void DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void DrawFromModel();
  void DrawAreas(QList<double> const& p_areas);
  void ClearImage();

  void UpdateLinesCount(int p_linesCount, int p_linesGoal = -1);
  void UpdatePartsCount(int p_partsCount, int p_partsGoal = -1);

  void StartLevel();
  void EndLevel();

signals:
  void Scribbling(QPoint const& p_startPoint);
  void Moving(QPoint const& p_currentPoint);
  void Slicing(QPoint const& p_endPoint);

private:
  PlayingScribblingView* m_scribblingView;
  int m_linesCount;
  int m_linesGoal;
  QLabel* m_linesCountLabel;
  int m_partsCount;
  int m_partsGoal;
  QLabel* m_partsCountLabel;
};

#endif
