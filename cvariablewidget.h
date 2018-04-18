#ifndef CVARIABLEWIDGET_H
    #define CVARIABLEWIDGET_H
    //----------------
    #include <QObject>
    #include <QWidget>
    #include <QVBoxLayout>
    #include "cjournalwidget.h"
    #include "qcell.h"
    #include "cvaribalelist.h"
    //-----------------------------------
    class CVariableWidget: public QWidget
    {
        Q_OBJECT

        public:
            explicit CVariableWidget(QWidget* parent = nullptr);

            void setData(const QVector<quint16>& data);
            void setVariableNames(const calc_value_list_t& calc_list);
            void resizeSize();

        signals:

        public slots:

        private:
            CVaribaleList* m_variablelist;
            QStringList    m_variables;
    };
#endif // CVARIABLEWIDGET_H
