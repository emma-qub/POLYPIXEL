#include "CreateLevelView.hxx"
#include "GUI/Models/CreateLevelModel.hxx"
#include "GUI/Views/CreateLevelScribblingView.hxx"
#include "GUI/Delegates/PolygonItemDelegate.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTreeView>
#include <QUndoView>
#include <QUndoStack>
#include <QHeaderView>
#include <QSpinBox>

CreateLevelView::CreateLevelView(QWidget* parent):
  QWidget(parent),
  m_createLevelLabel(new QLabel("Create your level")),
  m_testLevelButton(new QPushButton("Test")),
  m_menuButton(new QPushButton("Menu")),
  m_scribblingView(new CreateLevelScribblingView),
  m_treeView(new QTreeView),
  m_undoView(new QUndoView),
  m_linesGoalSpinBox(new QSpinBox),
  m_partsGoalSpinBox(new QSpinBox),
  m_maxGapToWinSpinBox(new QSpinBox),
  m_toleranceSpinBox(new QSpinBox)
{

  m_treeView->setHeaderHidden(true);

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_linesGoalSpinBox);
  menuLayout->addWidget(m_partsGoalSpinBox);
  menuLayout->addWidget(m_maxGapToWinSpinBox);
  menuLayout->addWidget(m_toleranceSpinBox);
  menuLayout->addWidget(m_testLevelButton);
  menuLayout->addWidget(m_menuButton);
  menuLayout->setAlignment(Qt::AlignCenter);
  auto viewLayout = new QHBoxLayout;
  viewLayout->addWidget(m_scribblingView);
  auto rightViewLayout = new QVBoxLayout;
  rightViewLayout->addWidget(m_treeView);
  rightViewLayout->addWidget(m_undoView);
  viewLayout->addLayout(rightViewLayout);
  viewLayout->setStretchFactor(m_scribblingView, 4);
  viewLayout->setStretchFactor(rightViewLayout, 1);
  mainLayout->addLayout(menuLayout);
  mainLayout->addLayout(viewLayout);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(viewLayout, 1);
  setLayout(mainLayout);

  connect(m_testLevelButton, &QPushButton::clicked, this, &CreateLevelView::TestLevelRequested);
  connect(m_menuButton, &QPushButton::clicked, this, &CreateLevelView::Done);

  connect(m_scribblingView, &CreateLevelScribblingView::PolygonInserted, this, &CreateLevelView::PolygonInserted);
  connect(m_scribblingView, &CreateLevelScribblingView::PolygonRemoved, this, &CreateLevelView::PolygonRemoved);
  connect(m_scribblingView, &CreateLevelScribblingView::PolygonMoved, this, &CreateLevelView::PolygonMoved);
  connect(m_scribblingView, &CreateLevelScribblingView::VertexInserted, this, &CreateLevelView::VertexInserted);
  connect(m_scribblingView, &CreateLevelScribblingView::VertexRemoved, this, &CreateLevelView::VertexRemoved);
  connect(m_scribblingView, &CreateLevelScribblingView::VertexMoved, this, &CreateLevelView::VertexMoved);
  connect(m_scribblingView, &CreateLevelScribblingView::PolygonSelected, this, &CreateLevelView::PolygonSelected);
  connect(m_scribblingView, &CreateLevelScribblingView::SnappedToGrid, this, &CreateLevelView::SnappedToGrid);

  auto itemDelegate = new PolygonItemDelegate(m_treeView);
  m_treeView->setItemDelegate(itemDelegate);
  connect(itemDelegate, &PolygonItemDelegate::ValueXChanged, this, &CreateLevelView::ValueXChanged);
  connect(itemDelegate, &PolygonItemDelegate::ValueYChanged, this, &CreateLevelView::ValueYChanged);
  connect(itemDelegate, &PolygonItemDelegate::EditionXDone, this, &CreateLevelView::EditionXDone);
  connect(itemDelegate, &PolygonItemDelegate::EditionYDone, this, &CreateLevelView::EditionYDone);

  // Game info
  connect(m_maxGapToWinSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CreateLevelView::UpdateMaxGapToWinPrefix);
  connect(m_toleranceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CreateLevelView::UpdateTolerancePrefix);

  m_linesGoalSpinBox->setRange(1, 25);
  m_linesGoalSpinBox->setFixedWidth(80);
  m_linesGoalSpinBox->setSuffix(" lines");
  m_partsGoalSpinBox->setRange(1, 50);
  m_partsGoalSpinBox->setFixedWidth(80);
  m_partsGoalSpinBox->setSuffix(" parts");
  m_maxGapToWinSpinBox->setRange(1, 50);
  m_maxGapToWinSpinBox->setFixedWidth(150);
  m_maxGapToWinSpinBox->setPrefix("dificulty ");
  m_maxGapToWinSpinBox->setValue(5);
  m_toleranceSpinBox->setRange(1, 20);
  m_toleranceSpinBox->setFixedWidth(150);
  m_toleranceSpinBox->setPrefix("perfect ");
  m_toleranceSpinBox->setValue(10);

  // Get back focus from spin box when playing on scribble view
  m_scribblingView->setFocusPolicy(Qt::StrongFocus);
}

int CreateLevelView::GetLinesGoal() const {
  return m_linesGoalSpinBox->value();
}

int CreateLevelView::GetPartsGoal() const {
  return m_partsGoalSpinBox->value();
}

int CreateLevelView::GetMaxGapToWin() const {
  return m_maxGapToWinSpinBox->value();
}

int CreateLevelView::GetTolerance() const {
  return m_toleranceSpinBox->value();
}

void CreateLevelView::UpdateMaxGapToWinPrefix(int p_value) {
  if (1 <= p_value && p_value < 5) {
    m_maxGapToWinSpinBox->setSuffix(" (hard)");
  } else if (5 <= p_value && p_value < 20) {
    m_maxGapToWinSpinBox->setSuffix(" (normal)");
  } else {
    m_maxGapToWinSpinBox->setSuffix(" (easy)");
  }
}

void CreateLevelView::UpdateTolerancePrefix(int p_value) {
  if (1 <= p_value && p_value < 5) {
    m_toleranceSpinBox->setSuffix(" (insane)");
  } else if (5 <= p_value && p_value < 10) {
    m_toleranceSpinBox->setSuffix(" (very hard)");
  } else {
    m_toleranceSpinBox->setSuffix(" (hard)");
  }
}

void CreateLevelView::SetModel(CreateLevelModel* p_model) {
  m_model = p_model;
  m_scribblingView->SetModel(p_model);
  m_treeView->setModel(p_model);
  m_treeView->header()->setMinimumSectionSize(50);
  m_scribblingView->SetSelectionModel(m_treeView->selectionModel());

  connect(m_model, &CreateLevelModel::rowsInserted, this, [this]() {
    m_treeView->expandAll();
    for (int column = m_model->columnCount()-1; column > -1; --column) {
      m_treeView->resizeColumnToContents(column);
    }
  });
}

void CreateLevelView::SetUndoStack(QUndoStack* p_undoStack)
{
  m_undoView->setStack(p_undoStack);
}

QItemSelectionModel* CreateLevelView::GetSelectionModel() const {
  return m_treeView->selectionModel();
}

void CreateLevelView::DrawFromModel() {
  m_scribblingView->DrawFromModel();
}

void CreateLevelView::ClearImage() {
  m_scribblingView->ClearImage();
}

void CreateLevelView::RedrawFromPolygons() {
  m_scribblingView->DrawFromCore();
}

void CreateLevelView::Redraw() {
  ClearImage();
  DrawFromModel();
}
