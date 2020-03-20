#include "LoadingWidget.hxx"

#include <QProgressBar>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>


#include <QStyleOption>
#include <QPainter>



LoadingWidget::LoadingWidget(QWidget* parent):
  QWidget(parent),
  m_progressBar(new QProgressBar),
  m_loadingLabel(new QLabel("Laoding...")) {

  m_progressBar->setRange(0, 100);
  m_progressBar->setValue(0);
  m_progressBar->setFixedWidth(100);
  connect(m_progressBar, &QProgressBar::valueChanged, this, [this](int p_value) {
    if (p_value >= 100) {
      Q_EMIT LoadingDone();
    }
  });

  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_loadingLabel);
  m_loadingLabel->setAlignment(Qt::AlignCenter);
  mainLayout->addWidget(m_progressBar);
  mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  setLayout(mainLayout);

  auto timer = new QTimer(this);
  timer->setSingleShot(false);
  timer->setInterval(10);
  connect(timer, &QTimer::timeout, this, [this](){
    m_progressBar->setValue(m_progressBar->value()+10);
  });
  timer->start();
}

void LoadingWidget::paintEvent(QPaintEvent* p_event) {
 QPainter p(this);
 QPainter painter(this);
 QRect drawingRect(0, 0, width(), height());
 painter.drawPixmap(drawingRect, QPixmap(":/backgrounds/loading.png"));

 QWidget::paintEvent(p_event);
}
