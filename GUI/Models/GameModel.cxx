#include "GameModel.hxx"

GameModel::GameModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_polygonsItem(new QStandardItem("Polygons")){

  appendRow(m_polygonsItem);
}

QList<QStandardItem*> GameModel::GetPolygonItemsList() const
{
  QList<QStandardItem*> polygonItems;

  for (int row = 0; row < m_polygonsItem->rowCount(); ++row) {
    polygonItems << m_polygonsItem->child(row, 0);
  }

  return polygonItems;
}

QList<ppxl::Polygon> GameModel::GetPolygonsList() const
{
  QList<ppxl::Polygon> polygonsList;
  for (int row = 0; row < m_polygonsItem->rowCount(); ++row) {
    polygonsList << m_polygonsItem->child(row, 0)->data(ePolygonRole).value<ppxl::Polygon>();
  }

  return polygonsList;
}

void GameModel::SetPolygonsList(const QList<ppxl::Polygon>& p_polygonsList)
{
  ClearPolygons();
  for (auto const& polygon: p_polygonsList) {
    AppendPolygon(polygon);
  }

  Q_EMIT(PolygonListChanged());
}

GameModel::~GameModel() = default;

void GameModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygonItem = new QStandardItem("Polygon #"+QString::number(p_row));
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon>(p_polygon), ePolygonRole);
  polygonItem->setData(QColor("#d64e9a"), Qt::DecorationRole);
  char i = 'A';

  for (auto vertex: p_polygon.GetVertices()) {
    auto vertexItem = new QStandardItem(QString(i));
    auto xItem = new QStandardItem(QString::number(vertex.GetX()));
    auto yItem = new QStandardItem(QString::number(vertex.GetY()));
    vertexItem->appendRow(QList<QStandardItem*>() << xItem << yItem);
    polygonItem->appendRow(vertexItem);
    i++;
  }

  m_polygonsItem->insertRow(p_row, polygonItem);
}

void GameModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_polygonsItem->rowCount(), p_polygon);
}

void GameModel::ClearPolygons() {
  for (int row = m_polygonsItem->rowCount()-1; row > -1; --row)
  {
    for (auto* item: m_polygonsItem->takeRow(row))
    {
      delete item;
    }
  }
}
