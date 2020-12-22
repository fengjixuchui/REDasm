#pragma once

#include "contextmodel.h"
#include <rdapi/rdapi.h>
#include "../hooks/isurface.h"

class CallTreeModel : public ContextModel
{
    Q_OBJECT

    public:
        explicit CallTreeModel(QObject *parent = nullptr);
        RDDocumentItem item(const QModelIndex& index) const;
        void initializeGraph(rd_address address);

    public:
        bool hasChildren(const QModelIndex& parentindex) const override;
        QModelIndex index(int row, int column, const QModelIndex &parent) const override;
        QModelIndex parent(const QModelIndex &child) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        int columnCount(const QModelIndex&) const override;
        int rowCount(const QModelIndex& parent) const override;

    public slots:
        void populateCallGraph(const QModelIndex& index);

    private:
        //REDasm::object_ptr<REDasm::Printer> m_printer;
        //std::unique_ptr<REDasm::CallTree> m_calltree;
        //REDasm::ListingItem m_currentitem;
};
