#include "listingitemmodel.h"
#include <redasm/disassembler/listing/listingdocument.h>
#include <redasm/plugins/assembler/assembler.h>
#include <redasm/plugins/loader/loader.h>
#include <redasm/support/demangler.h>
#include <redasm/support/utils.h>
#include "../themeprovider.h"
#include "../convert.h"
#include <QColor>

ListingItemModel::ListingItemModel(REDasm::ListingItemType itemtype, QObject *parent) : DisassemblerModel(parent), m_itemtype(itemtype) { }

void ListingItemModel::setDisassembler(const REDasm::DisassemblerPtr& disassembler)
{
    DisassemblerModel::setDisassembler(disassembler);
    auto lock = REDasm::s_lock_safe_ptr(m_disassembler->documentNew());

    this->beginResetModel();

    for(size_t i = 0; i < lock->items()->size(); i++)
    {
        const REDasm::ListingItem& item = lock->items()->at(i);

        if(!this->isItemAllowed(item))
            continue;

        m_items.insert(item.address_new);
    }

    this->endResetModel();
    lock->changed.connect(this, std::bind(&ListingItemModel::onListingChanged, this, std::placeholders::_1));
}

REDasm::ListingItem ListingItemModel::item(const QModelIndex &index) const
{
    if(!index.isValid() || (index.row() >= m_items.size()))
        return REDasm::ListingItem();

    auto lock = REDasm::s_lock_safe_ptr(m_disassembler->documentNew());
    REDasm::ListingItem item;

    if(m_itemtype == REDasm::ListingItemType::SegmentItem)
        item = lock->items()->segmentItem(m_items[index.row()].toU64());
    else if(m_itemtype == REDasm::ListingItemType::FunctionItem)
        item = lock->items()->functionItem(m_items[index.row()].toU64());
    else
    {
        item = lock->items()->instructionItem(m_items[index.row()].toU64()); // Try to get an instruction

        if(!item.isValid())
            item = lock->items()->symbolItem(m_items[index.row()].toU64());  // Try to get an symbol
    }

    return item;
}

address_location ListingItemModel::address(const QModelIndex &index) const
{
    if(!index.isValid() || (index.row() < 0) || (index.row() >= m_items.size()))
        return REDasm::invalid_location<address_t>();

    return REDasm::make_location(m_items[index.row()].toU64());
}

QModelIndex ListingItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if((row < 0) || (row >= m_items.size()))
        return QModelIndex();

    return this->createIndex(row, column);
}

int ListingItemModel::rowCount(const QModelIndex &) const { return m_items.size(); }
int ListingItemModel::columnCount(const QModelIndex &) const { return 4; }

QVariant ListingItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical) return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(section == 0)      return "Address";
        else if(section == 1) return "Symbol";
        else if(section == 2) return "R";
        else if(section == 3) return "Segment";
    }

    return DisassemblerModel::headerData(section, orientation, role);
}

QVariant ListingItemModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();

    auto lock = REDasm::s_lock_safe_ptr(m_disassembler->documentNew());
    const REDasm::Symbol* symbol = lock->symbols()->get(m_items[index.row()].toU64());

    if(!symbol) return QVariant();

    if(role == Qt::DisplayRole)
    {
        if(index.column() == 0)
            return Convert::to_qstring(REDasm::String::hex(symbol->address, m_disassembler->assembler()->bits()));

        if(index.column() == 1)
        {
            if(symbol->is(REDasm::SymbolType::StringNew))
            {
                const REDasm::BlockItem* block = lock->block(symbol->address);
                if(!block) return QVariant();

                if(symbol->hasFlag(REDasm::SymbolFlags::WideString))
                    return Convert::to_qstring(m_disassembler->readWString(symbol->address, block->size()).quoted());

                return Convert::to_qstring(m_disassembler->readString(symbol->address, block->size()).quoted());
            }

            return Convert::to_qstring(REDasm::Demangler::demangled(symbol->name));
        }

        if(index.column() == 2)
            return QString::number(m_disassembler->getReferencesCount(symbol->address));

        if(index.column() == 3)
        {
            const REDasm::Segment* segment = lock->segments()->find(symbol->address);
            return segment ? Convert::to_qstring(segment->name) : "???";
        }
    }
    else if(role == Qt::BackgroundRole)
    {
        if(symbol->isEntryPoint()) return THEME_VALUE("entrypoint_bg");
        if(symbol->isFunction() && symbol->isLocked()) return THEME_VALUE("locked_bg");
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 0) return THEME_VALUE("address_list_fg");
        if(symbol->is(REDasm::SymbolType::StringNew) && (index.column() == 1)) return THEME_VALUE("string_fg");
    }

    return QVariant();
}

bool ListingItemModel::isItemAllowed(const REDasm::ListingItem& item) const
{
    if(m_itemtype == REDasm::ListingItemType::AllItems)
        return true;

    return item.is(m_itemtype);
}

void ListingItemModel::onListingChanged(REDasm::EventArgs* e)
{
    REDasm::ListingDocumentChangedEventArgs *ldc = static_cast<REDasm::ListingDocumentChangedEventArgs*>(e);

    if(!this->isItemAllowed(ldc->itemNew()))
        return;

    if(ldc->isRemoved())
    {
        int idx = static_cast<int>(m_items.indexOf(ldc->itemNew().address_new));
        this->beginRemoveRows(QModelIndex(), idx, idx);
        m_items.eraseAt(static_cast<size_t>(idx));
        this->endRemoveRows();
    }
    else if(ldc->isInserted())
    {
        int idx = static_cast<int>(m_items.insertionIndex(ldc->itemNew().address_new));
        this->beginInsertRows(QModelIndex(), idx, idx);
        m_items.insert(ldc->itemNew().address_new);
        this->endInsertRows();
    }
}
