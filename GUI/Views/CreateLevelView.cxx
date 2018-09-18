#include "CreateLevelView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

CreateLevelView::CreateLevelView(QWidget* parent):
  QWidget(parent),
  m_createLevelLabel(new QLabel("Create your level")),
  m_testLevelButton(new QPushButton("Test")),
  m_menuButton(new QPushButton("Menu"))
{
  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_createLevelLabel);
  mainLayout->addWidget(m_testLevelButton);
  mainLayout->addWidget(m_menuButton);
  mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  setLayout(mainLayout);

  connect(m_testLevelButton, &QPushButton::clicked, this, &CreateLevelView::TestLevelRequested);
  connect(m_menuButton, &QPushButton::clicked, this, &CreateLevelView::Done);
}
