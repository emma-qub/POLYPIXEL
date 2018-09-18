#ifndef LEVELSMODEL_HXX
#define LEVELSMODEL_HXX

#include <QStandardItemModel>
#include <QFileInfo>

class LevelsModel: public QStandardItemModel {
  Q_OBJECT

public:
  enum ItemRoles {
    eLevelPathRole = Qt::UserRole
  };

  LevelsModel(QObject* p_parent = nullptr);

  void InsertLevel(int p_row, QFileInfo const& p_entry);
  void AppendLevel(QFileInfo const& p_entry);
};

#endif
