#include "PolygonModel.hxx"
#include "PolygonItem.hxx"

#include <random>

PolygonModel::PolygonModel(QObject* p_parent):
  QStandardItemModel(p_parent),
  m_polygonsItem(new QStandardItem("Polygons")),
  m_color() {

  setColumnCount(3);

  m_polygonsItem->setData(ePolygons, eItemTypeRole);
  appendRow(m_polygonsItem);
}

PolygonModel::~PolygonModel() {
  for (auto* polygon: m_polygons) {
    delete polygon;
  }
}

void PolygonModel::InitColor() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> distribution(0, 255);

  auto r = distribution(rng);
  auto g = distribution(rng);
  auto b = distribution(rng);

  m_color = QColor(r, g, b);
}

void PolygonModel::InsertPolygon(int p_row, ppxl::Polygon const& p_polygon) {
  auto polygon = new ppxl::Polygon(p_polygon);
  auto polygonItem = new PolygonItem(polygon, m_color);
  polygonItem->setData(ePolygon, eItemTypeRole);
  polygonItem->setData(QVariant::fromValue<ppxl::Polygon*>(polygon), ePolygonRole);

  m_polygonsItem->insertRow(p_row, QList<QStandardItem*>() << polygonItem << new QStandardItem << new QStandardItem);
  m_polygons << polygon;
}

void PolygonModel::AppendPolygon(ppxl::Polygon const& p_polygon) {
  InsertPolygon(m_polygonsItem->rowCount(), p_polygon);
}

void PolygonModel::SetPolygonsList(QList<ppxl::Polygon> const& p_polygonsList) {
  ClearPolygons();
  for (auto const& polygon: p_polygonsList) {
    AppendPolygon(polygon);
  }

  Q_EMIT(PolygonListChanged());
}

void PolygonModel::RemovePolygonAt(int p_polygonRow) {
  delete m_polygons.takeAt(p_polygonRow);
  for (auto* item: m_polygonsItem->takeRow(p_polygonRow))
  {
    delete item;
  }
}

void PolygonModel::ClearPolygons() {
  for (int row = m_polygonsItem->rowCount()-1; row > -1; --row)
  {
    RemovePolygonAt(row);
  }
  m_polygons.clear();
  m_polygonsItem->setRowCount(0);
  m_polygonsItem->setColumnCount(0);
}
