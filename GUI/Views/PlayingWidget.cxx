#include "PlayingWidget.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Views/PlayingScribblingView.hxx"

#include <cmath>

#include <QVBoxLayout>
#include <QLabel>
#include <QPoint>

PlayingWidget::PlayingWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_scribblingView(new PlayingScribblingView),
  m_linesCount(-1),
  m_linesGoal(-1),
  m_linesCountLabel(new QLabel),
  m_partsCount(-1),
  m_partsGoal(-1),
  m_partsCountLabel(new QLabel) {

  auto mainLayout = new QGridLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_linesCountLabel);
  menuLayout->addWidget(m_partsCountLabel, Qt::AlignRight);
  mainLayout->addLayout(menuLayout, 0, 0);
  mainLayout->addWidget(m_scribblingView);
  mainLayout->setRowStretch(0, 0);
  mainLayout->setRowStretch(1, 1);
  setLayout(mainLayout);

  connect(m_scribblingView, &PlayingScribblingView::Scribbling, this, &PlayingWidget::Scribbling);
  connect(m_scribblingView, &PlayingScribblingView::Moving, this, &PlayingWidget::Moving);
  connect(m_scribblingView, &PlayingScribblingView::Slicing, this, &PlayingWidget::Slicing);
  connect(m_scribblingView, &PlayingScribblingView::ControlPressed, this, &PlayingWidget::ControlPressed);
  connect(m_scribblingView, &PlayingScribblingView::ControlReleased, this, &PlayingWidget::ControlReleased);
  connect(m_scribblingView, &PlayingScribblingView::ReplayRequested, this, &PlayingWidget::ReplayRequested);
  connect(m_scribblingView, &PlayingScribblingView::GoToMapRequested, this, &PlayingWidget::GoToMapRequested);
}

void PlayingWidget::InitView() {
  m_scribblingView->InitView();
}

void PlayingWidget::SetModel(PolygonModel* p_playingModel) {
  m_scribblingView->SetPolygonModel(p_playingModel);
}

void PlayingWidget::SetObjectsList(QList<Object*> const& p_objectsList) {
  m_scribblingView->SetObjectsList(p_objectsList);
}

void PlayingWidget::DrawLine(ppxl::Segment const& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  m_scribblingView->DrawLine(p_line, p_color, p_penStyle);
}

void PlayingWidget::DrawFromModel() {
  m_scribblingView->DrawFromModel();
}

void PlayingWidget::DrawObjects() {
  m_scribblingView->DrawObjects();
}

void PlayingWidget::SetAreasData(QList<double> const& p_areas, QList<ppxl::Vector> p_shiftVectors, ppxl::Point const& p_figureCenter) {
  m_scribblingView->SetAreasData(p_areas, p_shiftVectors, p_figureCenter);
}

void PlayingWidget::ClearImage() {
  m_scribblingView->ClearImage();
}

void PlayingWidget::AnimatePolygons() {
  m_scribblingView->AnimatePolygons();
}

void PlayingWidget::UpdateLinesCount(int p_linesCount, int p_linesGoal) {
  if (p_linesGoal != -1)
  {
    m_linesGoal = p_linesGoal;
  }

  m_linesCount = p_linesCount;
  m_linesCountLabel->setText(tr("Lines: %1").arg(std::max(m_linesGoal-m_linesCount, 0)));
}

void PlayingWidget::UpdatePartsCount(int p_partsCount, int p_partsGoal) {
  if (p_partsGoal != -1)
  {
    m_partsGoal = p_partsGoal;
  }

  m_partsCount = p_partsCount;
  m_partsCountLabel->setText(tr("Parts: %1/%2").arg(m_partsCount).arg(m_partsGoal));
}

void PlayingWidget::StartLevel() {
  m_scribblingView->SetCanScribble(true);
}

void PlayingWidget::EndLevel() {
  m_scribblingView->SetCanScribble(false);
}

void PlayingWidget::SetEndLevelInfo(int m_linesCount, int m_linesGoal, int m_partsCount, int m_partsGoal, int m_stars) {
  m_scribblingView->SetEndLevelInfo(m_linesCount, m_linesGoal, m_partsCount, m_partsGoal, m_stars);
}

void PlayingWidget::DisplayGameOver() {
  m_scribblingView->DisplayGameOver();
}
