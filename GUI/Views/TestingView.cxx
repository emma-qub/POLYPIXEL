#include "TestingView.hxx"

#include "GUI/Views/GameScribblingView.hxx"

TestingView::TestingView(QWidget* p_parent):
  PlayingView(p_parent) {
}

void TestingView::DrawText(ppxl::Point p_position, const QString& p_text, int p_weight) {
  m_scribblingView->DrawText(p_position, p_text, p_weight);
}
