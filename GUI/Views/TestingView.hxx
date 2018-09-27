#ifndef TESTINGVIEW_HXX
#define TESTINGVIEW_HXX

#include "GUI/Views/PlayingView.hxx"

class TestingView : public PlayingView
{
public:
  TestingView(QWidget* p_parent = nullptr);

  void DrawText(ppxl::Point p_position, const QString& p_text, int p_weight);
};

#endif
