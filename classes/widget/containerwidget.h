#ifndef CONTAINERWIDGET_H
    #define CONTAINERWIDGET_H
    //----------------
    #include <QWidget>
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
                AnchorVariable
            };

        public:
            explicit CContainerWidget(const QString& title, QWidget* contentWidget, AnchorType anchor, QWidget* parent = nullptr);
            ~CContainerWidget();
            AnchorType anchor() const;
            QWidget* widget();
            void buttonHide();
            void buttonShow();
            void setWidget(QWidget* wgt);
            void setHeaderBackground(const QColor& backgroundColor);
            void setAnchor(AnchorType anchor);

        private:
            Ui::CContainerWidget* ui;
            QWidget*              m_contentWidget;
            AnchorType            m_anchor;
};

#endif // CONTAINERWIDGET_H
