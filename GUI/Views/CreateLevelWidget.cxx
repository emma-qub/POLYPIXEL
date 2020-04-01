#include "CreateLevelWidget.hxx"
#include "GUI/Models/CreateLevelObjectsListModel.hxx"
#include "GUI/Models/CreateLevelObjectsDetailModel.hxx"
#include "GUI/Models/CreateLevelVertexListModel.hxx"
#include "GUI/Views/CreateLevelScribblingView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTreeView>
#include <QUndoView>
#include <QHeaderView>
#include <QUndoStack>
#include <QSpinBox>
#include <QAction>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QMessageBox>
#include <QFileDialog>

CreateLevelWidget::CreateLevelWidget(QWidget* parent):
  QWidget(parent),
  m_createLevelLabel(new QLabel("Create your level")),
  m_testLevelButton(new QPushButton("Test")),
  m_menuButton(new QPushButton("Menu")),
  m_objectsListModel(nullptr),
  m_objectsDetailModel(nullptr),
  m_scribblingView(new CreateLevelScribblingView),
  m_objectsListTreeView(new QTreeView),
  m_objectsDetailTreeView(new QTreeView),
  m_vertexTreeView(new QTreeView),
  m_detailVertexStackWidget(new QStackedWidget),
  m_linesGoalSpinBox(new QSpinBox),
  m_partsGoalSpinBox(new QSpinBox),
  m_maxGapToWinSpinBox(new QSpinBox),
  m_toleranceSpinBox(new QSpinBox) {

  m_objectsListTreeView->setHeaderHidden(true);
  m_objectsDetailTreeView->setHeaderHidden(true);
  m_vertexTreeView->setHeaderHidden(true);
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
  rightViewLayout->addWidget(m_objectsListTreeView);
  m_detailVertexStackWidget->addWidget(m_objectsDetailTreeView);
  m_detailVertexStackWidget->addWidget(m_vertexTreeView);
  rightViewLayout->addWidget(m_detailVertexStackWidget);
  viewLayout->addLayout(rightViewLayout);
  viewLayout->setStretchFactor(m_scribblingView, 4);
  viewLayout->setStretchFactor(rightViewLayout, 1);
  mainLayout->addLayout(menuLayout);
  mainLayout->addLayout(viewLayout);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(viewLayout, 1);
  setLayout(mainLayout);

  connect(m_testLevelButton, &QPushButton::clicked, this, &CreateLevelWidget::TestLevelRequested);
  connect(m_menuButton, &QPushButton::clicked, this, &CreateLevelWidget::CreateLevelDone);

  auto addPolygonAction = new QAction("New polygon", this);
  addPolygonAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
  //connect(addPolygonAction, &QAction::triggered, this, &CreateLevelScribblingView::AppendPolygon);
  addAction(addPolygonAction);

  auto m_removeAction = new QAction("Remove", this);
  m_removeAction->setShortcut(QKeySequence(Qt::Key_Delete));
  //connect(m_removeAction, &QAction::triggered, this, &CreateLevelScribblingView::Remove);
  addAction(m_removeAction);

  auto snapToGridAction = new QAction("Snap to grid", this);
  snapToGridAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  connect(snapToGridAction, &QAction::triggered, this, &CreateLevelWidget::SnapToGridRequested);
  addAction(snapToGridAction);

  auto snapAllToGridAction = new QAction("Snap all to grid", this);
  snapAllToGridAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT+ Qt::Key_I));
  connect(snapAllToGridAction, &QAction::triggered, this, &CreateLevelWidget::SnapAllToGridRequested);
  addAction(snapAllToGridAction);

  auto newAction = new QAction("New level", this);
  newAction->setShortcut(QKeySequence::New);
  addAction(newAction);
  connect(newAction, &QAction::triggered, this, &CreateLevelWidget::ConfirmNewLevel);

  auto openAction = new QAction("Open level", this);
  openAction->setShortcut(QKeySequence::Open);
  addAction(openAction);
  connect(openAction, &QAction::triggered, this, &CreateLevelWidget::ConfirmOpenLevel);

  // Scribble view signals forward
  connect(m_scribblingView, &CreateLevelScribblingView::MousePressed, this, &CreateLevelWidget::MousePressed);
  connect(m_scribblingView, &CreateLevelScribblingView::MouseMoved, this, &CreateLevelWidget::MouseMoved);
  connect(m_scribblingView, &CreateLevelScribblingView::MouseReleased, this, &CreateLevelWidget::MouseReleased);
  connect(m_scribblingView, &CreateLevelScribblingView::KeyReturnPressed, this, &CreateLevelWidget::KeyReturnPressed);
  connect(m_scribblingView, &CreateLevelScribblingView::KeyLeftPressed, this, &CreateLevelWidget::KeyLeftPressed);
  connect(m_scribblingView, &CreateLevelScribblingView::KeyUpPressed, this, &CreateLevelWidget::KeyUpPressed);
  connect(m_scribblingView, &CreateLevelScribblingView::KeyRightPressed, this, &CreateLevelWidget::KeyRightPressed);
  connect(m_scribblingView, &CreateLevelScribblingView::KeyDownPressed, this, &CreateLevelWidget::KeyDownPressed);

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

void CreateLevelWidget::AddGraphicsItem(GraphicsObjectItem* p_graphicsItem) {
  m_scribblingView->AddGraphicsItem(p_graphicsItem);
}

void CreateLevelWidget::ShowDetailListView() {
  m_detailVertexStackWidget->setCurrentIndex(eDetailView);
}

void CreateLevelWidget::ShowVertexListView() {
  m_detailVertexStackWidget->setCurrentIndex(eVertexView);
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

void CreateLevelWidget::SetObjectsListModel(CreateLevelObjectsListModel* p_objectsListModel) {
  m_objectsListModel = p_objectsListModel;
  m_scribblingView->SetObjectsListModel(m_objectsListModel);
  m_objectsListTreeView->setModel(m_objectsListModel);
  m_objectsListTreeView->header()->setMinimumSectionSize(50);
  auto selectionModel = m_objectsListTreeView->selectionModel();
  m_scribblingView->SetSelectionModel(selectionModel);
  connect(selectionModel, &QItemSelectionModel::currentChanged, this, &CreateLevelWidget::CurrentObjectIndexChanged);

  connect(m_objectsListModel, &CreateLevelObjectsListModel::rowsInserted, this, [this]() {
    m_objectsListTreeView->expandAll();
    for (int column = m_objectsListModel->columnCount()-1; column > -1; --column) {
      m_objectsListTreeView->resizeColumnToContents(column);
    }
  });

  // Model signals
  connect(m_objectsListModel, &CreateLevelObjectsListModel::PolygonInserted, this, &CreateLevelWidget::UpdateView);
  connect(m_objectsListModel, &CreateLevelObjectsListModel::PolygonChanged, this, &CreateLevelWidget::UpdateView);
  connect(m_objectsListModel, &CreateLevelObjectsListModel::ObjectInserted, this, &CreateLevelWidget::UpdateView);
  connect(m_objectsListModel, &CreateLevelObjectsListModel::ObjectChanged, this, &CreateLevelWidget::UpdateView);
}

void CreateLevelWidget::SetObjectsDetailModel(CreateLevelObjectsDetailModel* p_objectsDetailModel) {
  m_objectsDetailModel = p_objectsDetailModel;
  m_objectsDetailTreeView->setModel(m_objectsDetailModel);
  m_objectsDetailTreeView->header()->setMinimumSectionSize(50);

  connect(m_objectsDetailModel, &CreateLevelObjectsListModel::rowsInserted, this, [this]() {
    m_objectsDetailTreeView->expandAll();
    for (int column = m_objectsDetailModel->columnCount()-1; column > -1; --column) {
      m_objectsDetailTreeView->resizeColumnToContents(column);
    }
  });
}

void CreateLevelWidget::SetVertexListModel(CreateLevelVertexListModel* p_vertexListModel) {
  m_vertexListModel = p_vertexListModel;
  m_vertexTreeView->setModel(m_vertexListModel);
  m_vertexTreeView->header()->setMinimumSectionSize(50);

  connect(m_vertexListModel, &CreateLevelVertexListModel::rowsInserted, this, [this]() {
    m_vertexTreeView->expandAll();
    for (int column = m_vertexListModel->columnCount()-1; column > -1; --column) {
      m_vertexTreeView->resizeColumnToContents(column);
    }
  });
}

QModelIndex CreateLevelWidget::GetCurrentObjectIndex() const {
  auto currentIndex = FindCurrentObjectIndex();
  Q_ASSERT_X(currentIndex.isValid(), QString("%1 is not an object index").arg(
    currentIndex.data().toString()).toStdString().c_str(), "CreateLevelWidget::GetCurrentObjectIndex");

  return currentIndex;
}

QModelIndex CreateLevelWidget::FindCurrentObjectIndex() const {
  auto currentIndex = m_objectsListTreeView->selectionModel()->currentIndex();
  if (m_objectsListModel->IsObjectIndex(currentIndex)) {
    return currentIndex;
  }

  return QModelIndex();
}

QModelIndex CreateLevelWidget::GetCurrentPolygonIndex() const {
  auto currentIndex = FindCurrentPolygonIndex();
  Q_ASSERT_X(currentIndex.isValid(), QString("%1 is not a polygon index").arg(
    currentIndex.data().toString()).toStdString().c_str(), "CreateLevelWidget::GetCurrentPolygonIndex");

  return currentIndex;
}

QModelIndex CreateLevelWidget::FindCurrentPolygonIndex() const {
  auto currentIndex = m_objectsListTreeView->selectionModel()->currentIndex();
  if (m_objectsListModel->IsPolygonIndex(currentIndex)) {
    return currentIndex;
  }

  return QModelIndex();
}

QModelIndex CreateLevelWidget::GetCurrentVertexIndex() const {
  auto currentIndex = FindCurrentVertexIndex();
  Q_ASSERT_X(currentIndex.isValid(), QString("%1 is not a vertex index").arg(
    currentIndex.data().toString()).toStdString().c_str(), "CreateLevelWidget::GetCurrentVertexIndex");

  return currentIndex;
}

QModelIndex CreateLevelWidget::FindCurrentVertexIndex() const {
  return m_vertexTreeView->selectionModel()->currentIndex();
}

void CreateLevelWidget::SetCurrentVertexIndex(int p_vertexRow) {
  m_vertexTreeView->selectionModel()->setCurrentIndex(m_vertexListModel->index(p_vertexRow, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void CreateLevelWidget::SetCurrentObjectOrPolygonIndex(QModelIndex const& p_index) {
  if (p_index.isValid()) {
    m_objectsListTreeView->selectionModel()->setCurrentIndex(p_index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
  } else {
    m_objectsListTreeView->selectionModel()->clearCurrentIndex();
  }
}

void CreateLevelWidget::ClearImage() {
  m_scribblingView->ClearImage();
}

void CreateLevelWidget::SetTestAvailable(bool p_enable)
{
  m_testLevelButton->setEnabled(p_enable);
}

void CreateLevelWidget::UpdateView() {
  m_scribblingView->UpdateView();
}

bool CreateLevelWidget::ConfirmClear() {
  return (GetGraphicsItemCount() == 1 ||
    QMessageBox::question(this, tr("Clear"), tr("You are currently creating a level, your modifications will be lost.\nContinue?")) == QMessageBox::Yes);
}

void CreateLevelWidget::ConfirmNewLevel() {
  if (ConfirmClear()) {
    Q_EMIT NewLevelRequested();
  }
}

void CreateLevelWidget::ConfirmOpenLevel() {
  if (ConfirmClear()) {
    auto fileName = QFileDialog::getOpenFileName(this, "Open level", ":/world1", "POLYPIXEL Files (*.ppxl)");
    if (!fileName.isEmpty()) {
      Q_EMIT OpenLevelRequested(fileName);
    }
  }
}

int CreateLevelWidget::GetGraphicsItemCount() const {
  return m_scribblingView->GetGraphicsItemCount();
}
