#ifndef PARSER_H
#define PARSER_H

#include <QtXml/QDomElement>

#include "Core/Point.hxx"
#include "Core/Segment.hxx"
#include "Core/Polygon.hxx"
//#include "Tape.h"
//#include "Mirror.h"
//#include "Portal.h"
//#include "Refractor.h"
//#include "Hint.h"

using PolygonList = QList<ppxl::Polygon>;
//using TapeList = QList<Tape>;
//using MirrorList = QList<Mirror>;
//using PortalList = QList<Portal>;
//using RefractorList = QList<Refractor>;
//using HintList = QList<Hint>;

class Parser {
public:
  Parser();
  Parser(QString const& p_xmlFileName);
  virtual ~Parser();

  inline int GetPolygonNodesCount() const { return m_polygonNodesCount; }

  int CreateInt(QDomElement const& p_element, QString const& p_attributeName);
  double CreateDouble(QDomElement const& p_element, QString const& p_attributeName);
  ppxl::Point CreatePoint(QDomElement const& p_element);
  ppxl::Segment CreateSegment(QDomElement const& p_element);

  int GetIntValue(QString const& p_tagName, QString const& p_attributeName = "value");
  int GetPartsGoal();
  int GetLinesGoal();
  int GetMaxGapToWin();
  int GetStarsCount();
  int GetTolerances();

  ppxl::Polygon CreatePolygon(QDomElement const& p_element);
//  Tape createTape(QDomElement const& element);
//  Mirror createMirror(QDomElement const& element);
//  Portal createPortal(QDomElement const& element);
//  Refractor createRefractor(QDomElement const& element);
//  Hint createHint(QDomElement const& element);

  QDomElement GetElementById(QDomElement& parent, QString const& p_name, int p_id);
  QDomElement GetPolygon(int p_id);
  QDomElement GetTape(int p_id);
  QDomElement GetMirror(int p_id);
  QDomElement GetPortal(int p_id);
  QDomElement GetRefractor(int p_id);
  QDomElement GetHint(int p_id);

  PolygonList CreatePolygonList();
//  TapeList createTapeList();
//  MirrorList createMirrorList();
//  PortalList createPortalList();
//  RefractorList createRefractorList();
//  HintList createHintList();

  inline QDomDocument GetDoc() const { return m_doc; }
  inline void InitFileName(QString const& fileName) { if (m_xmlFileName.isEmpty()) m_xmlFileName = fileName; }

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
