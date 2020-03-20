#include "CreateLevelWidget.hxx"
#include "GUI/Models/CreateLevelModel.hxx"
#include "GUI/Models/ObjectModel.hxx"
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
#include <QAction>
#include <QStackedWidget>
#include <QMouseEvent>

CreateLevelWidget::CreateLevelWidget(QWidget* parent):
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
  m_toleranceSpinBox(new QSpinBox) {

  m_treeView->setHeaderHidden(true);
  m_testLevelButton->setEnabled(false);

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

  connect(m_testLevelButton, &QPushButton::clicked, this, &CreateLevelWidget::TestLevelRequested);
  connect(m_menuButton, &QPushButton::clicked, this, &CreateLevelWidget::Done);

  connect(m_scribblingView, &CreateLevelScribblingView::PolygonInserted, this, &CreateLevelWidget::PolygonInserted);
  connect(m_scribblingView, &CreateLevelScribblingView::PolygonRemoved, this, &CreateLevelWidget::PolygonRemoved);
  connect(m_scribblingView, &CreateLevelScribblingView::PolygonMoved, this, &CreateLevelWidget::PolygonMoved);
  connect(m_scribblingView, &CreateLevelScribblingView::VertexInserted, this, &CreateLevelWidget::VertexInserted);
  connect(m_scribblingView, &CreateLevelScribblingView::VertexRemoved, this, &CreateLevelWidget::VertexRemoved);
  connect(m_scribblingView, &CreateLevelScribblingView::VertexMoved, this, &CreateLevelWidget::VertexMoved);
  connect(m_scribblingView, &CreateLevelScribblingView::SnappedToGrid, this, &CreateLevelWidget::SnappedToGrid);
  connect(m_scribblingView, &CreateLevelScribblingView::NewLevelRequested, this, &CreateLevelWidget::NewLevelRequested);
  connect(m_scribblingView, &CreateLevelScribblingView::OpenLevelRequested, this, &CreateLevelWidget::OpenLevelRequested);
  connect(m_scribblingView, &CreateLevelScribblingView::MousePressed, this, &CreateLevelWidget::MousePressed);
  connect(m_scribblingView, &CreateLevelScribblingView::MouseMoved, this, &CreateLevelWidget::MouseMoved);
  connect(m_scribblingView, &CreateLevelScribblingView::MouseReleased, this, &CreateLevelWidget::MouseReleased);

  auto itemDelegate = new PolygonItemDelegate(m_treeView);
  m_treeView->setItemDelegate(itemDelegate);
  connect(itemDelegate, &PolygonItemDelegate::ValueXChanged, this, &CreateLevelWidget::ValueXChanged);
  connect(itemDelegate, &PolygonItemDelegate::ValueYChanged, this, &CreateLevelWidget::ValueYChanged);
  connect(itemDelegate, &PolygonItemDelegate::EditionXDone, this, &CreateLevelWidget::EditionXDone);
  connect(itemDelegate, &PolygonItemDelegate::EditionYDone, this, &CreateLevelWidget::EditionYDone);

  ///connect(m_polygonTreeView, &QTreeView::clicked, this, &CreateLevelWidget::PolygonSelected);

  // Game info
  connect(m_maxGapToWinSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CreateLevelWidget::UpdateMaxGapToWinPrefix);
  connect(m_toleranceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CreateLevelWidget::UpdateTolerancePrefix);

  m_linesGoalSpinBox->setRange(1, 25);
  m_linesGoalSpinBox->setFixedWidth(80);
  m_linesGoalSpinBox->setSuffix(" lines");
  m_partsGoalSpinBox->setRange(2, 50);
  m_partsGoalSpinBox->setFixedWidth(80);
  m_partsGoalSpinBox->setSuffix(" parts");
  m_maxGapToWinSpinBox->setRange(3, 100);
  m_maxGapToWinSpinBox->setFixedWidth(150);
  m_maxGapToWinSpinBox->setPrefix("dificulty ");
  m_toleranceSpinBox->setRange(0, 1);
  m_toleranceSpinBox->setFixedWidth(150);
  m_toleranceSpinBox->setPrefix("perfect ");
  ResetGameInfo();
}

void CreateLevelWidget::InitViews() {
  m_scribblingView->InitView();
  m_scribblingView->setFocus();
}

int CreateLevelWidget::GetLinesGoal() const {
  return m_linesGoalSpinBox->value();
}

void CreateLevelWidget::SetLinesGoal(int p_value) {
  m_linesGoalSpinBox->setValue(p_value);
}

int CreateLevelWidget::GetPartsGoal() const {
  return m_partsGoalSpinBox->value();
}

void CreateLevelWidget::SetPartsGoal(int p_value) {
  m_partsGoalSpinBox->setValue(p_value);
}

int CreateLevelWidget::GetMaxGapToWin() const {
  return m_maxGapToWinSpinBox->value();
}

void CreateLevelWidget::SetMaxGapToWin(int p_value) {
  m_maxGapToWinSpinBox->setValue(p_value);
}

int CreateLevelWidget::GetTolerance() const {
  return m_toleranceSpinBox->value();
}

void CreateLevelWidget::SetTolerance(int p_value) {
  m_toleranceSpinBox->setValue(p_value);
}

void CreateLevelWidget::ResetGameInfo() {
  m_linesGoalSpinBox->setValue(1);
  m_partsGoalSpinBox->setValue(2);
  m_maxGapToWinSpinBox->setValue(50);
  m_toleranceSpinBox->setValue(10);
}

qreal CreateLevelWidget::GetSceneRectWidth() const {
  return m_scribblingView->GetSceneRectWidth();
}

qreal CreateLevelWidget::GetSceneRectHeight() const {
  return m_scribblingView->GetSceneRectHeight();
}

void CreateLevelWidget::CreateGraphicsObjectFromItem(QStandardItem* p_item) {
  m_scribblingView->CreateGraphicsObjectFromItem(p_item);
}

void CreateLevelWidget::UpdateMaxGapToWinPrefix(int p_value) {
  if (3 <= p_value && p_value < 5) {
    m_maxGapToWinSpinBox->setSuffix(" (hard)");
  } else if (5 <= p_value && p_value < 20) {
    m_maxGapToWinSpinBox->setSuffix(" (normal)");
  } else {
    m_maxGapToWinSpinBox->setSuffix(" (easy)");
  }
}

void CreateLevelWidget::UpdateTolerancePrefix(int p_value) {
  if (p_value == 0) {
    m_toleranceSpinBox->setSuffix(" (insane)");
  } else if (p_value == 1) {
    m_toleranceSpinBox->setSuffix(" (very hard)");
  }
}

void CreateLevelWidget::SetModel(CreateLevelModel* p_model) {
  m_model = p_model;
  m_scribblingView->SetModel(p_model);
  m_treeView->setModel(p_model);
  m_treeView->header()->setMinimumSectionSize(50);
  auto selectionModel = m_treeView->selectionModel();
  m_scribblingView->SetSelectionModel(selectionModel);

  connect(m_model, &CreateLevelModel::rowsInserted, this, [this]() {
    m_treeView->expandAll();
    for (int column = m_model->columnCount()-1; column > -1; --column) {
      m_treeView->resizeColumnToContents(column);
    }
  });

  ///connect(selectionModel, &QItemSelectionModel::currentChanged, )
}

void CreateLevelWidget::SetUndoStack(QUndoStack* p_undoStack) {
  m_undoView->setStack(p_undoStack);
}

QItemSelectionModel* CreateLevelWidget::GetSelectionModel() const {
  return m_treeView->selectionModel();
}

void CreateLevelWidget::DrawFromModel() {
  m_scribblingView->DrawFromModel();
}

void CreateLevelWidget::ClearImage() {
  m_scribblingView->ClearImage();
}

void CreateLevelWidget::RedrawFromPolygons() {
  m_scribblingView->DrawFromCore();
}

void CreateLevelWidget::SetTestAvailable(bool p_enable)
{
  m_testLevelButton->setEnabled(p_enable);
}

void CreateLevelWidget::Redraw() {
  ClearImage();
  DrawFromModel();
}
