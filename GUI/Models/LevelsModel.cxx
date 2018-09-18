#include "LevelsModel.hxx"

LevelsModel::LevelsModel(QObject* p_parent):
  QStandardItemModel(p_parent) {
}

void LevelsModel::InsertLevel(int p_row, QFileInfo const& p_entry)
{
  auto levelName = p_entry.baseName();
  auto imagePath = p_entry.absoluteFilePath()+"/preview.png";
  auto levelsPath = p_entry.absoluteFilePath()+"/level.xml";
  auto item = new QStandardItem(QIcon(imagePath), levelName);
  item->setData(levelsPath, eLevelPathRole);
  insertRow(p_row, item);
}

void LevelsModel::AppendLevel(QFileInfo const& p_entry)
{
  InsertLevel(rowCount(), p_entry);
}
