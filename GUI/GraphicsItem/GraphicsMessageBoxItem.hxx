#ifndef MESSAGEITEM_HXX
#define MESSAGEITEM_HXX

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>


class GraphicsMessageBoxTextItem: public QObject, public QGraphicsSimpleTextItem {
  Q_OBJECT
  Q_PROPERTY(int messageLength READ messageLength WRITE setMessageLength)

public:
  GraphicsMessageBoxTextItem(int p_width, int p_height, QFont const& p_font, QString const& p_message, QGraphicsItem* p_parent = nullptr);
  ~GraphicsMessageBoxTextItem() override;

  int messageLength() const;
  void setMessageLength(int p_messageLength);

  int GetMessageSize() const;
  bool TextDisplayDone() const;
  bool IsLastPage() const;
  void NextPage();
  void ClearText();

private:
  int m_messageLength;
  int m_width;
  int m_height;
  int m_currentPage;
  int m_pagesCount;
  QStringList m_messageList;
};

class GraphicsSquishRectItem: public QObject, public QGraphicsRectItem {
  Q_OBJECT
  Q_PROPERTY(int height READ height WRITE setHeight)

public:
  GraphicsSquishRectItem(int p_width, int p_height, QGraphicsItem* p_parent = nullptr);
  ~GraphicsSquishRectItem() override;

  int height() const {return m_height;}
  void setHeight(int p_height);

  int GetWidth() const { return m_width; }
  int GetHeight() const { return m_height; }

protected:
  int m_width;
  int m_height;
};


class GraphicsMessageBoxItem: public GraphicsSquishRectItem {
  Q_OBJECT

public:
  GraphicsMessageBoxItem(int p_width, int p_height, QString const& p_message, QGraphicsItem* p_parent = nullptr);
  ~GraphicsMessageBoxItem() override;

  void Animate(int p_startHeight, int p_endHeight, int p_startAvatarHeight, int p_endAvatarHeight);
  void Open();
  void Close();
  void DisplayText();

Q_SIGNALS:
  void Done();

protected:
  void UpdateTextStatus();
  void keyPressEvent(QKeyEvent* p_event) override;

private:
  GraphicsSquishRectItem* m_avatarItem;
  GraphicsMessageBoxTextItem* m_messageItem;
  QGraphicsRectItem* m_messageDoneIndicator;
  QGraphicsPolygonItem* m_messageNotDoneIndicator;
};

#endif
