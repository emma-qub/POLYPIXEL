#include "CreateLevelView.hxx"
#include "GUI/Models/CreateLevelModel.hxx"
#include "GUI/Views/CreateLevelScribblingView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

CreateLevelView::CreateLevelView(QWidget* parent):
  QWidget(parent),
  m_createLevelLabel(new QLabel("Create your level")),
  m_testLevelButton(new QPushButton("Test")),
  m_menuButton(new QPushButton("Menu")),
  m_scribblingView(new CreateLevelScribblingView)
{
  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_createLevelLabel);
  menuLayout->addWidget(m_testLevelButton);
  menuLayout->addWidget(m_menuButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_scribblingView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_scribblingView, 1);
  setLayout(mainLayout);

  connect(m_testLevelButton, &QPushButton::clicked, this, &CreateLevelView::TestLevelRequested);
  connect(m_menuButton, &QPushButton::clicked, this, &CreateLevelView::Done);
}

void CreateLevelView::SetModel(CreateLevelModel* p_model) {
  m_model = p_model;
  m_scribblingView->SetModel(p_model);
}

void CreateLevelView::DrawFromModel() {
  m_scribblingView->DrawFromModel();
}

void CreateLevelView::ClearImage() {
  m_scribblingView->ClearImage();
}

void CreateLevelView::Redraw() {
  ClearImage();
  DrawFromModel();
}
