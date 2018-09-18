#include "WorldsModel.hxx"

WorldsModel::WorldsModel(QObject* p_parent):
  QStandardItemModel(p_parent) {
}

void WorldsModel::InsertWorld(int p_row, QFileInfo const& p_entry)
{
  auto worldName = p_entry.baseName().mid(1).toUpper();
  auto imagePath = p_entry.absoluteFilePath()+"/preview.png";
  auto levelsPath = p_entry.absoluteFilePath()+"/levels";
  auto item = new QStandardItem(QIcon(imagePath), worldName);
  item->setData(levelsPath, eLevelsPathRole);
  insertRow(p_row, item);
}

void WorldsModel::AppendWorld(QFileInfo const& p_entry)
{
  InsertWorld(rowCount(), p_entry);
}
