#ifndef QPANEL_H
    #define QPANEL_H
    //--------------------
    #include <QDockWidget>
    #include <QVBoxLayout>
    #include <QScrollArea>
    #include <QListWidget>
    #include <QListWidgetItem>
    #include "cjournalwidget.h"
    #include "qcell.h"
    //------------------------------
    class QPanel: public QDockWidget
    {
        public:
            QPanel(QWidget* parent = Q_NULLPTR);
            void setData(const QVector<quint16>& data);
            void setVariableNames(const calc_value_list_t& calc_list);
        
        private:
            QListWidget*    m_central_wgt;
            QVector<QCell*> m_cell_list;
    };
#endif // QPANEL_H
