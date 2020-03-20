#ifndef POLYGONITEMDELEGATE_HXX
#define POLYGONITEMDELEGATE_HXX

#include <QStyledItemDelegate>
#include <QPainter>

class PolygonItemDelegate: public QStyledItemDelegate {
  Q_OBJECT

public:
  PolygonItemDelegate(QObject* parent = nullptr);

  QWidget* createEditor(QWidget* p_parent, QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const override;
  void setEditorData(QWidget* p_editor, QModelIndex const& p_option) const override;
  void setModelData(QWidget* p_editor, QAbstractItemModel* p_model, QModelIndex const& p_index) const override;
  void updateEditorGeometry(QWidget* p_editor, QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const override;

Q_SIGNALS:
  void ValueXChanged(int p_value, QModelIndex const& p_index) const;
  void ValueYChanged(int p_value, QModelIndex const& p_index) const;
  void EditionXDone(int p_value, QModelIndex const& p_index) const;
  void EditionYDone(int p_value, QModelIndex const& p_index) const;
};

#endif
