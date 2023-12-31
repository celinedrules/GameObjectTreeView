#include "hierarchytreeviewdelegate.h"

QWidget *HierarchyTreeViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // Call the base class createEditor function to create the editor widget
    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
    // Set the size policy of the editor to expanding, which means it will take up as much space as possible
    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Set the style sheet of the editor to have a dark gray background
    editor->setStyleSheet("background: #434343;");
    // Return the created editor widget
    return editor;
}
