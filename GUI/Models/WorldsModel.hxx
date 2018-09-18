#ifndef WORLDSMODEL_HXX
#define WORLDSMODEL_HXX

#include <QStandardItemModel>
#include <QFileInfo>

class WorldsModel: public QStandardItemModel {
  Q_OBJECT

public:
  enum ItemRoles {
    eLevelsPathRole = Qt::UserRole
  };

  explicit WorldsModel(QObject* p_parent = nullptr);

  void InsertWorld(int p_row, QFileInfo const& p_entry);
  void AppendWorld(QFileInfo const& p_entry);
};

#endif
