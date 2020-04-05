#include "TestLevelController.hxx"

#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Deviations/Portal.hxx"

#include "GUI/TestLevel/Views/TestLevelWidget.hxx"
#include "GUI/CreateLevel/Models/GraphicsObjectItem.hxx"

#include <QMouseEvent>

TestLevelController::TestLevelController(TestLevelWidget* p_testLevelWidget, QObject* p_parent):
  QObject(p_parent),
  m_testLevelWidget(p_testLevelWidget),
  m_polygonsList(),
  m_objectsList(),
  m_slicer(),
  m_graphicsPolygonItemsList(),
  m_polygonsColor(),
  m_colorPicked(false) {

  connect(m_testLevelWidget, &TestLevelWidget::MousePressed, this, &TestLevelController::MousePressEvent);
  connect(m_testLevelWidget, &TestLevelWidget::MouseMoved, this, &TestLevelController::MouseMoveEvent);
  connect(m_testLevelWidget, &TestLevelWidget::MouseReleased, this, &TestLevelController::MouseReleaseEvent);
}

void TestLevelController::SetLinesGoal(int LinesGoal) {

}

void TestLevelController::SetPartsGoal(int PartsGoal) {

}

void TestLevelController::SetMaxGapToWin(int MaxGapToWin) {

}

void TestLevelController::SetTolerance(int Tolerance) {

}

void TestLevelController::InitPolygonsList(std::vector<ppxl::Polygon*> const& p_polygonsList) {
  for (auto polygon: p_polygonsList) {
    m_polygonsList.push_back(*polygon);
  }
  m_slicer.SetPolygonsList(m_polygonsList);
}

void TestLevelController::SetPolygonsList(const std::vector<ppxl::Polygon>& p_polygonsList) {
  m_polygonsList.clear();
  m_polygonsList = p_polygonsList;
  m_slicer.SetPolygonsList(m_polygonsList);
}

void TestLevelController::SetObjectModelsList(std::vector<Object*> const& p_objectsList) {
  m_objectsList = p_objectsList;
  m_slicer.SeObjectsList(m_objectsList);
}

void TestLevelController::PlayLevel() {
  SetPolygonItems();
  SetObjectItems();
}

void TestLevelController::SetPolygonItems() {
  for (auto polygonItem: m_graphicsPolygonItemsList) {
    polygonItem->DeletePolygon();
    delete polygonItem;
  }
  m_graphicsPolygonItemsList.clear();

  for (auto polygon: m_polygonsList) {
    auto polygonItem = new GraphicsPolygonItem(new ppxl::Polygon(polygon));
    if (m_colorPicked) {
      polygonItem->SetColor(m_polygonsColor);
    } else {
      m_polygonsColor = polygonItem->GetColor();
      m_colorPicked = true;
    }
    m_graphicsPolygonItemsList.push_back(polygonItem);
    m_testLevelWidget->AddGraphicsItem(polygonItem);
  }
}

void TestLevelController::SetObjectItems() {
  for (auto object: m_objectsList) {
    switch (object->GetObjectType()) {
    case Object::eTape:{
      m_testLevelWidget->AddGraphicsItem(new GraphicsTapeItem(static_cast<Tape*>(object)));
      break;
    } case Object::eMirror:{
      m_testLevelWidget->AddGraphicsItem(new GraphicsMirrorItem(static_cast<Mirror*>(object)));
      break;
    } case Object::eOneWay:{
      m_testLevelWidget->AddGraphicsItem(new GraphicsOneWayItem(static_cast<OneWay*>(object)));
      break;
    } case Object::ePortal:{
      m_testLevelWidget->AddGraphicsItem(new GraphicsPortalItem(static_cast<Portal*>(object)));
      break;
    } default:
      break;
    }
  }
}

void TestLevelController::MousePressEvent(QMouseEvent* p_event) {
  m_testLevelWidget->CuttingStarted();
  m_slicer.SetStartPoint(ppxl::Point(p_event->pos().x(), p_event->pos().y()));
}

void TestLevelController::MouseMoveEvent(QMouseEvent* p_event) {
  auto lines = m_slicer.ComputeSlicingLines(ppxl::Point(p_event->pos().x(), p_event->pos().y()));
  switch (m_slicer.ComputeLinesType(lines)) {
  case Slicer::eGoodCrossing: {
    m_testLevelWidget->SetGoodCutState();
    break;
  } case Slicer::eBadCrossing: {
    m_testLevelWidget->SetBadCutState();
    break;
  } case Slicer::eNoCrossing:
    default: {
    m_testLevelWidget->SetNoCutState();
    break;
  }
  }
  m_testLevelWidget->SetCuttingLines(lines);
}

void TestLevelController::MouseReleaseEvent(QMouseEvent* p_event) {
  m_slicer.SliceIt(ppxl::Point(p_event->pos().x(), p_event->pos().y()));
  SetPolygonsList(m_slicer.GetPolygonsList());
  SetPolygonItems();

  m_testLevelWidget->SetCuttingLines({});
  m_testLevelWidget->CuttingEnded();
}
