#include "GraphicsMessageBoxItem.hxx"

#include <QBrush>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QKeyEvent>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsMessageBoxTextItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsMessageBoxTextItem::GraphicsMessageBoxTextItem(int p_width, int p_height, QFont const& p_font, QString const& p_message, QGraphicsItem* p_parent):
  QObject(),
  QGraphicsSimpleTextItem(p_parent),
  m_messageLength(0),
  m_width(p_width),
  m_height(p_height),
  m_currentPage(0) {

  setFont(p_font);

  // Chop message
  auto words = p_message.split(" ");
  QFontMetrics fm(p_font);

  int count = 0;
  while (!words.isEmpty()) {
    QString line;
    while (!words.isEmpty() && fm.boundingRect(line+words.first()).width() < m_width) {
      line+=" "+words.takeFirst();
    }
    if (count%2 == 0) {
      m_messageList << line;
    } else {
      m_messageList << m_messageList.takeLast()+"\n\n"+line;
    }
    ++count;
  }

  m_pagesCount = m_messageList.size();
}

GraphicsMessageBoxTextItem::~GraphicsMessageBoxTextItem() = default;

int GraphicsMessageBoxTextItem::messageLength() const {
  return m_messageLength;
}

void GraphicsMessageBoxTextItem::setMessageLength(int p_messageLength) {
  m_messageLength = p_messageLength;
  setText(m_messageList.at(m_currentPage).mid(0, p_messageLength));
}

int GraphicsMessageBoxTextItem::GetMessageSize() const {
  return m_messageList.at(m_currentPage).size();
}

bool GraphicsMessageBoxTextItem::TextDisplayDone() const {
  return m_currentPage == m_pagesCount;
}

bool GraphicsMessageBoxTextItem::IsLastPage() const {
  return m_currentPage == m_pagesCount-1;
}

void GraphicsMessageBoxTextItem::ClearText() {
  setText("");
}

void GraphicsMessageBoxTextItem::NextPage() {
  ++m_currentPage;
  ClearText();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsSquishRectItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsSquishRectItem::GraphicsSquishRectItem(int p_width, int p_height, QGraphicsItem* p_parent):
  QGraphicsRectItem(0, 0, p_width, p_height, p_parent),
  m_width(p_width),
  m_height(p_height) {
}

GraphicsSquishRectItem::~GraphicsSquishRectItem() = default;

void GraphicsSquishRectItem::setHeight(int p_height) {
  setRect(0, -static_cast<double>(p_height)/2., m_width, p_height);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GraphicsMessageBoxItem
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GraphicsMessageBoxItem::GraphicsMessageBoxItem(int p_width, int p_height, QString const& p_message, QGraphicsItem* p_parent):
  GraphicsSquishRectItem(p_width, p_height, p_parent),
  m_avatarItem(new GraphicsSquishRectItem(150, 150, this)) {

  setBrush(QBrush(QColor("#ffd319")));

  m_avatarItem->setBrush(QBrush(QColor("#ff2975")));
  m_avatarItem->setPos(m_width-150-25, 0);

  auto width = m_width - 25 - (m_avatarItem->GetWidth() + 25 + 25);
  auto height = m_height - 100;
  QString family = QFontDatabase::applicationFontFamilies(0).at(0);
  QFont textFont(family);
  textFont.setPixelSize(36);
  m_messageItem = new GraphicsMessageBoxTextItem(width, height, textFont, p_message, this);
  m_messageItem->setPos(50, -50);

  m_messageDoneIndicator = new QGraphicsRectItem(width-12, m_height/2 -20, 24, 24, this);
  m_messageDoneIndicator->setBrush(QBrush(QColor("#8c1eff")));
  m_messageDoneIndicator->setVisible(false);

  auto triangle = QPolygonF(QVector<QPointF>()
    << QPointF(width-12, m_height/2 -10)
    << QPointF(width+12, m_height/2 -10)
    << QPointF(width, m_height/2+6));
  m_messageNotDoneIndicator = new QGraphicsPolygonItem(triangle, this);
  m_messageNotDoneIndicator->setBrush(QBrush(QColor("#8c1eff")));
  m_messageNotDoneIndicator->setVisible(false);

  setFlag(QGraphicsItem::ItemIsFocusable);
}

GraphicsMessageBoxItem::~GraphicsMessageBoxItem() = default;

void GraphicsMessageBoxItem::Animate(int p_startHeight, int p_endHeight, int p_startAvatarHeight, int p_endAvatarHeight) {
  int duration = 200;

  auto dialogAnimation = new QPropertyAnimation(this, "height");
  dialogAnimation->setDuration(duration);
  dialogAnimation->setStartValue(p_startHeight);
  dialogAnimation->setEndValue(p_endHeight);
  dialogAnimation->setEasingCurve(QEasingCurve::InOutQuint);
  connect(dialogAnimation, &QPropertyAnimation::finished, dialogAnimation, &QPropertyAnimation::deleteLater);

  auto avatarAnimation = new QPropertyAnimation(m_avatarItem, "height");
  avatarAnimation->setDuration(duration);
  avatarAnimation->setStartValue(p_startAvatarHeight);
  avatarAnimation->setEndValue(p_endAvatarHeight);
  avatarAnimation->setEasingCurve(QEasingCurve::InOutQuint);
  connect(avatarAnimation, &QPropertyAnimation::finished, avatarAnimation, &QPropertyAnimation::deleteLater);

  auto animation = new QParallelAnimationGroup(this);
  animation->addAnimation(dialogAnimation);
  animation->addAnimation(avatarAnimation);
  connect(animation, &QParallelAnimationGroup::finished, animation, &QParallelAnimationGroup::deleteLater);
  if (m_messageItem->TextDisplayDone()) {
    connect(animation, &QParallelAnimationGroup::finished, this, &GraphicsMessageBoxItem::Done);
  } else {
    connect(animation, &QParallelAnimationGroup::finished, this, &GraphicsMessageBoxItem::DisplayText);
  }

  animation->start();
}

void GraphicsMessageBoxItem::Open() {
  Animate(0, m_height, 0, m_avatarItem->height());
}

void GraphicsMessageBoxItem::Close() {
  m_messageItem->ClearText();
  Animate(m_height, 0, m_avatarItem->height(), 0);
}

void GraphicsMessageBoxItem::DisplayText() {
  int durationPerChar = 50;
  int messageSize = m_messageItem->GetMessageSize();

  auto textAnimation = new QPropertyAnimation(m_messageItem, "messageLength");
  textAnimation->setDuration(messageSize*durationPerChar);
  textAnimation->setStartValue(0);
  textAnimation->setEndValue(messageSize);
  connect(textAnimation, &QPropertyAnimation::finished, this, &::GraphicsMessageBoxItem::UpdateTextStatus);
  connect(textAnimation, &QPropertyAnimation::finished, textAnimation, &QPropertyAnimation::deleteLater);

  textAnimation->start();
}

void GraphicsMessageBoxItem::UpdateTextStatus() {
  if (m_messageItem->IsLastPage()) {
    m_messageDoneIndicator->setVisible(true);
  } else {
    m_messageNotDoneIndicator->setVisible(true);
  }
}

void GraphicsMessageBoxItem::keyPressEvent(QKeyEvent* p_event) {
  if (p_event->key() == Qt::Key_Return) {
    if (m_messageItem->IsLastPage()) {
      m_messageItem->NextPage();
      m_messageDoneIndicator->setVisible(false);
      Close();
    } else {
      m_messageItem->NextPage();
      m_messageNotDoneIndicator->setVisible(false);
      DisplayText();
    }
  }
}
