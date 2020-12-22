#include "splititem.h"
#include "splitview.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QToolBar>
#include <QDialog>
#include "../../hooks/isurface.h"
#include "../../redasmfonts.h"

#define SPLIT_DIALOG_WIDTH  640
#define SPLIT_DIALOG_HEIGHT 480

SplitItem::SplitItem(QWidget* w, SplitView* view, QWidget* parent) : QWidget(parent), m_widget(w), m_view(view)
{
    m_tbactions = new QToolBar();
    m_tbactions->setIconSize({16, 16});
    m_tbactions->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->createDefaultButtons();

    view->onItemCreated(this);

    auto* lcontent = new QVBoxLayout();
    lcontent->setSpacing(0);
    lcontent->setMargin(0);
    lcontent->addWidget(m_tbactions);
    lcontent->addWidget(w, 1);

    m_container = new QWidget();
    m_container->setLayout(lcontent);

    QVBoxLayout* l = new QVBoxLayout();
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(m_container);

    this->setLayout(l);
    w->setFocus();
}

QWidget* SplitItem::widget() const { return m_widget; }

QAction* SplitItem::addButton(const QIcon& icon)
{
    if(!m_actfirstdefault) return m_tbactions->addAction(icon, QString());

    auto* act = new QAction(icon, QString(), m_tbactions);
    m_tbactions->insertAction(m_actfirstdefault, act);
    return act;
}

void SplitItem::splitInDialog()
{
    QVBoxLayout* l = new QVBoxLayout();
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(m_view->createView());

    QDialog* dialog = m_view->createDialog();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setLayout(l);
    dialog->resize(SPLIT_DIALOG_WIDTH, SPLIT_DIALOG_HEIGHT);
    dialog->show();
}

QAction* SplitItem::action(int idx) const
{
    auto actions = m_tbactions->actions();
    return actions.at(idx);
}

void SplitItem::setCanClose(bool b) { m_actclose->setVisible(b); }
void SplitItem::splitHorizontal() { return this->split(Qt::Horizontal); }
void SplitItem::splitVertical() { return this->split(Qt::Vertical); }

void SplitItem::unsplit()
{
    m_view->onItemDestroyed(this);
    this->hide();
    this->deleteLater();
}

void SplitItem::createDefaultButtons()
{
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_actfirstdefault = m_tbactions->addWidget(empty);

    connect(m_tbactions->addAction(FA_ICON(0xf105), QString()), &QAction::triggered, this, &SplitItem::splitHorizontal);
    connect(m_tbactions->addAction(FA_ICON(0xf107), QString()), &QAction::triggered, this, &SplitItem::splitVertical);
    connect(m_tbactions->addAction(FA_ICON(0xf2d2), QString()), &QAction::triggered, this, &SplitItem::splitInDialog);

    m_actclose = m_tbactions->addAction(FA_ICON(0xf057), QString());
    connect(m_actclose, &QAction::triggered, this, &SplitItem::unsplit);
}

void SplitItem::split(Qt::Orientation orientation)
{
    auto* splitter = new QSplitter(orientation);
    splitter->setStyleSheet(QString("QSplitter::handle { background-color: '%1'; }").arg(qApp->palette().color(QPalette::Window).name()));

    this->layout()->removeWidget(m_container);
    m_container->deleteLater();
    m_container = nullptr;
    m_tbactions = nullptr;

    auto* item = new SplitItem(m_widget, m_view, this);
    auto* newitem = new SplitItem(m_view->createWidget(), m_view, this);

    splitter->addWidget(item);
    splitter->addWidget(newitem);
    this->layout()->addWidget(splitter);

    m_view->onItemSplit(item, newitem);
}
