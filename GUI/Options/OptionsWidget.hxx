#ifndef OPTIONWIDGET_HXX
#define OPTIONWIDGET_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class OptionsWidget: public QWidget {
  Q_OBJECT

public:
  explicit OptionsWidget(QWidget* parent = nullptr);

Q_SIGNALS:
  void Done();

private:
  QLabel* m_optionsLabel;
  QPushButton* m_menuButton;
};

#endif
