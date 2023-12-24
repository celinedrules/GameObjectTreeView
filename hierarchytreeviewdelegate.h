#ifndef HIERARCHYTREEVIEWDELEGATE_H
#define HIERARCHYTREEVIEWDELEGATE_H

#include <QEvent>
#include <QMouseEvent>
#include <QStyledItemDelegate>


class HierarchyTreeViewDelegate : public QStyledItemDelegate {
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


#endif // HIERARCHYTREEVIEWDELEGATE_H
