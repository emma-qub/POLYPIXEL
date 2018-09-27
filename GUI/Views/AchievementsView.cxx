#include "AchievementsView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AchievementsView::AchievementsView(QWidget* parent):
  QWidget(parent),
  m_achievementsLabel(new QLabel("Achievements")),
  m_worldsButton(new QPushButton("Worlds")) {

  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_achievementsLabel);
  mainLayout->addWidget(m_worldsButton);
  mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  setLayout(mainLayout);

  connect(m_worldsButton, &QPushButton::clicked, this, &AchievementsView::Done);
}
