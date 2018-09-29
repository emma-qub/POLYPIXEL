#ifndef PARSER_H
#define PARSER_H

#include <QtXml/QDomElement>

#include "Core/Point.hxx"
#include "Core/Segment.hxx"
#include "Core/Polygon.hxx"
#include "Objects/Tape.hxx"
#include "Objects/Mirror.hxx"
#include "Objects/Portal.hxx"

using PolygonList = QList<ppxl::Polygon>;
using TapeList = QList<Tape>;
using MirrorList = QList<Mirror>;
using PortalList = QList<Portal>;

class Parser {
public:
  Parser();
  Parser(QString const& p_xmlFileName);
  virtual ~Parser();

  // Primitives
  int GetInt(QDomElement const& p_element, QString const& p_attributeName) const;
  double GetDouble(QDomElement const& p_element, QString const& p_attributeName) const;
  int GetIntValue(QString const& p_tagName, QString const& p_attributeName = "value") const;
  QDomElement GetElementById(QDomElement const& parent, QString const& p_name, int p_id) const;
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
  PolygonList GetPolygonList() const;
  inline int GetPolygonNodesCount() const { return m_polygonNodesCount; }

  // Tape
  Tape GetTape(QDomElement const& element) const;
  QDomElement GetTapeById(int p_id) const;
  TapeList GetTapeList();

  // Mirror
  Mirror GetMirror(QDomElement const& element) const;
  QDomElement GetMirrorById(int p_id) const;
  MirrorList GetMirrorList() const;

  // Portal
  Portal GetPortal(QDomElement const& element) const;
  QDomElement GetPortalById(int p_id) const;
  PortalList GetPortalList() const;

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
};

#endif
