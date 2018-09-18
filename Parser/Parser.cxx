#include "Parser.hxx"

#include <QFile>
#include <QDebug>

Parser::Parser(QString const& p_xmlFileName):
  m_xmlFileName(p_xmlFileName),
  m_doc(QDomDocument("PPXLML")) {

  QFile XMLDoc(m_xmlFileName);
  if(!XMLDoc.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Cannot open XML file in Parser::Parser(QString xmlFileName)";
    return;
  }

  if(!m_doc.setContent(&XMLDoc)) {
    XMLDoc.close();
    qDebug() << "Cannot set content of dom in Parser::Parser(QString xmlFileName)";
    return;
  }

  m_polygons = m_doc.firstChildElement("level").firstChildElement("polygons");
  m_tapes = m_doc.firstChildElement("level").firstChildElement("linemodifiers").firstChildElement("tapes");
  m_mirrors = m_doc.firstChildElement("level").firstChildElement("linemodifiers").firstChildElement("mirrors");
  m_portals = m_doc.firstChildElement("level").firstChildElement("linemodifiers").firstChildElement("portals");
  m_refractors = m_doc.firstChildElement("level").firstChildElement("linemodifiers").firstChildElement("refractors");
  m_hints = m_doc.firstChildElement("level").firstChildElement("hints");

  m_polygonNodesCount = m_polygons.elementsByTagName("polygon").count();
  m_tapeNodesCount = m_tapes.elementsByTagName("tape").count();
  m_mirrorNodesCount = m_mirrors.elementsByTagName("mirror").count();
  m_portalNodesCount = m_portals.elementsByTagName("portal").count();
  m_refractorNodesCount = m_refractors.elementsByTagName("refractor").count();
  m_hintNodesCount = m_hints.elementsByTagName("hint").count();

  XMLDoc.close();
}

Parser::~Parser() = default;

int Parser::CreateInt(QDomElement const& p_element, QString const& attributeName) {
  return p_element.attribute(attributeName).toInt();
}

double Parser::CreateDouble(QDomElement const& p_element, QString const& p_attributeName) {
  return p_element.attribute(p_attributeName).toDouble();
}

ppxl::Point Parser::CreatePoint(QDomElement const& p_element) {
  return ppxl::Point(CreateDouble(p_element, "x"), CreateDouble(p_element, "y"));
}

ppxl::Segment Parser::CreateSegment(QDomElement const& p_element) {
  QDomNodeList bounds = p_element.elementsByTagName("bound");
  if (bounds.size() != 2) {
    qDebug() << "Cannot create segment whithin Parser::createSegment: file corrupted";
    return ppxl::Segment();
  }
  return ppxl::Segment(CreatePoint(bounds.at(0).toElement()), CreatePoint(bounds.at(1).toElement()));
}

int Parser::GetIntValue(QString const& p_tagName, QString const& p_attributeName) {
  QDomNodeList nodeList = m_doc.elementsByTagName(p_tagName);
  if (nodeList.size() != 1)
    return -1;

  QDomElement const& element = nodeList.at(0).toElement();
  return CreateInt(element, p_attributeName);
}

int Parser::GetPartsGoal() {
  return GetIntValue("partsgoal");
}

int Parser::GetLinesGoal() {
  return GetIntValue("linesgoal");
}

int Parser::GetMaxGapToWin() {
  return GetIntValue("maxgaptowin");
}

int Parser::GetStarsCount() {
  return GetIntValue("starscount");
}

int Parser::GetTolerances() {
  return GetIntValue("tolerances");
}

ppxl::Polygon Parser::CreatePolygon(QDomElement const& p_element) {
  ppxl::Polygon polygon;

  QDomNodeList vertices = p_element.elementsByTagName("vertex");
  if (vertices.size() < 3) {
    qDebug() << "Cannot create polygon whithin Parser::createPolygon: file corrupted";
    return polygon;
  }
  for (int k = 0; k < vertices.size(); k++) {
    polygon << CreatePoint(vertices.at(k).toElement());
  }

  return polygon;
}

//Tape Parser::createTape(QDomElement const& element) {
//  return Tape(
//    element.attribute("x", "-1").toInt(),
//    element.attribute("y", "-1").toInt(),
//    element.attribute("w", "-1").toInt(),
//    element.attribute("h", "-1").toInt()
//  );
//}

//Mirror Parser::createMirror(QDomElement const& element) {
//  return Mirror(
//    element.attribute("xa", "-1").toInt(),
//    element.attribute("ya", "-1").toInt(),
//    element.attribute("xb", "-1").toInt(),
//    element.attribute("yb", "-1").toInt()
//  );
//}

//Portal Parser::createPortal(QDomElement const& element) {
//  return Portal(
//      element.attribute("xaIn", "-1").toInt(),
//      element.attribute("yaIn", "-1").toInt(),
//      element.attribute("xbIn", "-1").toInt(),
//      element.attribute("ybIn", "-1").toInt(),
//      element.attribute("xaOut", "-1").toInt(),
//      element.attribute("yaOut", "-1").toInt(),
//      element.attribute("xbOut", "-1").toInt(),
//      element.attribute("ybOut", "-1").toInt()
//  );
//}

//Refractor Parser::createRefractor(QDomElement const& element) {
//  return Refractor(
//      element.attribute("xa", "-1").toInt(),
//      element.attribute("ya", "-1").toInt(),
//      element.attribute("xb", "-1").toInt(),
//      element.attribute("yb", "-1").toInt(),
//      element.attribute("indice", "-1.").toDouble()
//  );
//}

//Hint Parser::createHint(QDomElement const& element) {
//  QDomNodeList lines = element.elementsByTagName("line");
//  QDomNodeList costs = element.elementsByTagName("cost");
//  if (lines.size() != 1 || costs.size() != 1) {
//    qDebug() << "Cannot create hint whithin Parser::createHint: file corrupted";
//    return Hint();
//  }

//  return Hint(createSegment(lines.at(0).toElement()), createInt(costs.at(0).toElement(), "value"));
//}

QDomElement Parser::GetElementById(QDomElement& parent, QString const& p_name, int p_id)  {
  QDomNodeList nodeList = parent.elementsByTagName(p_name);
  for (int k = 0; k < nodeList.size(); k++) {
    QDomElement element = nodeList.at(k).toElement();
    if (element.attribute("id").toInt() == p_id) {
      return element;
    }
  }

  qDebug() << "Error: cannot find element " << p_name << " with number" << QString::number(p_id);
  return QDomElement();
}

QDomElement Parser::GetPolygon(int p_id) {
  return GetElementById(m_polygons, "polygon", p_id);
}

QDomElement Parser::GetTape(int p_id) {
  return GetElementById(m_tapes, "tape", p_id);
}

QDomElement Parser::GetMirror(int p_id) {
  return GetElementById(m_mirrors, "mirror", p_id);
}

QDomElement Parser::GetPortal(int p_id) {
  return GetElementById(m_portals, "portal", p_id);
}

QDomElement Parser::GetRefractor(int p_id) {
  return GetElementById(m_refractors, "refractor", p_id);
}

QDomElement Parser::GetHint(int p_id) {
  return GetElementById(m_hints, "hint", p_id);
}

PolygonList Parser::CreatePolygonList() {
  PolygonList polygonList;
  for (int k = 0; k < m_polygonNodesCount; ++k)
    polygonList << CreatePolygon(GetPolygon(k));

  return polygonList;
}

//TapeList Parser::createTapeList() {
//  TapeList tapeList;
//  for (int k = 0; k < m_tapeNodesCount; ++k)
//    tapeList << createTape(getTape(k));

//  return tapeList;
//}

//MirrorList Parser::createMirrorList() {
//  MirrorList mirrorList;
//  for (int k = 0; k < m_mirrorNodesCount; ++k)
//    mirrorList << createMirror(getMirror(k));

//  return mirrorList;
//}

//PortalList Parser::createPortalList() {
//  PortalList portalList;
//  for (int k = 0; k < m_portalNodesCount; ++k)
//    portalList << createPortal(getPortal(k));

//  return portalList;
//}

//RefractorList Parser::createRefractorList() {
//  RefractorList refractorList;
//  for (int k = 0; k < m_refractorNodesCount; ++k)
//    refractorList << createRefractor(getRefractor(k));

//  return refractorList;
//}

//HintList Parser::createHintList() {
//  HintList hintList;
//  for (int k = 0; k < m_hintNodesCount; ++k)
//    hintList << createHint(getHint(k));

//  return hintList;
//}
