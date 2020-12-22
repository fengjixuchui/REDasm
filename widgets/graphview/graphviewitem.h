#pragma once

#include <QObject>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <rdapi/graph/graph.h>

class GraphViewItem: public QObject
{
    Q_OBJECT

    public:
        enum: size_t { None = 0, Selected, Focused };

    public:
        explicit GraphViewItem(RDGraphNode node, const RDGraph* g, QObject* parent = nullptr);
        virtual ~GraphViewItem() = default;
        const RDGraph* graph() const;
        RDGraphNode node() const;
        int x() const;
        int y() const;
        int width() const;
        int height() const;
        QRect rect() const;
        bool contains(const QPoint& p) const;
        const QPoint& position() const;
        void move(const QPoint &pos);

    protected:
        virtual void itemSelectionChanged(bool selected);
        virtual void mouseDoubleClickEvent(QMouseEvent *e);
        virtual void mousePressEvent(QMouseEvent *e);
        virtual void mouseMoveEvent(QMouseEvent *e);

    public:
        QPoint mapToItem(const QPoint& p) const;
        virtual int currentRow() const;
        virtual void render(QPainter* painter, size_t state) = 0;
        virtual QSize size() const = 0;

    public slots:
        virtual void invalidate(bool notify = true);

    signals:
        void invalidated();

    private:
        QPoint m_pos;
        RDGraphNode m_node;
        const RDGraph* m_graph;

    friend class GraphView;
};
