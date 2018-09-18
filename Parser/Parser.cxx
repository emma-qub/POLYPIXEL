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

  m_polygonNodesCount = m_polygons.elementsByTagName("polygon").count();
  m_tapeNodesCount = m_tapes.elementsByTagName("tape").count();
  m_mirrorNodesCount = m_mirrors.elementsByTagName("mirror").count();
  m_portalNodesCount = m_portals.elementsByTagName("portal").count();

  XMLDoc.close();
}

Parser::~Parser() = default;


// Primitives

int Parser::GetInt(QDomElement const& p_element, QString const& attributeName) const {
  return p_element.attribute(attributeName).toInt();
}

double Parser::GetDouble(QDomElement const& p_element, QString const& p_attributeName) const {
  return p_element.attribute(p_attributeName).toDouble();
}

int Parser::GetIntValue(QString const& p_tagName, QString const& p_attributeName) const {
  QDomNodeList nodeList = m_doc.elementsByTagName(p_tagName);
  if (nodeList.size() != 1)
    return -1;

  QDomElement const& element = nodeList.at(0).toElement();
  return GetInt(element, p_attributeName);
}

QDomElement Parser::GetElementById(QDomElement const& parent, QString const& p_name, int p_id) const  {
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


// Game Infos

int Parser::GetPartsGoal() const {
  return GetIntValue("partsgoal");
}

int Parser::GetLinesGoal() const {
  return GetIntValue("linesgoal");
}

int Parser::GetMaxGapToWin() const {
  return GetIntValue("maxgaptowin");
}

int Parser::GetStarsCount() const {
  return GetIntValue("starscount");
}

int Parser::GetTolerances() const {
  return GetIntValue("tolerances");
}


// Polygon

ppxl::Point Parser::GetPoint(QDomElement const& p_element) const {
  return ppxl::Point(GetDouble(p_element, "x"), GetDouble(p_element, "y"));
}

ppxl::Segment Parser::GetSegment(QDomElement const& p_element) const {
  QDomNodeList bounds = p_element.elementsByTagName("bound");
  if (bounds.size() != 2) {
    qDebug() << "Cannot create segment whithin Parser::createSegment: file corrupted";
    return ppxl::Segment();
  }
  return ppxl::Segment(GetPoint(bounds.at(0).toElement()), GetPoint(bounds.at(1).toElement()));
}

ppxl::Polygon Parser::GetPolygon(QDomElement const& p_element) const {
  ppxl::Polygon polygon;

  QDomNodeList vertices = p_element.elementsByTagName("vertex");
  if (vertices.size() < 3) {
    qDebug() << "Cannot create polygon whithin Parser::createPolygon: file corrupted";
    return polygon;
  }
  for (int k = 0; k < vertices.size(); k++) {
    polygon << GetPoint(vertices.at(k).toElement());
  }

  return polygon;
}

QDomElement Parser::GetPolygonById(int p_id) const {
  return GetElementById(m_polygons, "polygon", p_id);
}

PolygonList Parser::GetPolygonList() const {
  PolygonList polygonList;
  for (int k = 0; k < m_polygonNodesCount; ++k)
    polygonList << GetPolygon(GetPolygonById(k));

  return polygonList;
}


// Tape

Tape Parser::GetTape(QDomElement const& element) const {
  return Tape(
    element.attribute("x", "-1").toInt(),
    element.attribute("y", "-1").toInt(),
    element.attribute("w", "-1").toInt(),
    element.attribute("h", "-1").toInt()
  );
}

QDomElement Parser::GetTapeById(int p_id) const {
  return GetElementById(m_tapes, "tape", p_id);
}

TapeList Parser::GetTapeList() {
  TapeList tapeList;
  for (int k = 0; k < m_tapeNodesCount; ++k)
    tapeList << GetTape(GetTapeById(k));

  return tapeList;
}


// Mirror

Mirror Parser::GetMirror(QDomElement const& element) const {
  return Mirror(
    element.attribute("xa", "-1").toInt(),
    element.attribute("ya", "-1").toInt(),
    element.attribute("xb", "-1").toInt(),
    element.attribute("yb", "-1").toInt()
  );
}

QDomElement Parser::GetMirrorById(int p_id) const {
  return GetElementById(m_mirrors, "mirror", p_id);
}

MirrorList Parser::GetMirrorList() const {
  MirrorList mirrorList;
  for (int k = 0; k < m_mirrorNodesCount; ++k)
    mirrorList << GetMirror(GetMirrorById(k));

  return mirrorList;
}


// Portal

Portal Parser::GetPortal(QDomElement const& element) const {
  return Portal(
      element.attribute("xaIn", "-1").toInt(),
      element.attribute("yaIn", "-1").toInt(),
      element.attribute("xbIn", "-1").toInt(),
      element.attribute("ybIn", "-1").toInt(),
      element.attribute("xaOut", "-1").toInt(),
      element.attribute("yaOut", "-1").toInt(),
      element.attribute("xbOut", "-1").toInt(),
      element.attribute("ybOut", "-1").toInt()
  );
}

QDomElement Parser::GetPortalById(int p_id) const {
  return GetElementById(m_portals, "portal", p_id);
}

PortalList Parser::GetPortalList() const {
  PortalList portalList;
  for (int k = 0; k < m_portalNodesCount; ++k)
    portalList << GetPortal(GetPortalById(k));

  return portalList;
}
