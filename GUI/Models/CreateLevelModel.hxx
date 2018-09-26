#ifndef CREATELEVELMODEL_HXX
#define CREATELEVELMODEL_HXX

#include "GUI/Models/PolygonModel.hxx"

#include <QStack>
#include <QPair>

class CreateLevelModel: public PolygonModel {
  Q_OBJECT

public:
  using Selection = QPair<int, int>;
  using SelectionStack = QStack<Selection>;

  CreateLevelModel(QObject* p_parent = nullptr);
  ~CreateLevelModel() override;

  inline SelectionStack GetSelection() const { return m_selections; }
  inline void PushSelection(int p_polygonRow, int p_vertexRow) { m_selections << Selection({p_polygonRow, p_vertexRow}); }
  inline Selection PopSelection() { return m_selections.pop(); }

  // Polygon
  void SetPolygon(int p_polygonRow, ppxl::Polygon const& p_polygon);
  void TranslatePolygon(int p_polygonRow, ppxl::Vector const& p_direction);

  // Vertex
  void InsertVertex(int p_polygonRow, int p_vertexRow, ppxl::Point const& p_vertex);
  void RemoveVertexAt(int p_polygonRow, int p_vertexRow);
  void TranslateVertex(int p_polygonRow, int p_vertexRow, ppxl::Vector const& p_direction);
  QModelIndex GetVertexIndex(int p_polygonRow, int p_vertexRow) const;

private:
  SelectionStack m_selections;
};

#endif
