#include "contextmodel.h"
#include <iostream>

ContextModel::ContextModel(QObject *parent) : QAbstractListModel(parent) { }
const RDContextPtr& ContextModel::context() const { return m_context; }
const RDDocument* ContextModel::document() const { return m_document; }

void ContextModel::setContext(const RDContextPtr& context)
{
    m_context = context;
    m_document = RDContext_GetDocument(context.get());
}

QVariant ContextModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)

    if((orientation == Qt::Horizontal) && (role == Qt::TextAlignmentRole))
        return Qt::AlignCenter;

    return QVariant();
}
