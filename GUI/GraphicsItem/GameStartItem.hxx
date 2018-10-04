#ifndef GAMESTARTITEM_HXX
#define GAMESTARTITEM_HXX

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QMouseEvent>
#include <QFont>

class GraphicsStarsItem;
class GraphicsGoalItem;


class CloseItem: public QObject, public QGraphicsRectItem {
  Q_OBJECT

public:
  CloseItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem *p_parent = nullptr);
  ~CloseItem() override;

protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent* p_event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* p_event) override;
};



class PlayItem: public QObject, public QGraphicsRectItem {
  Q_OBJECT

public:
  PlayItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem *p_parent = nullptr);
  ~PlayItem() override;

protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent* p_event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* p_event) override;
};



class GameStartItem: public QObject, public QGraphicsRectItem {
  Q_OBJECT
  Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
  GameStartItem(qreal p_x, qreal p_y, qreal p_width, qreal p_height, QGraphicsItem *p_parent = nullptr);
  ~GameStartItem() override;

  void SetLevelInfo(int p_levelNumber, int p_linesGoal, int p_partsGoal, int p_starsMax);
  void Open(QPointF const& p_startPos, QPointF const& p_endPos);

  void CloseToPlay();
  void CloseToCancel();

signals:
  void StartLevelRequested();
  void CancelLevelkRequested();

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* p_event) override;

private:
  QGraphicsRectItem* m_overlayItem;
  QGraphicsSimpleTextItem* m_levelNumberItem;
  GraphicsGoalItem* m_levelLinesGoalItem;
  GraphicsGoalItem* m_levelPartsGoalItem;
  GraphicsStarsItem* m_levelStarsItem;
  CloseItem* m_closeRectItem;
  PlayItem* m_playRectItem;
  QPen m_pen;
  QPointF m_startPos;
  QPointF m_endPos;
  QFont m_font;
};

#endif
