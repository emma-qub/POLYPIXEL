#ifndef TESTLEVELWIDGET_HXX
#define TESTLEVELWIDGET_HXX

#include <QWidget>

class TestLevelWidget: public QWidget {
  Q_OBJECT

public:
  explicit TestLevelWidget(QWidget* p_parent = nullptr);

  void InitView();

Q_SIGNALS:
  void AmendLevelRequested();
  void Done();
};

#endif
