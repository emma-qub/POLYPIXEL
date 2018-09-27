#include "MainMenuView.hxx"

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>



#include <QPainter>
#include <QStyleOption>



MainMenuView::MainMenuView(QWidget* parent):
  QWidget(parent),
  m_menuLabel(new QLabel("Menu")),
  m_worldsButton(new QPushButton("Play")),
  m_createLevelButton(new QPushButton("Create Level")),
  m_optionsButton(new QPushButton("Options")) {

  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_menuLabel);
  m_menuLabel->setAlignment(Qt::AlignCenter);
  mainLayout->addWidget(m_worldsButton);
  mainLayout->addWidget(m_createLevelButton);
  mainLayout->addWidget(m_optionsButton);
  mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
  setLayout(mainLayout);

  connect(m_worldsButton, &QPushButton::clicked, this, &MainMenuView::PlayRequested);
  connect(m_createLevelButton, &QPushButton::clicked, this, &MainMenuView::CreateLevelRequested);
  connect(m_optionsButton, &QPushButton::clicked, this, &MainMenuView::OptionsRequested);
}

void MainMenuView::paintEvent(QPaintEvent* p_event) {
  QPainter p(this);
  QPainter painter(this);
  QRect drawingRect(0, 0, width(), height());
  painter.drawPixmap(drawingRect, QPixmap("../POLYPIXEL/resources/loading.png"));

  QWidget::paintEvent(p_event);
}
