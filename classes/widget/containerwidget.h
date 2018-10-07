#ifndef CONTAINERWIDGET_H
    #define CONTAINERWIDGET_H
    //----------------
    #include <QWidget>
    #include <QMouseEvent>
    #include <QDrag>
    #include <QMimeData>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CContainerWidget;
    }
    //------------------------------------
    class CContainerWidget: public QWidget
    {
        Q_OBJECT

        public:
            enum class AnchorType
            {
                AnchorFree,
                AnchorDockWidget
            };

        public:
            explicit CContainerWidget();
            explicit CContainerWidget(const QString& title, QWidget* contentWidget, AnchorType anchor, QWidget* parent = nullptr);
            ~CContainerWidget();
            AnchorType anchor() const;
            QWidget* widget();
            void functionButtonHide();
            void functionButtonShow();
            QColor backgroundColorHeader() const;
            QString headerTitle() const;
            int  id() const;
            QWidget* superParent() const;
            void setWidget(QWidget* wgt);
            void setHeaderBackground(const QColor& backgroundColor);
            void setAnchor(AnchorType anchor);
            void setID(int id);
            void setSuperParent(QWidget *parent);

        signals:
            void removeContainer(int);
            void containerClicked(QMouseEvent*);
            void containerMoved(QMouseEvent*, int);

        protected:
            bool eventFilter(QObject* object, QEvent* event) override;

        private:
            Ui::CContainerWidget* ui;
            QWidget*              m_superParent;
            QWidget*              m_contentWidget;
            AnchorType            m_anchor;
            QPoint                m_pos;
            int                   m_id;
            QColor                m_background_color;
};

#endif // CONTAINERWIDGET_H
