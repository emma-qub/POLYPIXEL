#ifndef OPTIONVIEW_HXX
#define OPTIONVIEW_HXX

#include <QWidget>

class QLabel;
class QPushButton;

class OptionsView : public QWidget
{
  Q_OBJECT
public:
  explicit OptionsView(QWidget* parent = nullptr);

signals:
  void Done();

public slots:

private:
  QLabel* m_optionsLabel;
  QPushButton* m_menuButton;
};

#endif
