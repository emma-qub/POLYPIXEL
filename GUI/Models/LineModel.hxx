#ifndef LINEMODEL_HXX
#define LINEMODEL_HXX

#include <QStandardItemModel>

#include "Core/Geometry/Segment.hxx"

class LineModel: public QStandardItemModel {
  Q_OBJECT

public:
  LineModel(QObject* p_parent = nullptr);

  void PushBackLine(ppxl::Segment const& p_line);
  void PopLine();
  void SetX(int p_lineRow, int p_x);
  void SetY(int p_lineRow, int p_y);
};

#endif
