#include "SaveLevelView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

SaveLevelView::SaveLevelView(QWidget* parent):
  QWidget(parent),
  m_saveLevelLabel(new QLabel("Save your level")),
  m_menuButton(new QPushButton("Menu")),
  m_createNewLevelButton(new QPushButton("Create new level")) {

  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_saveLevelLabel);
  mainLayout->addWidget(m_createNewLevelButton);
  mainLayout->addWidget(m_menuButton);
  mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  setLayout(mainLayout);

  connect(m_menuButton, &QPushButton::clicked, this, &SaveLevelView::Done);
  connect(m_createNewLevelButton, &QPushButton::clicked, this, &SaveLevelView::CreateNewLevelRequested);
}
