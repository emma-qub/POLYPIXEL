#include "TestingView.hxx"

#include "GUI/Views/PlayingScribblingView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

TestingView::TestingView(QWidget* p_parent):
  PlayingView(p_parent),
  m_starsCountLabel(new QLabel("Stars: 0")),
  m_perfectLabel(new QLabel("Perfect: NO")),
  m_restartButton(new QPushButton("Restart")),
  m_saveButton(new QPushButton("Save")) {

  m_restartButton->setFixedWidth(100);
  m_saveButton->setFixedWidth(100);
  m_saveButton->setEnabled(false);

  auto mainLayout = static_cast<QGridLayout*>(layout());

  auto layoutItem = mainLayout->takeAt(0);
  auto menuLayout = static_cast<QHBoxLayout*>(layoutItem->layout());
  auto linesLayoutItem = menuLayout->takeAt(0);
  auto linesLabel = static_cast<QLabel*>(linesLayoutItem->widget());
  auto partsLayoutItem = menuLayout->takeAt(0);
  auto partsLabel = static_cast<QLabel*>(partsLayoutItem->widget());

  auto newMenuLayout = new QVBoxLayout;
  newMenuLayout->addWidget(linesLabel);
  newMenuLayout->addWidget(partsLabel);
  newMenuLayout->addWidget(m_starsCountLabel);
  newMenuLayout->addWidget(m_perfectLabel);
  newMenuLayout->addWidget(m_restartButton);
  newMenuLayout->addWidget(m_saveButton);

  newMenuLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

  auto widgetItem = mainLayout->takeAt(0);
  auto scribblingView = static_cast<PlayingScribblingView*>(widgetItem->widget());

  mainLayout->addWidget(scribblingView, 0, 0);
  mainLayout->addLayout(newMenuLayout, 0, 1);
  mainLayout->setRowStretch(0, 1);
  mainLayout->setRowStretch(1, 0);
  mainLayout->setColumnStretch(0, 4);
  mainLayout->setColumnStretch(1, 1);

  connect(m_restartButton, &QPushButton::clicked, this, &TestingView::RestartRequested);
  connect(m_saveButton, &QPushButton::clicked, this, &TestingView::SaveLevelRequested);
}

void TestingView::DrawText(ppxl::Point p_position, const QString& p_text, int p_weight) {
  m_scribblingView->DrawText(p_position, p_text, p_weight);
}

void TestingView::UpdateStarsCount(int p_starsCount) {
  m_starsCountLabel->setText(tr("Stars: %1").arg(p_starsCount));
}

void TestingView::UpdatePerfect(bool p_perfect) {
  QString perfect = p_perfect ? "YES": "NO";
  m_perfectLabel->setText(tr("perfect: %1").arg(perfect));
}

void TestingView::SetSaveButtonEnable(bool p_enable) {
  m_saveButton->setEnabled(p_enable);
}
