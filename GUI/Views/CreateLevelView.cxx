#include "CreateLevelView.hxx"
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

CreateLevelView::CreateLevelView(QWidget* parent):
  QWidget(parent),
  m_createLevelLabel(new QLabel("Create your level")),
  m_testLevelButton(new QPushButton("Test")),
  m_menuButton(new QPushButton("Menu")),
  m_scribblingView(new CreateLevelScribblingView),
  m_polygonTreeView(new QTreeView),
  m_objectsTreeView(new QTreeView),
  m_treeViewStackWidget(new QStackedWidget),
  m_undoView(new QUndoView),
  m_linesGoalSpinBox(new QSpinBox),
  m_partsGoalSpinBox(new QSpinBox),
  m_maxGapToWinSpinBox(new QSpinBox),
  m_toleranceSpinBox(new QSpinBox) {

  m_polygonTreeView->setHeaderHidden(true);
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
  m_treeViewStackWidget->addWidget(m_polygonTreeView);
  m_treeViewStackWidget->addWidget(m_objectsTreeView);
  m_treeViewStackWidget->setCurrentWidget(m_polygonTreeView);
  rightViewLayout->addWidget(m_treeViewStackWidget);
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
  connect(m_scribblingView, &CreateLevelScribblingView::SnappedToGrid, this, &CreateLevelView::SnappedToGrid);
  connect(m_scribblingView, &CreateLevelScribblingView::NewLevelRequested, this, &CreateLevelView::NewLevelRequested);
  connect(m_scribblingView, &CreateLevelScribblingView::OpenLevelRequested, this, &CreateLevelView::OpenLevelRequested);

  auto itemDelegate = new PolygonItemDelegate(m_polygonTreeView);
  m_polygonTreeView->setItemDelegate(itemDelegate);
  connect(itemDelegate, &PolygonItemDelegate::ValueXChanged, this, &CreateLevelView::ValueXChanged);
  connect(itemDelegate, &PolygonItemDelegate::ValueYChanged, this, &CreateLevelView::ValueYChanged);
  connect(itemDelegate, &PolygonItemDelegate::EditionXDone, this, &CreateLevelView::EditionXDone);
  connect(itemDelegate, &PolygonItemDelegate::EditionYDone, this, &CreateLevelView::EditionYDone);

  connect(m_polygonTreeView, &QTreeView::clicked, this, &CreateLevelView::PolygonSelected);

  // Game info
  connect(m_maxGapToWinSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CreateLevelView::UpdateMaxGapToWinPrefix);
  connect(m_toleranceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CreateLevelView::UpdateTolerancePrefix);

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

  auto polygonAction = new QAction("Polygon Tool (P)", this);
  polygonAction->setShortcut(QKeySequence(Qt::Key_P));
  addAction(polygonAction);
  connect(polygonAction, &QAction::triggered, this, [this](){Q_EMIT(ToolActivated(ePolygonTool));});

  auto tapeAction = new QAction("Tape Tool (T)", this);
  tapeAction->setShortcut(QKeySequence(Qt::Key_T));
  addAction(tapeAction);
  connect(tapeAction, &QAction::triggered, this, [this](){Q_EMIT(ToolActivated(eTapeTool));});

  auto mirrorAction = new QAction("Mirror Tool (M)", this);
  mirrorAction->setShortcut(QKeySequence(Qt::Key_M));
  addAction(mirrorAction);
  connect(mirrorAction, &QAction::triggered, this, [this](){Q_EMIT(ToolActivated(eMirrorTool));});

  auto oneWayAction = new QAction("One Way Tool (O)", this);
  oneWayAction->setShortcut(QKeySequence(Qt::Key_O));
  addAction(oneWayAction);
  connect(oneWayAction, &QAction::triggered, this, [this](){Q_EMIT(ToolActivated(eOneWayTool));});

  auto portalAction = new QAction("Portal Tool (R)", this);
  portalAction->setShortcut(QKeySequence(Qt::Key_R));
  addAction(portalAction);
  connect(portalAction, &QAction::triggered, this, [this](){Q_EMIT(ToolActivated(ePortalTool));});
}

void CreateLevelView::InitView() {
  m_scribblingView->InitView();
  m_scribblingView->setFocus();
}

int CreateLevelView::GetLinesGoal() const {
  return m_linesGoalSpinBox->value();
}

void CreateLevelView::SetLinesGoal(int p_value) {
  m_linesGoalSpinBox->setValue(p_value);
}

int CreateLevelView::GetPartsGoal() const {
  return m_partsGoalSpinBox->value();
}

void CreateLevelView::SetPartsGoal(int p_value) {
  m_partsGoalSpinBox->setValue(p_value);
}

int CreateLevelView::GetMaxGapToWin() const {
  return m_maxGapToWinSpinBox->value();
}

void CreateLevelView::SetMaxGapToWin(int p_value) {
  m_maxGapToWinSpinBox->setValue(p_value);
}

int CreateLevelView::GetTolerance() const {
  return m_toleranceSpinBox->value();
}

void CreateLevelView::SetTolerance(int p_value) {
  m_toleranceSpinBox->setValue(p_value);
}

void CreateLevelView::ResetGameInfo() {
  m_linesGoalSpinBox->setValue(1);
  m_partsGoalSpinBox->setValue(2);
  m_maxGapToWinSpinBox->setValue(50);
  m_toleranceSpinBox->setValue(10);
}

void CreateLevelView::ActivatePolygonTool() {
  m_treeViewStackWidget->setCurrentWidget(m_polygonTreeView);
  m_scribblingView->SetToolMode(CreateLevelScribblingView::ePolygonMode);
}

void CreateLevelView::ActivateTapeTool() {
  m_treeViewStackWidget->setCurrentWidget(m_objectsTreeView);
  m_objectsTreeView->setModel(m_objectModelsList.at(ObjectModel::eTapeModel));
  m_objectsTreeView->expandAll();
  m_scribblingView->SetToolMode(CreateLevelScribblingView::eTapeMode);
}

void CreateLevelView::ActivateMirrorTool() {
  m_treeViewStackWidget->setCurrentWidget(m_objectsTreeView);
  m_objectsTreeView->setModel(m_objectModelsList.at(ObjectModel::eMirrorModel));
  m_objectsTreeView->expandAll();
  m_scribblingView->SetToolMode(CreateLevelScribblingView::eMirrorMode);
}

void CreateLevelView::ActivateOneWayTool() {
  m_treeViewStackWidget->setCurrentWidget(m_objectsTreeView);
  m_objectsTreeView->setModel(m_objectModelsList.at(ObjectModel::eOneWayModel));
  m_objectsTreeView->expandAll();
  m_scribblingView->SetToolMode(CreateLevelScribblingView::eOneWayMode);
}

void CreateLevelView::ActivatePortalTool() {
  m_treeViewStackWidget->setCurrentWidget(m_objectsTreeView);
  m_objectsTreeView->setModel(m_objectModelsList.at(ObjectModel::ePortalModel));
  m_objectsTreeView->expandAll();
  m_scribblingView->SetToolMode(CreateLevelScribblingView::ePortalMode);
}

void CreateLevelView::UpdateMaxGapToWinPrefix(int p_value) {
  if (3 <= p_value && p_value < 5) {
    m_maxGapToWinSpinBox->setSuffix(" (hard)");
  } else if (5 <= p_value && p_value < 20) {
    m_maxGapToWinSpinBox->setSuffix(" (normal)");
  } else {
    m_maxGapToWinSpinBox->setSuffix(" (easy)");
  }
}

void CreateLevelView::UpdateTolerancePrefix(int p_value) {
  if (p_value == 0) {
    m_toleranceSpinBox->setSuffix(" (insane)");
  } else if (p_value == 1) {
    m_toleranceSpinBox->setSuffix(" (very hard)");
  }
}

void CreateLevelView::SetPolygonModel(CreateLevelModel* p_polygonModel) {
  m_polygonModel = p_polygonModel;
  m_scribblingView->SetPolygonModel(p_polygonModel);
  m_polygonTreeView->setModel(p_polygonModel);
  m_polygonTreeView->header()->setMinimumSectionSize(50);
  m_scribblingView->SetSelectionModel(m_polygonTreeView->selectionModel());

  connect(m_polygonModel, &CreateLevelModel::rowsInserted, this, [this]() {
    m_polygonTreeView->expandAll();
    for (int column = m_polygonModel->columnCount()-1; column > -1; --column) {
      m_polygonTreeView->resizeColumnToContents(column);
    }
  });
}

void CreateLevelView::SetObjectModelsList(QList<ObjectModel*> const& p_objectModelsList) {
  m_objectModelsList.clear();
  m_objectModelsList << p_objectModelsList;
  m_scribblingView->SetObjectModelsList(p_objectModelsList);
  m_objectsTreeView->header()->setMinimumSectionSize(50);

  for (auto const* objectModel: m_objectModelsList) {
    connect(objectModel, &QStandardItemModel::rowsInserted, this, [this, objectModel]() {
      m_objectsTreeView->expandAll();
      for (int column = objectModel->columnCount()-1; column > -1; --column) {
        m_objectsTreeView->resizeColumnToContents(column);
      }
    });
  }
}

void CreateLevelView::SetUndoStack(QUndoStack* p_undoStack) {
  m_undoView->setStack(p_undoStack);
}

QItemSelectionModel* CreateLevelView::GetSelectionModel() const {
  return m_polygonTreeView->selectionModel();
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

void CreateLevelView::SetTestAvailable(bool p_enable)
{
  m_testLevelButton->setEnabled(p_enable);
}

void CreateLevelView::Redraw() {
  ClearImage();
  DrawFromModel();
}
