#ifndef CREATELEVELMODEL_HXX
#define CREATELEVELMODEL_HXX

#include "Core/Geometry/Polygon.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Point.hxx"
#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Deviations/Portal.hxx"

#include <QStandardItemModel>
#include <QStack>
#include <QPair>

class Object;

class CreateLevelModel: public QStandardItemModel {
  Q_OBJECT

public:
  enum ItemRole {
    ePolygonRole = Qt::UserRole,
    eItemTypeRole,
    eStateRole
  };
  enum ItemType {
    ePolygons,
    ePolygon,
    eVertex,
    eX,
    eY
  };
  enum State {
    eSelected,
    eEnabled,
    eDisabled,
    eHighlightUp,
    eHighlightDown
  };

  using Selection = QPair<int, int>;
  using SelectionStack = QStack<Selection>;

  CreateLevelModel(QObject* p_parent = nullptr);
  ~CreateLevelModel() override;

  inline SelectionStack GetSelection() const { return m_selections; }
  inline void PushSelection(int p_polygonRow, int p_vertexRow) { m_selections << Selection({p_polygonRow, p_vertexRow}); }
  inline Selection PopSelection() { return m_selections.pop(); }

  // Polygon
  void SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void TranslatePolygon(int p_polygonRow, ppxl::Vector const& p_direction);

  // Vertex
  void InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void RemoveVertexAt(int p_polygonRow, int p_vertexRow);
  void TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction);
  QModelIndex GetVertexIndex(int p_polygonRow, int p_vertexRow) const;

  inline QColor GetColor() const { return m_color; }
  void InitColor();

  void InsertPolygon(int p_row, ppxl::Polygon const& p_polygon);
  void AppendPolygon(ppxl::Polygon const& p_polygon);
  inline int GetPolygonsCount() const { return m_polygonsItem->rowCount(); }
  inline QStandardItem* GetPolygonsItem() const { return m_polygonsItem; }
  inline QList<ppxl::Polygon*> GetPolygonsList() const { return m_polygons; }
  void SetPolygonsList(QList<ppxl::Polygon> const& p_polygonsList);
  void RemovePolygonAt(int p_polygonRow);
  void ClearPolygons();

  void AddTape(Tape const& p_tape);
  inline QList<Tape*> GetTapesList() const { return m_tapesList; }
  void SetTape(int p_index, Object* p_object);

  void AddMirror(Mirror const& p_mirror);
  inline QList<Mirror*> GetMirrorsList() const { return m_mirrorsList; }
  void SetMirror(int p_index, Object* p_object);

  void AddOneWay(OneWay const& p_oneWay);
  inline QList<OneWay*> GetOneWaysList() const { return m_oneWaysList; }
  void SetOneWay(int p_index, Object* p_object);

  void AddPortal(Portal const& p_portal);
  inline QList<Portal*> GetPortalsList() const { return m_portalsList; }
  void SetPortal(int p_index, Object* p_object);

  inline QList<Object*> GetObjectsList() const { return m_objectItemMap.keys(); }
  inline QStandardItem* GetItemFromObject(Object* p_object) const { return m_objectItemMap[p_object]; }

protected:
  inline void RegisterObject(Object* p_object, QStandardItem* p_item) { m_objectItemMap[p_object] = p_item; }

signals:
  void PolygonListChanged();

private:
  SelectionStack m_selections;

  QStandardItem* m_polygonsItem;
  QStandardItem* m_tapesItem;
  QStandardItem* m_mirrorsItem;
  QStandardItem* m_oneWaysItem;
  QStandardItem* m_portalsItem;

  QList<ppxl::Polygon*> m_polygons;
  QMap<Object*, QStandardItem*> m_objectItemMap;
  QList<Tape*> m_tapesList;
  QList<Mirror*> m_mirrorsList;
  QList<OneWay*> m_oneWaysList;
  QList<Portal*> m_portalsList;

  QColor m_color;
};

Q_DECLARE_METATYPE(ppxl::Polygon*)
Q_DECLARE_METATYPE(CreateLevelModel::ItemType);

#endif
