#ifndef CHOOSELEVELSCONTROLLER_HXX
#define CHOOSELEVELSCONTROLLER_HXX

#include <QObject>

class ChooseLevelWidget;

class ChooseLevelController: public QObject {
  Q_OBJECT

public:
  explicit ChooseLevelController(ChooseLevelWidget* p_chooseLevelWidget, QObject* p_parent = nullptr);

Q_SIGNALS:

};

#endif
