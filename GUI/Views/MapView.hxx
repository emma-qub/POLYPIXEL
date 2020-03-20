#ifndef MAPVIEW_HXX
#define MAPVIEW_HXX

#include <QGraphicsView>

class GameStartItem;

class MapView: public QGraphicsView {
  Q_OBJECT

public:
  explicit MapView(QWidget* p_parent = nullptr);
  ~MapView() override;

  void InitView();

Q_SIGNALS:
  void PlayLevelRequested(QString const& p_levelPath);
  void CancelLevelRequested();

protected:
  void OpenMessageBoxTest();
  void mousePressEvent(QMouseEvent* p_mouseEvent) override;
  void PlayLevel();
  void CleanGameStart();

  void DisplayGameStart();

private:
  QGraphicsScene* m_scene;
  GameStartItem* m_gameStartItem;
  bool m_viewInitialized;
  QList<QGraphicsRectItem*> m_levelsGraphicsList;
  QString m_currentLevelPath;
  int m_currentLevelNumber;
};

#endif
