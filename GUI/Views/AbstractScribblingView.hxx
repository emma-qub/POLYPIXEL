#ifndef ABSTRACTSCRIBBLINGVIEW_HXX
#define ABSTRACTSCRIBBLINGVIEW_HXX

#include <QWidget>

#include "Core/Segment.hxx"

class AbstractLevelModel;

class AbstractScribblingView: public QWidget {
  Q_OBJECT

public:
  explicit AbstractScribblingView(QWidget* p_parent = nullptr);
  ~AbstractScribblingView() override;

  virtual void SetModel(AbstractLevelModel* p_model);

  void DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  virtual void DrawFromModel();
  void ClearImage();

protected:
  inline QImage& GetImage() { return m_image; }
  inline AbstractLevelModel* GetModel() const { return m_model; }

  void DrawLine(ppxl::Point const& p_startPoint, ppxl::Point const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void DrawLine(QPoint const& p_startPoint, QPoint const& p_endPoint, QColor const& p_color, Qt::PenStyle p_penStyle = Qt::SolidLine);
  void ResizeImage(QImage* image, const QSize& newSize);

  void mousePressEvent(QMouseEvent* event) override = 0;
  void mouseMoveEvent(QMouseEvent* event) override = 0;
  void mouseReleaseEvent(QMouseEvent* event) override = 0;
  void paintEvent(QPaintEvent* event) override = 0;
  void resizeEvent(QResizeEvent* event) override;

private:
  AbstractLevelModel* m_model;
  int m_myPenWidth;
  QColor m_myPenColor;
  QImage m_image;
};

#endif
