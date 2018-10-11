#ifndef PARSER_H
#define PARSER_H

#include <QtXml/QDomElement>

#include "Core/Point.hxx"
#include "Core/Segment.hxx"
#include "Core/Polygon.hxx"
#include "Objects/Obstacles/Tape.hxx"
#include "Objects/Obstacles/OneWay.hxx"
#include "Objects/Deviations/Mirror.hxx"
#include "Objects/Deviations/Portal.hxx"

using PolygonsList = QList<ppxl::Polygon>;
using TapesList = QList<Tape>;
using OneWaysList = QList<OneWay>;
using MirrorsList = QList<Mirror>;
using PortalsList = QList<Portal>;

class Parser {
public:
  Parser();
  Parser(QString const& p_xmlFileName);
  virtual ~Parser();

  // Primitives
  int GetInt(QDomElement const& p_element, QString const& p_attributeName) const;
  double GetDouble(QDomElement const& p_element, QString const& p_attributeName) const;
  int GetIntValue(QString const& p_tagName, QString const& p_attributeName = "value") const;
  QDomElement GetElementById(QDomElement const& p_parent, QString const& p_name, int p_id) const;
  inline QDomDocument GetDoc() const { return m_doc; }

  // Game Infos
  int GetPartsGoal() const;
  int GetLinesGoal() const;
  int GetMaxGapToWin() const;
  int GetTolerance() const;
  int GetStarsCount() const;

  // Polygon
  ppxl::Point GetPoint(QDomElement const& p_element) const;
  ppxl::Segment GetSegment(QDomElement const& p_element) const;
  ppxl::Polygon GetPolygon(QDomElement const& p_element) const;
  QDomElement GetPolygonById(int p_id) const;
  PolygonsList GetPolygonsList() const;
  inline int GetPolygonNodesCount() const { return m_polygonNodesCount; }

  // Tape
  Tape GetTape(QDomElement const& p_element) const;
  QDomElement GetTapeById(int p_id) const;
  TapesList GetTapesList();

  // OneWay
  OneWay GetOneWay(QDomElement const& p_element) const;
  QDomElement GetOneWayById(int p_id) const;
  OneWaysList GetOneWaysList() const;

  // Mirror
  Mirror GetMirror(QDomElement const& p_element) const;
  QDomElement GetMirrorById(int p_id) const;
  MirrorsList GetMirrorsList() const;

  // Portal
  Portal GetPortal(QDomElement const& p_element) const;
  QDomElement GetPortalById(int p_id) const;
  PortalsList GetPortalsList() const;

private:
  QString m_xmlFileName;
  QDomDocument m_doc;

  QDomElement m_polygons;
  QDomElement m_tapes;
  QDomElement m_oneWays;
  QDomElement m_mirrors;
  QDomElement m_portals;

  int m_polygonNodesCount;
  int m_tapeNodesCount;
  int m_oneWayNodesCount;
  int m_mirrorNodesCount;
  int m_portalNodesCount;
};

#endif
