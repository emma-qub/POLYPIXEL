#include "TestLevelView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

TestLevelView::TestLevelView(QWidget* parent):
  QWidget(parent),
  m_testLevelLabel(new QLabel("Test your level")),
  m_menuButton(new QPushButton("Menu")),
  m_amendLevelButton(new QPushButton("Amend level")),
  m_saveLevelButton(new QPushButton("Save level"))
{
  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_testLevelLabel);
  mainLayout->addWidget(m_amendLevelButton);
  mainLayout->addWidget(m_saveLevelButton);
  mainLayout->addWidget(m_menuButton);
  mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  setLayout(mainLayout);

  connect(m_menuButton, &QPushButton::clicked, this, &TestLevelView::Done);
  connect(m_amendLevelButton, &QPushButton::clicked, this, &TestLevelView::AmendLevelRequested);
  connect(m_saveLevelButton, &QPushButton::clicked, this, &TestLevelView::SaveLevelRequested);
}
