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
    #include "cdockpanelitemctrl.h"
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
            void headerShow() const;
            void headerHide() const;
            QWidget* superParent() const;
            CDockPanelItemCtrl::DirType side() const;
            int position() const;
            QString name() const;
            void setWidget(QWidget* wgt);
            void setHeaderBackground(const QColor& backgroundColor);
            void setAnchor(AnchorType anchor);
            void setID(int id);
            void setSuperParent(QWidget *parent);
            void setButtonFunctionState(bool state);
            void setSide(CDockPanelItemCtrl::DirType dir);
            void setPosition(int pos);
            void setName(const QString& name);

        public slots:
            void buttonFunctionStateChanged(bool state = false);
            void expandedContainer();
            void processClose();

        signals:
            void removeContainer(int);
            void containerClicked(QMouseEvent*);
            void containerMoved(QMouseEvent*, int);
            void containerClose(bool = false);

        protected:
            bool eventFilter(QObject* object, QEvent* event) override;

        private:
            Ui::CContainerWidget*       ui;
            QWidget*                    m_superParent;
            QWidget*                    m_contentWidget;
            AnchorType                  m_anchor;
            CDockPanelItemCtrl::DirType m_dock_side;
            QPoint                      m_pos;
            QPoint                      m_pos_grip;
            int                         m_id;
            QColor                      m_background_color;
            int                         m_position;
            QString                     m_name;
    };
#endif // CONTAINERWIDGET_H
