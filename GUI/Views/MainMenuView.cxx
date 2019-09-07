#include "MainMenuView.hxx"

#include "GUI/Components/qtmaterialcontainedbutton.h"

#include <QLabel>
#include <QVBoxLayout>

#include <QPainter>
#include <QStyleOption>


MainMenuView::MainMenuView(QWidget* parent):
  QWidget(parent),
  m_worldsButton(new QtMaterialContainedButton("Play")),
  m_createLevelButton(new QtMaterialContainedButton("Create Level")),
  m_optionsButton(new QtMaterialContainedButton("Options")) {

  auto mainLayout = new QVBoxLayout;
  auto emptyLayout = new QVBoxLayout;
  emptyLayout->addWidget(new QLabel);
  auto buttonLayout = new QVBoxLayout;
  buttonLayout->addWidget(m_worldsButton);
  buttonLayout->addWidget(m_createLevelButton);
  buttonLayout->addWidget(m_optionsButton);
  buttonLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(emptyLayout);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  auto buttonFont = m_worldsButton->font();
  buttonFont.setWeight(100);
  buttonFont.setPointSize(16);
  auto buttonCornerRadius = 20;
  auto buttonWidth = 450;
  m_worldsButton->setCornerRadius(buttonCornerRadius);
  m_worldsButton->setFixedWidth(buttonWidth);
  m_worldsButton->setFont(buttonFont);
  m_createLevelButton->setCornerRadius(buttonCornerRadius);
  m_createLevelButton->setFixedWidth(buttonWidth);
  m_createLevelButton->setFont(buttonFont);
  m_optionsButton->setCornerRadius(buttonCornerRadius);
  m_optionsButton->setFixedWidth(buttonWidth);
  m_optionsButton->setFont(buttonFont);

  connect(m_worldsButton, &QPushButton::clicked, this, &MainMenuView::PlayRequested);
  connect(m_createLevelButton, &QPushButton::clicked, this, &MainMenuView::CreateLevelRequested);
  connect(m_optionsButton, &QPushButton::clicked, this, &MainMenuView::OptionsRequested);
}

void MainMenuView::paintEvent(QPaintEvent* p_event) {
  QPainter p(this);
  QPainter painter(this);
  QRect drawingRect(0, 0, width(), height());
  painter.drawPixmap(drawingRect, QPixmap(":/backgrounds/loading.png"));

  QWidget::paintEvent(p_event);
}
