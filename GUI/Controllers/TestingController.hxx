#ifndef TESTINGCONTROLLER_HXX
#define TESTINGCONTROLLER_HXX

#include "GUI/Controllers/PlayingController.hxx"

class TestingView;

class TestingController: public PlayingController {
  Q_OBJECT

public:
  TestingController(TestingView* p_view, QObject* p_parent);

  void SetPolygonsItem(PolygonModel* p_model);

  void SetLinesGoal(int p_linesGoal);
  void SetPartsGoal(int p_partsGoal);
  void SetMaxGapToWin(int p_maxGapToWin);
  void SetTolerance(int p_tolerance);

  void PlayLevel();

protected:
  void CheckWinning() override;
  void Redraw();
  void DisplayAreas(QPoint const& p_endPoint);
  void UpdateViewFromGameInfo() override;
  void ResetPolygonList();
  void Restart();
  void SaveLevel();

private:
  TestingView* m_view;
  QList<ppxl::Polygon> m_polygonsList;
};

#endif
