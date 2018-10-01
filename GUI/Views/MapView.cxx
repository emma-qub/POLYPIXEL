#include "MapView.hxx"

#include "GUI/GraphicsItem/PlayerItem.hxx"
#include "GUI/GraphicsItem/GraphicsMessageBoxItem.hxx"

#include <QAction>



#include <QDebug>



MapView::MapView(QWidget* p_parent):
  QGraphicsView(p_parent) {

  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   m_scene = new QGraphicsScene(this);
   setScene(m_scene);
   m_scene->setSceneRect(0, 0, 1600, 800);

   m_scene->setBackgroundBrush(QBrush(QColor("#f222ff")));

   m_player = new Player;
   m_scene->addItem(m_player);
   m_player->setFocus();
   m_player->setPos(width()/2, height()/2);

   auto openMessageAction = new QAction(this);
   addAction(openMessageAction);
   openMessageAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
   connect(openMessageAction, &QAction::triggered, this, &MapView::OpenMessageBoxTest);
}



void MapView::OpenMessageBoxTest() {
  auto messageBoxItem = new GraphicsMessageBoxItem(width()-100, 200,
    "BIENVENUE DANS NOTRE ROYAUME, HUMAIN. VOTRE MISSION, SI VOUS L'ACCEPTEZ,"
    " CONSISTERA A DECOUPER DES POLYGONES SELON CERTAINES REGLES, QUE JE VOUS"
    " EXPLIQUERAI AU FUR ET A MESURE.");
  m_scene->addItem(messageBoxItem);
  messageBoxItem->setPos(50, 80);
  messageBoxItem->setFocus();
  messageBoxItem->Open();
  connect(messageBoxItem, &GraphicsMessageBoxItem::Done, this, [this, messageBoxItem](){
    m_scene->removeItem(messageBoxItem);
    delete messageBoxItem;
    m_player->setFocus();
  });
}
