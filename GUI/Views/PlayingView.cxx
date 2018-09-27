#include "PlayingView.hxx"

#include "GUI/Models/PolygonModel.hxx"
#include "GUI/Views/GameScribblingView.hxx"

#include <QVBoxLayout>
#include <QLabel>
#include <QPoint>

PlayingView::PlayingView(QWidget* p_parent):
  QWidget(p_parent),
  m_polygonModel(nullptr),
  m_scribblingView(new GameScribblingView),
  m_linesCount(-1),
  m_linesGoal(-1),
  m_linesCountLabel(new QLabel),
  m_partsCount(-1),
  m_partsGoal(-1),
  m_partsCountLabel(new QLabel)
{

  auto mainLayout = new QVBoxLayout;
  auto menuLayout = new QHBoxLayout;
  menuLayout->addWidget(m_linesCountLabel);
  menuLayout->addWidget(m_partsCountLabel);
  menuLayout->setAlignment(Qt::AlignCenter);
  mainLayout->addLayout(menuLayout);
  mainLayout->addWidget(m_scribblingView);
  mainLayout->setStretchFactor(menuLayout, 0);
  mainLayout->setStretchFactor(m_scribblingView, 1);
  setLayout(mainLayout);

  connect(m_scribblingView, &GameScribblingView::Scribbling, this, &PlayingView::Scribbling);
  connect(m_scribblingView, &GameScribblingView::Moving, this, &PlayingView::Moving);
  connect(m_scribblingView, &GameScribblingView::Slicing, this, &PlayingView::Slicing);
}

void PlayingView::SetModel(PolygonModel* p_playingModel) {
  m_scribblingView->SetModel(p_playingModel);
}

void PlayingView::DrawLine(const ppxl::Segment& p_line, QColor const& p_color, Qt::PenStyle p_penStyle) {
  m_scribblingView->DrawLine(p_line, p_color, p_penStyle);
}

void PlayingView::DrawFromModel() {
  m_scribblingView->DrawFromModel();
}

void PlayingView::DrawAreas(QList<double> const& p_areas) {
  m_scribblingView->DrawAreas(p_areas);
}

void PlayingView::ClearImage() {
  m_scribblingView->ClearImage();
}

void PlayingView::UpdateLinesCount(int p_linesCount, int p_linesGoal) {
  if (p_linesGoal != -1)
  {
    m_linesGoal = p_linesGoal;
  }

  m_linesCount = p_linesCount;
  m_linesCountLabel->setText(tr("Lines: %1/%2").arg(m_linesCount).arg(m_linesGoal));
}

void PlayingView::UpdatePartsCount(int p_partsCount, int p_partsGoal) {
  if (p_partsGoal != -1)
  {
    m_partsGoal = p_partsGoal;
  }

  m_partsCount = p_partsCount;
  m_partsCountLabel->setText(tr("Parts: %1/%2").arg(m_partsCount).arg(m_partsGoal));
}
