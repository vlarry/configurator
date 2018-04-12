#ifndef CDOCKPANEL_H
    #define CDOCKPANEL_H
    //----------------
    #include <QWidget>
    //----------
    namespace Ui
    {
        class CDockPanel;
    }
    /*!
     * \brief The CDockPanel class
     *
     * Виджет ввиде панели
     */
    class CDockPanel: public QWidget
    {
        Q_OBJECT

        public:
            explicit CDockPanel(QWidget* parent = nullptr);
            ~CDockPanel();

        private:
            Ui::CDockPanel* ui;
    };
#endif // CDOCKPANEL_H
