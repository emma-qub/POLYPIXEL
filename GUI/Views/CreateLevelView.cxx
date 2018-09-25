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

CreateLevelView::CreateLevelView(QWidget* parent):
  QWidget(parent),
  m_createLevelLabel(new QLabel("Create your level")),
  m_testLevelButton(new QPushButton("Test")),
  m_menuButton(new QPushButton("Menu")),
  m_scribblingView(new CreateLevelScribblingView),
  m_treeView(new QTreeView),
  m_undoView(new QUndoView) {

  m_treeView->setHeaderHidden(true);

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_createLevelLabel);
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

  auto itemDelegate = new PolygonItemDelegate(m_treeView);
  m_treeView->setItemDelegate(itemDelegate);
  connect(itemDelegate, &PolygonItemDelegate::ValueXChanged, this, &CreateLevelView::ValueXChanged);
  connect(itemDelegate, &PolygonItemDelegate::ValueYChanged, this, &CreateLevelView::ValueYChanged);
  connect(itemDelegate, &PolygonItemDelegate::EditionXDone, this, &CreateLevelView::EditionXDone);
  connect(itemDelegate, &PolygonItemDelegate::EditionYDone, this, &CreateLevelView::EditionYDone);
}

void CreateLevelView::SetModel(CreateLevelModel* p_model) {
  m_model = p_model;
  m_scribblingView->SetModel(p_model);
  m_treeView->setModel(p_model);
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
