#ifndef GAMEMODEL_HXX
#define GAMEMODEL_HXX

#include <QStandardItemModel>

#include "Core/Polygon.hxx"

class GameModel: public QStandardItemModel {
  Q_OBJECT

public:
  enum ItemRoles {
    ePolygonRole = Qt::UserRole
  };

  GameModel(QObject* p_parent = nullptr);
  ~GameModel() override;

  inline int GetPolygonsCount() const { return m_polygonsItem->rowCount(); }

  QList<QStandardItem*> GetPolygonItemsList() const;
  QList<ppxl::Polygon> GetPolygonsList() const;
  void SetPolygonsList(QList<ppxl::Polygon> const& p_polygonsList);
  void ClearPolygons();

signals:
  void PolygonListChanged();

protected:
  void InsertPolygon(int p_row, ppxl::Polygon const& p_polygon);
  void AppendPolygon(const ppxl::Polygon& p_polygon);

private:
  QStandardItem* m_polygonsItem;
};

Q_DECLARE_METATYPE(ppxl::Polygon)

#endif
