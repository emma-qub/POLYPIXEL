#ifndef GAMEVIEW_HXX
#define GAMEVIEW_HXX

#include <QWidget>

#include "Core/Point.hxx"
#include "Core/Segment.hxx"

class QLabel;
class QPushButton;
class GameModel;

class ScriblingView: public QWidget {
  Q_OBJECT

public:
  explicit ScriblingView(QWidget* parent = nullptr);
  ~ScriblingView() override;

  void setModel(GameModel* p_gameModel);

  void drawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void drawFromModel();
  void clearImage();

signals:
  void Scribbling(QPoint const& p_startPoint);
  void Moving(QPoint const& p_currentPoint);
  void Slicing(QPoint const& p_endPoint);

protected:
  void drawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void drawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void resizeImage(QImage* image, const QSize& newSize);

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

private:
  GameModel* m_gameModel;
  int m_myPenWidth;
  QColor m_myPenColor;
  QImage m_image;
  bool m_scribbling;
};

class GameView: public QWidget {
  Q_OBJECT

public:
  explicit GameView(QWidget* parent = nullptr);

  void SetModel(GameModel* p_gameModel);

  void DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void DrawFromModel();
  void ClearImage();

  void UpdateLinesCount(int p_linesCount, int p_linesGoal = -1);
  void UpdatePartsCount(int p_partsCount, int p_partsGoal = -1);

signals:
  void PauseRequested();
  void Scribbling(QPoint const& p_startPoint);
  void Moving(QPoint const& p_currentPoint);
  void Slicing(QPoint const& p_endPoint);

public slots:

private:
  QLabel* m_gameLabel;
  QPushButton* m_pauseButton;
  GameModel* m_gameModel;
  ScriblingView* m_scriblingView;
  int m_linesCount;
  int m_linesGoal;
  QLabel* m_linesCountLabel;
  int m_partsCount;
  int m_partsGoal;
  QLabel* m_partsCountLabel;
};

#endif
