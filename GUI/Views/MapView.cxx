#include "MapView.hxx"

#include "GUI/GraphicsItem/GameStartItem.hxx"
#include "Parser/Parser.hxx"

MapView::MapView(QWidget* p_parent):
  QGraphicsView(p_parent),
  m_scene(nullptr),
  m_gameStartItem(nullptr),
  m_viewInitialized(false) {
}

MapView::~MapView() = default;

void MapView::InitView() {
  if (m_viewInitialized) {
    return;
  }

  // Hide scrollbars
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Set scene
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);
  m_scene->setSceneRect(0, 0, width(), height());
  m_scene->setBackgroundBrush(QBrush(QColor("#ac204a")));

  auto rowCount = 4;
  auto colCount = 5;
  auto leftMargin = 200;
  auto topMargin = 100;
  auto rowMargin = 20;
  auto colMargin = 40;
  auto levelWidth = (width() - 2*leftMargin - (colCount-1)*colMargin)/colCount;
  auto levelHeight = (height() - 2*topMargin - (rowCount-1)*rowMargin)/rowCount;
  for (int row = 0; row < rowCount; ++row) {
    for (int col = 0; col < colCount; ++col) {
      auto left = leftMargin+col*levelWidth+col*colMargin;
      auto top = topMargin+row*levelHeight+row*rowMargin;
      auto levelRect = m_scene->addRect(left, top, levelWidth, levelHeight, QPen(Qt::NoPen), QBrush(Qt::white));
      m_levelsGraphicsList << levelRect;
      levelRect->setAcceptHoverEvents(true);
      auto levelNumberText = m_scene->addText(QString::number(col+colCount*row+1));
      levelNumberText->setPos(levelRect->boundingRect().center()-levelNumberText->boundingRect().center());
    }
  }

  m_viewInitialized = true;
}

void MapView::mousePressEvent(QMouseEvent* p_mouseEvent) {
  auto item = itemAt(p_mouseEvent->pos());
  for (int k = 0; k < m_levelsGraphicsList.size(); ++k) {
    if (item == m_levelsGraphicsList.at(k)) {
      m_currentLevelPath = QString(":/levels/W%1_L%2%3.ppxl").arg(1).arg((k+1) < 10 ? "0": "").arg(k+1);
      m_currentLevelNumber = k+1;
      DisplayGameStart();
      return;
    }
  }

  QGraphicsView::mousePressEvent(p_mouseEvent);
}

void MapView::PlayLevel() {
  CleanGameStart();
  Q_EMIT PlayLevelRequested(m_currentLevelPath);
}

void MapView::CleanGameStart() {
  scene()->removeItem(m_gameStartItem);
  m_gameStartItem = nullptr;
}

void MapView::DisplayGameStart() {
  // Create GameStart item
  m_gameStartItem = new GameStartItem(0, 0, width()/4, 2*height()/3);
  scene()->addItem(m_gameStartItem);
  m_gameStartItem->InitItems();

  QPointF startTopLeftCorner(3.*width()/8., -2.*height()/3.);
  QPointF endStartLeftCorner(startTopLeftCorner.x(), height()/6.);

  m_gameStartItem->setPos(startTopLeftCorner);

  Parser parser(m_currentLevelPath);
  m_gameStartItem->SetLevelInfo(m_currentLevelNumber,
    parser.GetLinesGoal(), parser.GetPartsGoal(), parser.GetStarsCount());
  m_gameStartItem->Open(startTopLeftCorner, endStartLeftCorner);
  m_gameStartItem->setFocus();

  connect(m_gameStartItem, &GameStartItem::StartLevelRequested, this, &MapView::PlayLevel);
  connect(m_gameStartItem, &GameStartItem::CancelLevelRequested, this, &MapView::CleanGameStart);
}
