#include "PauseView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

PauseView::PauseView(QWidget* parent):
  QWidget(parent),
  m_pauseLabel(new QLabel("Pause")),
  m_resumeButton(new QPushButton("Resume")),
  m_restartButton(new QPushButton("Restart")),
  m_levelsButton(new QPushButton("Levels"))
{
  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_pauseLabel);
  mainLayout->addWidget(m_resumeButton);
  mainLayout->addWidget(m_restartButton);
  mainLayout->addWidget(m_levelsButton);
  mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  setLayout(mainLayout);

  connect(m_resumeButton, &QPushButton::clicked, this, &PauseView::ResumeRequested);
  connect(m_restartButton, &QPushButton::clicked, this, &PauseView::RestartRequested);
  connect(m_levelsButton, &QPushButton::clicked, this, &PauseView::LevelsRequested);
}
