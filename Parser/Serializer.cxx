#include "Serializer.hxx"

#include <QFile>
#include <QDebug>

Serializer::Serializer(QString const& p_xmlFileName):
  m_xmlFileName(p_xmlFileName),
  m_doc(QDomDocument("PPXLML")),
  m_polygons(m_doc.createElement("polygons")),
  m_tapes(m_doc.createElement("tapes")),
  m_oneWays(m_doc.createElement("oneways")),
  m_mirrors(m_doc.createElement("mirrors")),
  m_portals(m_doc.createElement("portals")),
  m_linesGoal(m_doc.createElement("linesgoal")),
  m_partsGoal(m_doc.createElement("partsgoal")),
  m_maxGapToWin(m_doc.createElement("maxgaptowin")),
  m_tolerance(m_doc.createElement("tolerance")),
  m_starsCount(m_doc.createElement("starscount")) {
}

Serializer::~Serializer() = default;


// Serialize

void Serializer::WriteXML(int p_indent) {
  QFile XMLDoc(m_xmlFileName);
  if(!XMLDoc.open(QIODevice::WriteOnly)) {
    qDebug() << "Cannot open XML file in Serializer::writeXML\n" << XMLDoc.errorString();
    return;
  }

  QDomElement root = m_doc.createElement("level");
  m_doc.appendChild(root);
  root.appendChild(m_polygons);

  auto objectsElement = m_doc.createElement("objects");
  root.appendChild(objectsElement);
  auto obstaclesElement = m_doc.createElement("obstacles");
  objectsElement.appendChild(obstaclesElement);
  objectsElement.appendChild(m_tapes);
  objectsElement.appendChild(m_oneWays);
  auto deviationsElement = m_doc.createElement("deviations");
  objectsElement.appendChild(deviationsElement);
  deviationsElement.appendChild(m_mirrors);
  deviationsElement.appendChild(m_portals);

  root.appendChild(m_linesGoal);
  root.appendChild(m_partsGoal);
  root.appendChild(m_maxGapToWin);
  root.appendChild(m_tolerance);
  root.appendChild(m_starsCount);

  QTextStream inFile(&XMLDoc);
  inFile << m_doc.toString(p_indent);

  XMLDoc.close();
}


// Game Infos

void Serializer::SetLinesGoal(int p_linesGoal) {
  m_linesGoal.setAttribute("value", p_linesGoal);
}

void Serializer::SetPartsGoal(int p_partsGoal) {
  m_partsGoal.setAttribute("value", p_partsGoal);
}

void Serializer::SetMaxGapToWin(int p_maxGapToWin) {
  m_maxGapToWin.setAttribute("value", p_maxGapToWin);
}

void Serializer::SetTolerance(int p_tolerance) {
  m_tolerance.setAttribute("value", p_tolerance);
}

void Serializer::SetStarsCount(int p_starscount) {
  m_starsCount.setAttribute("value", p_starscount);
}


// Polygon

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

void Serializer::AppendPolygon(ppxl::Polygon const& p_polygon, int p_id) {
  QDomElement element(m_doc.createElement("polygon"));
  element.setAttribute("id", p_id);

  for (auto const& vertex: p_polygon.GetVertices()) {
    element.appendChild(PointToNode(vertex, "vertex"));
  }

  m_polygons.appendChild(element);
}

void Serializer::SetPolygonsList(QList<ppxl::Polygon> const& p_polygons) {
  int id = 0;
  for (auto const& polygon: p_polygons) {
    AppendPolygon(polygon, id);
    ++id;
  }
}


// Tape

QDomElement Serializer::TapeToNode(Tape const& p_tape, int p_id) {
  QDomElement element(m_doc.createElement("tape"));
  element.setAttribute("id", p_id);

  element.setAttribute("x", p_tape.getX());
  element.setAttribute("y", p_tape.getY());
  element.setAttribute("w", p_tape.getW());
  element.setAttribute("h", p_tape.getH());

  return element;
}

void Serializer::AppendTape(Tape const& p_tape, int p_id) {
  m_tapes.appendChild(TapeToNode(p_tape, p_id));
}

void Serializer::SetTapeList(QList<Tape> const& p_tapes) {
  int id = 0;
  for (auto const& tape: p_tapes) {
    AppendTape(tape, id);
    ++id;
  }
}


// OneWay

QDomElement Serializer::OneWayToNode(OneWay const& p_oneWay, int p_id) {
  QDomElement element(m_doc.createElement("oneway"));
  element.setAttribute("id", p_id);

  ppxl::Segment line(p_oneWay.GetLine());
  element.setAttribute("xa", line.GetA().GetX());
  element.setAttribute("ya", line.GetA().GetY());
  element.setAttribute("xb", line.GetB().GetX());
  element.setAttribute("yb", line.GetB().GetY());

  ppxl::Vector direction(p_oneWay.GetDirection());
  element.setAttribute("xd", direction.GetX());
  element.setAttribute("yd", direction.GetY());

  return element;
}

void Serializer::SetOneWaysList(const QList<OneWay>& p_oneWays) {
  int id = 0;
  for (auto const& oneWay: p_oneWays) {
    AppendOneWay(oneWay, id);
    ++id;
  }
}

void Serializer::AppendOneWay(OneWay const& p_oneWay, int p_id) {
  m_oneWays.appendChild(OneWayToNode(p_oneWay, p_id));
}


// Mirror

QDomElement Serializer::MirrorToNode(Mirror const& p_mirror, int p_id) {
  QDomElement element(m_doc.createElement("mirror"));
  element.setAttribute("id", p_id);

  ppxl::Segment mirrorLine(p_mirror.GetMirrorLine());
  element.setAttribute("xa", mirrorLine.GetA().GetX());
  element.setAttribute("ya", mirrorLine.GetA().GetY());
  element.setAttribute("xb", mirrorLine.GetB().GetX());
  element.setAttribute("yb", mirrorLine.GetB().GetY());

  return element;
}

void Serializer::AppendMirror(Mirror const& p_mirror, int p_id) {
  m_mirrors.appendChild(MirrorToNode(p_mirror, p_id));
}

void Serializer::SetMirrorsList(QList<Mirror> const& p_mirrors) {
  int id = 0;
  for (auto const& mirror: p_mirrors) {
    AppendMirror(mirror, id);
    ++id;
  }
}


// Portal

QDomElement Serializer::PortalToNode(Portal const& p_portal, int p_id) {
  QDomElement element(m_doc.createElement("portal"));
  element.setAttribute("id", p_id);

  ppxl::Segment portalLineIn(p_portal.GetIn());
  element.setAttribute("xaIn", portalLineIn.GetA().GetX());
  element.setAttribute("yaIn", portalLineIn.GetA().GetY());
  element.setAttribute("xbIn", portalLineIn.GetB().GetX());
  element.setAttribute("ybIn", portalLineIn.GetB().GetY());

  ppxl::Segment portalLineOut(p_portal.GetOut());
  element.setAttribute("xaOut", portalLineOut.GetA().GetX());
  element.setAttribute("yaOut", portalLineOut.GetA().GetY());
  element.setAttribute("xbOut", portalLineOut.GetB().GetX());
  element.setAttribute("ybOut", portalLineOut.GetB().GetY());

  return element;
}

void Serializer::AppendPortal(Portal const& p_portal, int p_id) {
  m_portals.appendChild(PortalToNode(p_portal, p_id));
}

void Serializer::SetPortalsList(QList<Portal> const& p_portals) {
  int id = 0;
  for (auto const& portal: p_portals) {
    AppendPortal(portal, id);
    ++id;
  }
}
