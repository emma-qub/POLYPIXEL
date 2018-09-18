#ifndef SERIALIZER_HXX
#define SERIALIZER_HXX

#include <QtXml/QDomElement>

#include "Core/Point.hxx"
#include "Core/Segment.hxx"
#include "Core/Polygon.hxx"

class Serializer
{
public:
//  Serializer();
  Serializer(QString const& p_xmlFileName);

  virtual ~Serializer();

  QDomElement IntToNode(int p_value, QString const& p_nodeName);
  QDomElement PointToNode(ppxl::Point const& p_point, QString const& p_nodeName);
  QDomElement SegmentToNode(ppxl::Segment const& p_segment);
  QDomElement PartsGoalToNode(int p_partsGoal = 1);
  QDomElement LinesGoalToNode(int p_linesGoal = 0);
  QDomElement StarscountToNode(int p_starscount = 0);
  QDomElement TolerancesToNode(int p_tolerances = 1);
  QDomElement PolygonToNode(ppxl::Polygon const& p_polygon, int p_id);
//  QDomElement tapeToNode(Tape const& tape, int id);
//  QDomElement mirrorToNode(Mirror const& mirror, int id);
//  QDomElement portalToNode(Portal const& portal, int id);
//  QDomElement refractorToNode(Refractor const& refractor, int id);
//  QDomElement hintToNode(Hint const& hint, int id);

  void AddPolygon(ppxl::Polygon const& p_polygon);
//  void addTape(Tape const& tape);
//  void addMirror(Mirror const& mirror);
//  void addPortal(Portal const& portal);
//  void addRefractor(Refractor const& refractor);
//  void addHint(Hint const& hint);

  void SetStarsCount(int p_starscount);

  void WriteXML(int p_indent = 4);

private:
  QString m_xmlFileName;
  QDomDocument m_doc;

  QDomElement m_polygons;
  QDomElement m_lineModifiers;
  QDomElement m_tapes;
  QDomElement m_mirrors;
  QDomElement m_portals;
  QDomElement m_refractors;
  QDomElement m_hints;

  int m_polygonNodesCount;
  int m_tapeNodesCount;
  int m_mirrorNodesCount;
  int m_portalNodesCount;
  int m_refractorNodesCount;
  int m_hintNodesCount;
};

#endif
