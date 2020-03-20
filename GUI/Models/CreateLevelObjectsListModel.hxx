#ifndef CREATELEVELOBJECTSLISTMODEL_HXX
#define CREATELEVELOBJECTSLISTMODEL_HXX

#include "Core/Geometry/Polygon.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Point.hxx"
#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Deviations/Portal.hxx"
#include "GUI/GraphicsItem/GraphicsObjectItem.hxx"

#include <QStandardItemModel>
#include <QStack>
#include <QPair>

class Object;

class CreateLevelObjectsListModel: public QStandardItemModel {
  Q_OBJECT

public:
  enum ItemRole {
    eIsPolygonRole = Qt::UserRole, // bool
    ePolygonRole, // Polygon*
    eIsObjectRole, // bool
    eObjectRole, // Object*
    eObjectTypeRole, // ObjectType
    eIsListRole, // bool
    eListTypeRole, // ListType
    eGraphicsItemRole // QGraphicsItem*
  };
  enum ObjectType {
    eUnknownObjectType,
    ePolygonObjectType,
    eTapeObjectType,
    eMirrorObjectType,
    eOneWayObjectType,
    ePortalObjectType,
  };
  enum ListType {
    eUnknownObjectListType,
    ePolygonsListType,
    eTapesListType,
    eMirrorsListType,
    eOneWaysListType,
    ePortalsListType
  };

  using Selection = QPair<int, int>;
  using SelectionStack = QStack<Selection>;

  CreateLevelObjectsListModel(QObject* p_parent = nullptr);
  ~CreateLevelObjectsListModel() override;

  inline SelectionStack GetSelection() const { return m_selections; }
  inline void PushSelection(int p_polygonRow, int p_vertexRow) { m_selections << Selection({p_polygonRow, p_vertexRow}); }
  inline Selection PopSelection() { return m_selections.pop(); }

  // Polygon
  bool IsPolygonItem(QStandardItem* p_item) const;
  bool IsPolygonIndex(QModelIndex const& p_index) const;
  ppxl::Polygon* GetPolygonFromIndex(QModelIndex const& p_index) const;
  ppxl::Polygon* GetPolygonFromItem(QStandardItem* p_item) const;
  ppxl::Polygon* GetPolygonFromRow(int p_row) const;
  ppxl::Polygon* FindPolygonFromIndex(QModelIndex const& p_index) const;
  ppxl::Polygon* FindPolygonFromItem(QStandardItem* p_item) const;
  ppxl::Polygon* FindPolygonFromRow(int p_row) const;

  QColor GetRandomColor();

  inline int GetPolygonsCount() const { return m_polygonsItem->rowCount(); }
  inline QStandardItem* GetPolygonsItem() const { return m_polygonsItem; }
  inline QStandardItem* GetTapesItem() const { return m_tapesItem; }
  inline QStandardItem* GetMirrorsItem() const { return m_mirrorsItem; }
  inline QStandardItem* GetOneWaysItem() const { return m_oneWaysItem; }
  inline QStandardItem* GetPortalsItem() const { return m_portalsItem; }
  QList<ppxl::Polygon*> GetPolygonsList() const;

  QStandardItem* AddPolygon(ppxl::Polygon* p_polygon, GraphicsPolygonItem* p_graphicsObjectItem);
  void TranslatePolygon(int p_polygonRow, const ppxl::Vector& p_direction);
  void InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void SetVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);

  // Object
  bool IsObjectItem(QStandardItem* p_item) const;
  bool IsObjectIndex(QModelIndex const& p_index) const;
  bool IsNotPolygonItem(QStandardItem* p_item) const;
  bool IsNotPolygonIndex(QModelIndex const& p_index) const;
  Object* GetObjectFromItem(QStandardItem* p_item) const;
  Object* GetObjectFromIndex(QModelIndex const& p_index) const;
  Object* FindObjectFromItem(QStandardItem* p_item) const;
  Object* FindObjectFromIndex(QModelIndex const& p_index) const;
  //  Tape
  QStandardItem* AddTape(Object* p_tape, GraphicsObjectItem* p_tapeGraphicsItem);
  //  Mirror
  QStandardItem* AddMirror(Object* p_mirror, GraphicsObjectItem* p_mirrorGraphicsItem);
  //  One Way
  QStandardItem* AddOneWay(Object* p_oneWay, GraphicsObjectItem* p_oneWayGraphicsItem);
  //  Portal
  QStandardItem* AddPortal(Object* p_portal, GraphicsObjectItem* p_portalGraphicsItem);
  //  Move
  void MoveObject(QModelIndex const& p_objectIndex, ppxl::Point const& p_pos, Object::ControlPointType p_controlPointType);

  // All
  void Clear();
  void SetGraphicsToItem(GraphicsObjectItem* p_graphicsItem, QStandardItem* p_item);
  GraphicsObjectItem* GetGraphicsFromItem(QStandardItem* p_item) const;
  GraphicsObjectItem* GetGraphicsFromIndex(QModelIndex const& p_index) const;  
  ListType GetListTypFromIndex(const QModelIndex& p_index) const;

Q_SIGNALS:
  void PolygonListChanged();

  void ObjectInserted();
  void ObjectChanged();
  void PolygonInserted();
  void PolygonChanged();

private:
  QStandardItem* AddObject(Object* p_object, GraphicsObjectItem* p_graphicsObjectItem, QStandardItem* p_listItem);

  void SetDefaultItems();

  SelectionStack m_selections;

  QStandardItem* m_polygonsItem;
  QStandardItem* m_tapesItem;
  QStandardItem* m_mirrorsItem;
  QStandardItem* m_oneWaysItem;
  QStandardItem* m_portalsItem;
};

Q_DECLARE_METATYPE(ppxl::Polygon*)
Q_DECLARE_METATYPE(Object*)
Q_DECLARE_METATYPE(GraphicsObjectItem*);
Q_DECLARE_METATYPE(CreateLevelObjectsListModel::ItemRole);
Q_DECLARE_METATYPE(CreateLevelObjectsListModel::ObjectType);
Q_DECLARE_METATYPE(CreateLevelObjectsListModel::ListType);

#endif
