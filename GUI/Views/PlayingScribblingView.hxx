#ifndef GAMESCRIBBLINGVIEW_HXX
#define GAMESCRIBBLINGVIEW_HXX

#include "GUI/Views/AbstractScribblingView2.hxx"

class PolygonModel;
class GameOverItem;
class GraphicsRoundedRectItem;

class PlayingScribblingView: public AbstractScribblingView2 {
  Q_OBJECT

public:
  PlayingScribblingView(QWidget* parent = nullptr);
  ~PlayingScribblingView() override;

  void InitView() override;

  void AnimatePolygons();

  void SetEndLevelInfo(int p_linesCount, int p_linesGoal, int p_partsCount, int p_partsGoal, int p_stars);
  void SetAreasData(QList<double> const& p_areas, QList<ppxl::Vector> p_shiftVectors, ppxl::Point const& p_figureCenter);
  void DisplayGameOver();
  void DisplayWinOrFail();

signals:
  void Scribbling(QPoint const& p_startPoint);
  void Moving(QPoint const& p_currentPoint);
  void Slicing(QPoint const& p_endPoint);
  void ControlPressed(QPoint const& p_cursorPosition);
  void ControlReleased(QPoint const& p_cursorPosition);
  void PolygonsAnimationDone();
  void FadeInOverlayDone();
  void FadeOutOverlayDone();

protected:
  void keyPressEvent(QKeyEvent* p_event) override;
  void keyReleaseEvent(QKeyEvent* p_event) override;
  void mousePressEvent(QMouseEvent* p_event) override;
  void mouseMoveEvent(QMouseEvent* p_event) override;
  void mouseReleaseEvent(QMouseEvent* p_event) override;

  void AnimateFail(GraphicsRoundedRectItem* p_item, QPointF const& p_startPos, QPointF const& p_endPos);
  void AnimateWin(QObject* p_item, QPointF const& p_startPos, QPointF const& p_endPos);
  void AnimatePerfect(QObject* p_rectItem, const QRectF& p_endRect);

private:
  bool m_scribbling;

  QPoint m_cursorPosition;
  GameOverItem* m_gameOverItem;
  QGraphicsRectItem* m_overlayItem;
  QBrush m_overlayBrush;

  int m_levelNumber;
  int m_linesCount;
  int m_linesGoal;
  int m_partsCount;
  int m_partsGoal;
  int m_starsMax;
  QList<double> m_areas;
  QList<ppxl::Vector> m_shiftVectors;
  ppxl::Point m_figureCenter;
};

#endif
