#ifndef CREATELEVELSCRIBBLINGVIEW_HXX
#define CREATELEVELSCRIBBLINGVIEW_HXX

#include "Core/Geometry/Point.hxx"
#include "Core/Geometry/Vector.hxx"
#include "Core/Geometry/Polygon.hxx"

#include <QGraphicsView>

class GraphicsObjectItem;
class CreateLevelObjectsListModel;
class ObjectModel;
class QItemSelectionModel;
class QStandardItem;

class CreateLevelScribblingView: public QGraphicsView {
  Q_OBJECT

public:
  CreateLevelScribblingView(QWidget* parent = nullptr);
  ~CreateLevelScribblingView() override;

  void InitView();

  void SetObjectsListModel(CreateLevelObjectsListModel* p_objectsListModel);
  void SetObjectModelsList(QList<ObjectModel*> const& p_objectModelsList);
  void SetSelectionModel(QItemSelectionModel* p_selectionModel);
  void SetCurrentPolygonItem(QStandardItem* p_currentPolygonItem);
  void SetCurrentObjectItem(QStandardItem* p_currentObjectItem);

  void ClearImage();
  void UpdateView();

  void AddGraphicsItem(GraphicsObjectItem* p_graphicsItem);

Q_SIGNALS:
  void SnappedToGrid();
  void NewLevelRequested();
  void OpenLevelRequested(QString const& p_fileName);
  void MousePressed(QMouseEvent* p_event);
  void MouseMoved(QMouseEvent* p_event);
  void MouseReleased(QMouseEvent* p_event);

protected:
  void mousePressEvent(QMouseEvent* p_event) override;
  void mouseMoveEvent(QMouseEvent* p_event) override;
  void mouseReleaseEvent(QMouseEvent* p_event) override;

private:
  QGraphicsScene* m_scene;
  QGraphicsPixmapItem* m_gridPixmapItem;
  CreateLevelObjectsListModel* m_objectsListModel;
  QItemSelectionModel* m_objectSelectionModel;
  bool m_viewInitialized;
};

#endif
