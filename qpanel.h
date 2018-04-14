#ifndef QPANEL_H
    #define QPANEL_H
    //--------------------
    #include <QDockWidget>
    #include <QVBoxLayout>
    #include <QScrollArea>
    #include <QListWidget>
    #include <QListWidgetItem>
    #include "cvaribalelist.h"
    #include "cjournalwidget.h"
    #include "qcell.h"
    //------------------------------
    class QPanel: public QDockWidget
    {
        public:
            QPanel(QWidget* parent = Q_NULLPTR);
            void setData(const QVector<quint16>& data);
            void setVariableNames(const calc_value_list_t& calc_list);
            void resizeSize();
        
        private:
            CVaribaleList*  m_central_wgt;
            QStringList     m_varibles;
    };
#endif // QPANEL_H
