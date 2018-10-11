#ifndef CONTAINERWIDGET_H
    #define CONTAINERWIDGET_H
    //----------------
    #include <QFrame>
    #include <QMouseEvent>
    #include <QDrag>
    #include <QMimeData>
    #include <QToolButton>
    #include <QDebug>
    #include <QSizeGrip>
    #include <QPainter>
    //----------
    namespace Ui
    {
        class CContainerWidget;
    }
    //-----------------------------------
    class CContainerWidget: public QFrame
    {
        Q_OBJECT

        public:
            enum class AnchorType
            {
                AnchorFree,
                AnchorDockWidget
            };

        public:
            explicit CContainerWidget(const QString& title, QWidget* contentWidget, AnchorType anchor, QWidget* parent = nullptr);
            ~CContainerWidget();

            AnchorType anchor() const;
            QWidget* widget();
            void functionButtonHide();
            void functionButtonShow();
            QColor backgroundColorHeader() const;
            QString headerTitle() const;
            int  id() const;
            bool buttonFunctionState() const;
            QToolButton* buttonFunction() const;
            QWidget* superParent() const;
            void setWidget(QWidget* wgt);
            void setHeaderBackground(const QColor& backgroundColor);
            void setAnchor(AnchorType anchor);
            void setID(int id);
            void setSuperParent(QWidget *parent);
            void setButtonFunctionState(bool state);

        public slots:
            void buttonFunctionStateChanged(bool state = false);
            void expandedContainer();

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
            QPoint                m_size_pos;
            int                   m_id;
            QColor                m_background_color;
    };
#endif // CONTAINERWIDGET_H
