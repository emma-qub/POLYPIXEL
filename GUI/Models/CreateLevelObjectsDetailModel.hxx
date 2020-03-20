#ifndef CREATELEVELOBJECTSDETAILMODEL_HXX
#define CREATELEVELOBJECTSDETAILMODEL_HXX

#include <QStandardItemModel>

class Object;

class CreateLevelObjectsDetailModel: public QStandardItemModel
{
  Q_OBJECT

public:
  CreateLevelObjectsDetailModel(QObject* p_parent = nullptr);
  ~CreateLevelObjectsDetailModel() override;

  void ResetCurrentObject(Object* p_object);
  void UpdateCurrentObject();

private:
  QStandardItem* m_tapeX1Item;
  QStandardItem* m_tapeY1Item;
  QStandardItem* m_tapeX2Item;
  QStandardItem* m_tapeY2Item;
  QStandardItem* m_tapeWItem;
  QStandardItem* m_tapeHItem;
  QStandardItem* m_mirrorX1Item;
  QStandardItem* m_mirrorY1Item;
  QStandardItem* m_mirrorX2Item;
  QStandardItem* m_mirrorY2Item;
  QStandardItem* m_oneWayX1Item;
  QStandardItem* m_oneWayY1Item;
  QStandardItem* m_oneWayX2Item;
  QStandardItem* m_oneWayY2Item;
  QStandardItem* m_oneWayNXItem;
  QStandardItem* m_oneWayNYItem;
  QStandardItem* m_portalInX1Item;
  QStandardItem* m_portalInY1Item;
  QStandardItem* m_portalInX2Item;
  QStandardItem* m_portalInY2Item;
  QStandardItem* m_portalInNXItem;
  QStandardItem* m_portalInNYItem;
  QStandardItem* m_portalOutX1Item;
  QStandardItem* m_portalOutY1Item;
  QStandardItem* m_portalOutX2Item;
  QStandardItem* m_portalOutY2Item;
  QStandardItem* m_portalOutNXItem;
  QStandardItem* m_portalOutNYItem;

  Object* m_object;
};

#endif
