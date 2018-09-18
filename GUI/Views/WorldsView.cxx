#include "WorldsView.hxx"
#include "GUI/Models/WorldsModel.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListView>

WorldsView::WorldsView(QWidget* parent):
  QWidget(parent),
  m_worldsLevelLabel(new QLabel("Worlds")),
  m_worldsListView(new QListView(this)),
  m_achievementsButton(new QPushButton("Achievements")),
  m_menuButton(new QPushButton("Menu")),
  m_model(nullptr) {

  m_worldsListView->setViewMode(QListView::IconMode);
  m_worldsListView->setLayoutMode(QListView::Batched);
  m_worldsListView->setGridSize(QSize(350, 370));
  m_worldsListView->setUniformItemSizes(true);
  m_worldsListView->setIconSize(QSize(350, 350));

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_worldsLevelLabel);
  menuLayout->addWidget(m_achievementsButton);
  menuLayout->addWidget(m_menuButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_worldsListView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_worldsListView, 1);
  setLayout(mainLayout);

  connect(m_achievementsButton, &QPushButton::clicked, this, &WorldsView::AchievementsRequested);
  connect(m_menuButton, &QPushButton::clicked, this, &WorldsView::Done);
}

void WorldsView::SetModel(WorldsModel* p_model) {
  m_worldsListView->setModel(p_model);
}

QItemSelectionModel* WorldsView::GetSelectionModel() const {
  return m_worldsListView->selectionModel();
}
