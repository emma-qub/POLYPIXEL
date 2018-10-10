#ifndef GAMESTARTITEM_HXX
#define GAMESTARTITEM_HXX

#include <QObject>
#include "GraphicsPixelRectItem.hxx"
#include <QPen>
#include <QMouseEvent>
#include <QFont>

class GraphicsStarsItem;
class GraphicsGoalItem;


class GameStartItem: public QObject, public GraphicsPixelRectItem {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
  GameStartItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem *p_parent = nullptr);
  ~GameStartItem() override;

  void InitItems();

  void SetLevelInfo(int p_levelNumber, int p_linesGoal, int p_partsGoal, int p_starsMax);
  void Open(QPointF const& p_startPos, QPointF const& p_endPos);

  void CloseToPlay();
  void CloseToCancel();

signals:
  void StartLevelRequested();
  void CancelLevelRequested();

private:
  QGraphicsSimpleTextItem* m_levelNumberItem;
  GraphicsGoalItem* m_levelLinesGoalItem;
  GraphicsGoalItem* m_levelPartsGoalItem;
  GraphicsStarsItem* m_levelStarsItem;
  QGraphicsRectItem* m_overlayItem;
  QPointF m_startPos;
  QPointF m_endPos;
  QFont m_font;
};

#endif
