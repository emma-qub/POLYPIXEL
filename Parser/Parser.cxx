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
  m_tapes = m_doc.firstChildElement("level").firstChildElement("objects").firstChildElement("obstacles").firstChildElement("tapes");
  m_oneWays = m_doc.firstChildElement("level").firstChildElement("objects").firstChildElement("obstacles").firstChildElement("oneways");
  m_mirrors = m_doc.firstChildElement("level").firstChildElement("objects").firstChildElement("deviations").firstChildElement("mirrors");
  m_portals = m_doc.firstChildElement("level").firstChildElement("objects").firstChildElement("deviations").firstChildElement("portals");

  m_polygonNodesCount = m_polygons.elementsByTagName("polygon").count();
  m_tapeNodesCount = m_tapes.elementsByTagName("tape").count();
  m_oneWayNodesCount = m_oneWays.elementsByTagName("oneway").count();
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

QDomElement Parser::GetElementById(QDomElement const& p_parent, QString const& p_name, int p_id) const  {
  QDomNodeList nodeList = p_parent.elementsByTagName(p_name);
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

int Parser::GetTolerance() const {
  return GetIntValue("tolerance");
}

int Parser::GetStarsCount() const {
  return GetIntValue("starscount");
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

PolygonsList Parser::GetPolygonsList() const {
  PolygonsList polygonList;
  for (int k = 0; k < m_polygonNodesCount; ++k) {
    polygonList << GetPolygon(GetPolygonById(k));
  }

  return polygonList;
}


// Tape

Tape Parser::GetTape(QDomElement const& p_element) const {
  return Tape(
    p_element.attribute("x", "-1.").toDouble(),
    p_element.attribute("y", "-1.").toDouble(),
    p_element.attribute("w", "-1.").toDouble(),
    p_element.attribute("h", "-1.").toDouble()
  );
}

QDomElement Parser::GetTapeById(int p_id) const {
  return GetElementById(m_tapes, "tape", p_id);
}

TapesList Parser::GetTapesList() {
  TapesList tapeList;
  for (int k = 0; k < m_tapeNodesCount; ++k) {
    tapeList << GetTape(GetTapeById(k));
  }

  return tapeList;
}


// One Way

OneWay Parser::GetOneWay(QDomElement const& p_element) const {
  return OneWay(
    p_element.attribute("xa", "-1.").toDouble(),
    p_element.attribute("ya", "-1.").toDouble(),
    p_element.attribute("xb", "-1.").toDouble(),
    p_element.attribute("yb", "-1.").toDouble()
  );
}

QDomElement Parser::GetOneWayById(int p_id) const {
  return GetElementById(m_oneWays, "oneway", p_id);
}

OneWaysList Parser::GetOneWaysList() const {
  OneWaysList oneWayList;
  for (int k = 0; k < m_oneWayNodesCount; ++k) {
    oneWayList << GetOneWay(GetOneWayById(k));
  }

  return oneWayList;
}


// Mirror

Mirror Parser::GetMirror(QDomElement const& p_element) const {
  return Mirror(
    p_element.attribute("xa", "-1.").toDouble(),
    p_element.attribute("ya", "-1.").toDouble(),
    p_element.attribute("xb", "-1.").toDouble(),
    p_element.attribute("yb", "-1.").toDouble()
  );
}

QDomElement Parser::GetMirrorById(int p_id) const {
  return GetElementById(m_mirrors, "mirror", p_id);
}

MirrorsList Parser::GetMirrorsList() const {
  MirrorsList mirrorList;
  for (int k = 0; k < m_mirrorNodesCount; ++k) {
    mirrorList << GetMirror(GetMirrorById(k));
  }

  return mirrorList;
}


// Portal

Portal Parser::GetPortal(QDomElement const& p_element) const {
  return Portal(
    p_element.attribute("xaIn", "-1.").toDouble(),
    p_element.attribute("yaIn", "-1.").toDouble(),
    p_element.attribute("xbIn", "-1.").toDouble(),
    p_element.attribute("ybIn", "-1.").toDouble(),
    p_element.attribute("xaOut", "-1.").toDouble(),
    p_element.attribute("yaOut", "-1.").toDouble(),
    p_element.attribute("xbOut", "-1.").toDouble(),
    p_element.attribute("ybOut", "-1").toDouble()
  );
}

QDomElement Parser::GetPortalById(int p_id) const {
  return GetElementById(m_portals, "portal", p_id);
}

PortalsList Parser::GetPortalsList() const {
  PortalsList portalList;
  for (int k = 0; k < m_portalNodesCount; ++k) {
    portalList << GetPortal(GetPortalById(k));
  }

  return portalList;
}
