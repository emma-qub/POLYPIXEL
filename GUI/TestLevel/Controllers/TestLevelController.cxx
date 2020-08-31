#include "TestLevelController.hxx"

#include "Core/Objects/Obstacles/Tape.hxx"
#include "Core/Objects/Deviations/Mirror.hxx"
#include "Core/Objects/Obstacles/OneWay.hxx"
#include "Core/Objects/Deviations/Portal.hxx"

#include "GUI/TestLevel/Views/TestLevelWidget.hxx"
#include "GUI/CreateLevel/Models/GraphicsObjectItem.hxx"

#include "Parser/Serializer.hxx"

#include <QMouseEvent>
#include <QDir>

TestLevelController::TestLevelController(TestLevelWidget* p_testLevelWidget, QObject* p_parent):
  QObject(p_parent),
  m_testLevelWidget(p_testLevelWidget),
  m_polygonsList(),
  m_initialPolygonsList(),
  m_objectsList(),
  m_slicer(),
  m_graphicsPolygonItemsList(),
  m_polygonsColor(),
  m_colorPicked(false),
  m_linesGoal(-1),
  m_linesDrawn(0),
  m_partsGoal(-1),
  m_partsSplit(-1),
  m_maxGapToWin(-1),
  m_tolerance(-1) {

  connect(m_testLevelWidget, &TestLevelWidget::MousePressed, this, &TestLevelController::MousePressEvent);
  connect(m_testLevelWidget, &TestLevelWidget::MouseMoved, this, &TestLevelController::MouseMoveEvent);
  connect(m_testLevelWidget, &TestLevelWidget::MouseReleased, this, &TestLevelController::MouseReleaseEvent);

  connect(m_testLevelWidget, &TestLevelWidget::SaveRequested, this, &TestLevelController::SaveLevel);
  connect(m_testLevelWidget, &TestLevelWidget::AmendRequested, this, &TestLevelController::AmendLevel);
  connect(m_testLevelWidget, &TestLevelWidget::RetryRequested, this, &TestLevelController::RetryLevel);
}

void TestLevelController::SetLinesGoal(int p_linesGoal) {
  m_linesGoal = p_linesGoal;
}

void TestLevelController::SetPartsGoal(int p_partsGoal) {
  m_partsGoal = p_partsGoal;
}

void TestLevelController::SetMaxGapToWin(int p_maxGapToWin) {
  m_maxGapToWin = p_maxGapToWin;
}

void TestLevelController::SetTolerance(int p_tolerance) {
  m_tolerance = p_tolerance;
}

void TestLevelController::InitPolygonsList(std::vector<ppxl::Polygon*> const& p_polygonsList) {
  for (auto polygon: p_polygonsList) {
    m_polygonsList.push_back(*polygon);
    m_initialPolygonsList.push_back(*polygon);
  }
  m_slicer.SetPolygonsList(m_polygonsList);
  m_slicer.InitTotalOrientedArea();
  m_partsSplit = m_polygonsList.size();
}

void TestLevelController::ResetPolygonsList(std::vector<ppxl::Polygon> const& p_polygonsList) {
  m_polygonsList.clear();
  m_polygonsList = p_polygonsList;
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

  for (auto const& polygon: m_polygonsList) {
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

double TestLevelController::CheckWin() const {
  double gap = 100.;

  if (m_linesGoal == m_linesDrawn && m_partsGoal == m_partsSplit) {
    qDebug() << "CONGRATS";
    double minArea = 0.0;
    double maxArea = 0.0;
    m_slicer.ComputeAreas(minArea, maxArea);
    gap = maxArea-minArea;
    qDebug() << QString("Tolerance: %1 / %2").arg(m_tolerance).arg(gap);
  } else if (m_linesGoal > m_linesDrawn && m_partsGoal > m_partsSplit) {
    qDebug() << "Carry on";
  } else {
    qDebug() << "GAME OVER";
  }

  return gap;
}

void TestLevelController::SaveLevel() const {
  QDir worldsDir("../POLYPIXEL/worlds/");
  auto levelNumber = worldsDir.entryList(QDir::NoDotAndDotDot | QDir::Files).count();

  Serializer serializer(QString("%1/W1_L%2.ppxl").arg(worldsDir.absolutePath()).arg(100 + ++levelNumber));

  serializer.SetLinesGoal(m_linesGoal);
  serializer.SetPartsGoal(m_partsGoal);
  serializer.SetTolerance(m_tolerance);
  serializer.SetMaxGapToWin(m_maxGapToWin);

  for (auto k = 0u; k < m_initialPolygonsList.size(); ++k) {
    serializer.AppendPolygon(m_initialPolygonsList.at(k), k);
  }

  int tapesCount = -1;
  int mirrorsCount = -1;
  int oneWaysCount = -1;
  int portalsCount = -1;

  for (auto object: m_objectsList) {
    switch (object->GetObjectType()) {
    case Object::eTape: {
      auto tape = static_cast<Tape*>(object);
      serializer.AppendTape(*tape, ++tapesCount);
      break;
    } case Object::eMirror: {
      auto mirror = static_cast<Mirror*>(object);
      serializer.AppendMirror(*mirror, ++mirrorsCount);
      break;
    } case Object::eOneWay: {
      auto oneWay = static_cast<OneWay*>(object);
      serializer.AppendOneWay(*oneWay, ++oneWaysCount);
      break;
    } case Object::ePortal: {
      auto portal = static_cast<Portal*>(object);
      serializer.AppendPortal(*portal, ++portalsCount);
      break;
    } default:
      break;
    }
  }

  serializer.WriteXML();
}

void TestLevelController::AmendLevel() {
  m_testLevelWidget->SetSaveButtonEnabled(false);
  m_polygonsList.clear();
  m_initialPolygonsList.clear();
  m_objectsList.clear();
  m_slicer = Slicer();
  m_graphicsPolygonItemsList.clear();
  m_polygonsColor = QColor();
  m_colorPicked = false;
  m_linesGoal = -1;
  m_linesDrawn = 0;
  m_partsGoal = -1;
  m_partsSplit = -1;
  m_maxGapToWin = -1;
  m_tolerance = -1;

  m_testLevelWidget->ClearGraphicsView();
}

void TestLevelController::RetryLevel() {
  m_testLevelWidget->SetSaveButtonEnabled(false);
  m_linesDrawn = 0;
  m_polygonsList = m_initialPolygonsList;
  m_partsSplit = m_polygonsList.size();
  m_slicer.SetPolygonsList(m_polygonsList);
  m_testLevelWidget->ClearGraphicsView();
  m_graphicsPolygonItemsList.clear();
  PlayLevel();
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

  if (m_slicer.SliceIt(ppxl::Point(p_event->pos().x(), p_event->pos().y()))) {
    ResetPolygonsList(m_slicer.GetPolygonsList());
    SetPolygonItems();

    m_partsSplit = m_polygonsList.size();
    ++m_linesDrawn;

    auto gap = CheckWin();

    qDebug() << QString("LINES: %1 / %2").arg(m_linesDrawn).arg(m_linesGoal);
    qDebug() << QString("PARTS: %1 / %2").arg(m_partsSplit).arg(m_partsGoal) << "\n";
    m_testLevelWidget->SetSaveButtonEnabled(m_tolerance > gap);
  }
  m_testLevelWidget->SetCuttingLines({});
  m_testLevelWidget->CuttingEnded();

}
