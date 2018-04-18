#ifndef CDOCKPANEL_H
    #define CDOCKPANEL_H
    //----------------
    #include <QWidget>
    #include <QFrame>
    #include <QLabel>
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QDebug>
    //-----------------------------
    class CDockPanel: public QFrame
    {
        Q_OBJECT

        public:
            explicit CDockPanel(QWidget* parent = nullptr);
            ~CDockPanel();

            QLabel*  header() const;
            QLabel*  footer() const;
            QWidget* widget() const;

            QString headerText() const;
            QString footerText() const;

            void setHeader(QLabel* header);
            void setFooter(QLabel* footer);
            void setWidget(QWidget* widget);

            void setHeaderText(const QString& text);
            void setFooterText(const QString& text);

            void setHeaderVisible(bool visible);
            void setFooterVisible(bool visible);

        private:
            QLabel*  m_header;
            QLabel*  m_footer;
            QWidget* m_widget;
            bool     m_header_visible;
            bool     m_footer_visible;
    };
#endif // CDOCKPANEL_H
