#ifndef GAMEMODEL_HXX
#define GAMEMODEL_HXX

#include "GUI/Models/AbstractLevelModel.hxx"

class GameModel: public AbstractLevelModel {
  Q_OBJECT

public:
  GameModel(QObject* p_parent = nullptr);
  ~GameModel() override;

};

#endif
