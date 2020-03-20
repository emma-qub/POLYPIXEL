#ifndef LOADINGWIDGET_HXX
#define LOADINGWIDGET_HXX

#include <QWidget>

class QProgressBar;
class QLabel;

class LoadingWidget: public QWidget {
  Q_OBJECT

public:
  explicit LoadingWidget(QWidget* parent = nullptr);

signals:
  void LoadingDone();

protected:
  void paintEvent(QPaintEvent* p_event) override;

private:
  QProgressBar* m_progressBar;
  QLabel* m_loadingLabel;
};

#endif
