#include "hierarchytreeviewdelegate.h"

QWidget *HierarchyTreeViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editor->setStyleSheet("background: #434343;");
    return editor;
}
