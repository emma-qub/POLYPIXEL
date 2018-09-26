#include "LevelsView.hxx"
#include "GUI/Models/LevelsModel.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListView>

LevelsView::LevelsView(QWidget* parent):
  QWidget(parent),
  m_levelsLabel(new QLabel("Levels")),
  m_levelsListView(new QListView(this)),
  m_selectWorldButton(new QPushButton("Worlds")),
  m_menuButton(new QPushButton("Menu")),
  m_model(nullptr) {

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_levelsLabel);
  menuLayout->addWidget(m_selectWorldButton);
  menuLayout->addWidget(m_menuButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_levelsListView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_levelsListView, 1);
  setLayout(mainLayout);

  m_levelsListView->setViewMode(QListView::IconMode);
  m_levelsListView->setLayoutMode(QListView::Batched);
  auto iconWidth = 300;
  m_levelsListView->setGridSize(QSize(iconWidth, iconWidth+20));
  m_levelsListView->setUniformItemSizes(true);
  m_levelsListView->setIconSize(QSize(iconWidth, iconWidth));

  connect(m_selectWorldButton, &QPushButton::clicked, this, &LevelsView::WorldsSelectionRequested);
  connect(m_menuButton, &QPushButton::clicked, this, &LevelsView::Done);
}

void LevelsView::SetModel(LevelsModel* p_model) {
  m_levelsListView->setModel(p_model);
}

QItemSelectionModel* LevelsView::GetSelectionModel() const {
  return m_levelsListView->selectionModel();
}
