#ifndef TESTLEVELCOMMANDS_HXX
#define TESTLEVELCOMMANDS_HXX

#include <QUndoCommand>

#include "Core/Geometry/Segment.hxx"

class PolygonModel;

class SliceCommand: public QUndoCommand {
public:
  SliceCommand();
  //SliceCommand(PolygonModel* p_polygonModel, /*LineModel* p_lineModel,*/ ppxl::Segment const& p_line);

private:
  PolygonModel* m_polygonModel;
  //LineModel* m_lineModel;
  ppxl::Segment m_segment;
};

#endif
