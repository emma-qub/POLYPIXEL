#ifndef LOADINGVIEW_HXX
#define LOADINGVIEW_HXX

#include <QWidget>

class QProgressBar;
class QLabel;

class LoadingView: public QWidget
{
  Q_OBJECT
public:
  explicit LoadingView(QWidget* parent = nullptr);

signals:
  void LoadingDone();

public slots:

protected:
  void paintEvent(QPaintEvent* p_event) override;

private:
  QProgressBar* m_progressBar;
  QLabel* m_loadingLabel;
};

#endif
