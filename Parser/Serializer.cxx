#include "Serializer.hxx"

#include <QFile>
#include <QDebug>

//Serializer::Serializer():
//  m_xmlFileName(),
//  m_doc("PPXLML") {

//  QDomElement root = m_doc.createElement("level");
//  m_doc.appendChild(root);

//  m_polygons = m_doc.createElement("polygons");
//  root.appendChild(m_polygons);

//  m_lineModifiers = m_doc.createElement("linemodifiers");
//  root.appendChild(m_lineModifiers);

//  m_tapes = m_doc.createElement("tapes");
//  m_lineModifiers.appendChild(m_tapes);

//  m_mirrors = m_doc.createElement("mirrors");
//  m_lineModifiers.appendChild(m_mirrors);

//  m_portals = m_doc.createElement("portals");
//  m_lineModifiers.appendChild(m_portals);

//  m_refractors = m_doc.createElement("refractors");
//  m_lineModifiers.appendChild(m_refractors);

//  root.appendChild(m_doc.createElement("linesgoal"));
//  root.appendChild(m_doc.createElement("partsgoal"));
//  root.appendChild(m_doc.createElement("maxgaptowin"));
//  root.appendChild(m_doc.createElement("starscount"));

//  m_hints = m_doc.createElement("hints");
//  root.appendChild(m_hints);
//}

Serializer::Serializer(QString const& p_xmlFileName):
  m_xmlFileName(p_xmlFileName),
  m_doc(QDomDocument("PPXLML")) {

  QFile XMLDoc(m_xmlFileName);
  if(!XMLDoc.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Cannot open XML file in Serializer::Serializer(QString xmlFileName)";
    return;
  }

  if(!m_doc.setContent(&XMLDoc)) {
    XMLDoc.close();
    qDebug() << "Cannot set content of dom in Serializer::Serializer(QString xmlFileName)";
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

Serializer::~Serializer() = default;

QDomElement Serializer::IntToNode(int p_value, QString const& p_nodeName) {
  QDomElement element(m_doc.createElement(p_nodeName));
  element.setAttribute("value", p_value);
  return element;
}

QDomElement Serializer::PointToNode(ppxl::Point const& p_point, QString const& p_nodeName) {
  QDomElement element(m_doc.createElement(p_nodeName));
  element.setAttribute("x", p_point.GetX());
  element.setAttribute("y", p_point.GetY());
  return element;
}

QDomElement Serializer::SegmentToNode(ppxl::Segment const& p_segment) {
  QDomElement element(m_doc.createElement("Line"));
  element.appendChild(PointToNode(p_segment.GetA(), "bound"));
  element.appendChild(PointToNode(p_segment.GetB(), "bound"));
  return element;
}

QDomElement Serializer::PartsGoalToNode(int p_partsGoal) {
  return IntToNode(p_partsGoal, "partsgoal");
}

QDomElement Serializer::LinesGoalToNode(int p_linesGoal) {
  return IntToNode(p_linesGoal, "linesgoal");
}

QDomElement Serializer::StarscountToNode(int p_starscount) {
  return IntToNode(p_starscount, "starscount");
}

QDomElement Serializer::TolerancesToNode(int p_tolerances) {
  return IntToNode(p_tolerances, "tolerances");
}

QDomElement Serializer::PolygonToNode(ppxl::Polygon const& p_polygon, int p_id) {
  QDomElement element(m_doc.createElement("polygon"));
  element.setAttribute("id", p_id);

  for (ppxl::Point const& vertex: p_polygon.GetVertices()) {
    element.appendChild(PointToNode(vertex, "vertex"));
  }

  return element;
}

//QDomElement Serializer::tapeToNode(Tape const& tape, int id) {
//  QDomElement element(m_doc.createElement("tape"));
//  element.setAttribute("id", id);

//  element.setAttribute("x", tape.getX());
//  element.setAttribute("y", tape.getY());
//  element.setAttribute("w", tape.getW());
//  element.setAttribute("h", tape.getH());

//  return element;
//}

//QDomElement Serializer::mirrorToNode(Mirror const& mirror, int id) {
//  QDomElement element(m_doc.createElement("mirror"));
//  element.setAttribute("id", id);

//  ppxl::Segment mirrorLine(mirror.getMirrorLine());
//  element.setAttribute("xa", mirrorLine.getA().getX());
//  element.setAttribute("ya", mirrorLine.getA().getY());
//  element.setAttribute("xb", mirrorLine.getB().getX());
//  element.setAttribute("yb", mirrorLine.getB().getY());

//  return element;
//}

//QDomElement Serializer::portalToNode(Portal const& portal, int id) {
//  QDomElement element(m_doc.createElement("portal"));
//  element.setAttribute("id", id);

//  ppxl::Segment portalLineIn(portal.getIn());
//  element.setAttribute("xaIn", portalLineIn.getA().getX());
//  element.setAttribute("yaIn", portalLineIn.getA().getY());
//  element.setAttribute("xbIn", portalLineIn.getB().getX());
//  element.setAttribute("ybIn", portalLineIn.getB().getY());

//  ppxl::Segment portalLineOut(portal.getOut());
//  element.setAttribute("xaOut", portalLineOut.getA().getX());
//  element.setAttribute("yaOut", portalLineOut.getA().getY());
//  element.setAttribute("xbOut", portalLineOut.getB().getX());
//  element.setAttribute("ybOut", portalLineOut.getB().getY());

//  return element;
//}

//QDomElement Serializer::refractorToNode(Refractor const& refractor, int id) {
//  QDomElement element(m_doc.createElement("refractor"));
//  element.setAttribute("id", id);

//  ppxl::Segment refractorLineIn(refractor.getRefractorLine());
//  element.setAttribute("xa", refractorLineIn.getA().getX());
//  element.setAttribute("ya", refractorLineIn.getA().getY());
//  element.setAttribute("xb", refractorLineIn.getB().getX());
//  element.setAttribute("yb", refractorLineIn.getB().getY());

//  Double indice(refractor.getIndice());
//  element.setAttribute("indice", indice);

//  return element;
//}

//QDomElement Serializer::hintToNode(Hint const& hint, int id) {
//  QDomElement element(m_doc.createElement("hint"));
//  element.setAttribute("id", id);

//  element.appendChild(segmentToNode(hint.getLine()));
//  element.appendChild(intToNode(hint.getCost(), "cost"));

//  return element;
//}

void Serializer::AddPolygon(ppxl::Polygon const& p_polygon) {
  m_polygons.appendChild(PolygonToNode(p_polygon, m_polygonNodesCount++));
}

//void Serializer::addTape(Tape const& tape) {
//  m_tapes.appendChild(tapeToNode(tape, m_tapeNodesCount++));
//}

//void Serializer::addMirror(Mirror const& mirror) {
//  m_mirrors.appendChild(mirrorToNode(mirror, m_mirrorNodesCount++));
//}

//void Serializer::addPortal(Portal const& portal) {
//  m_portals.appendChild(portalToNode(portal, m_portalNodesCount++));
//}

//void Serializer::addRefractor(Refractor const& refractor) {
//  m_refractors.appendChild(refractorToNode(refractor, m_refractorNodesCount++));
//}

//void Serializer::addHint(Hint const& hint) {
//  m_hints.appendChild(hintToNode(hint, m_hintNodesCount++));
//}

void Serializer::SetStarsCount(int p_starscount) {
  m_doc.elementsByTagName("starscount").at(0).toElement().setAttribute("value", p_starscount);
}

void Serializer::WriteXML(int p_indent) {
  QFile XMLDoc(m_xmlFileName);
  if(!XMLDoc.open(QIODevice::WriteOnly)) {
    qDebug() << "Cannot open XML file in Serializer::writeXML";
    return;
  }

  QTextStream inFile(&XMLDoc);
  inFile << m_doc.toString(p_indent);

  XMLDoc.close();
}
