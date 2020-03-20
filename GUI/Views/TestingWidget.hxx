#ifndef TESTINGWIDGET_HXX
#define TESTINGWIDGET_HXX

#include "GUI/Views/PlayingWidget.hxx"

class QPushButton;

class TestingWidget: public PlayingWidget {
  Q_OBJECT

public:
  TestingWidget(QWidget* p_parent = nullptr);

  void DrawText(ppxl::Point p_position, const QString& p_text, int p_weight);

  void UpdateStarsCount(int p_starsCount);
  void UpdatePerfect(bool p_perfect);

  void SetSaveButtonEnable(bool p_enable);

signals:
  void RestartRequested();
  void SaveLevelRequested();

private:
  QLabel* m_starsCountLabel;
  QLabel* m_perfectLabel;
  QPushButton* m_restartButton;
  QPushButton* m_saveButton;
};

#endif
