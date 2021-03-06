#ifndef CVARIABLEWIDGET_H
    #define CVARIABLEWIDGET_H
    //----------------
    #include <QObject>
    #include <QWidget>
    #include <QVBoxLayout>
    #include <QSqlDatabase>
    #include <QSqlQuery>
    #include "cjournalwidget.h"
    #include "qcell.h"
    #include "cvaribalelist.h"
    //-----------------------------------
    class CVariableWidget: public QWidget
    {
        Q_OBJECT

        public:
            struct var_t
            {
                QString type;
                int     id;
                QString index;
                QString name;
                QString unit;
                QString description;
            };

            struct var_data_t
            {
                var_t var1;
                var_t var2;
            };

            typedef QVector<var_data_t> var_list_t;

            struct var_group_t
            {
                QString     name;
                QStringList columns;
                var_list_t  var_list;
            };

        public:
            explicit CVariableWidget(QWidget* parent = nullptr);

            void setData(const QVector<quint16>& data);
            void setTitle(const QString& title);
            void setID(int id);
            void setHeaderBackground(const QLinearGradient& gradient);
            void init(QSqlDatabase& database);
            int  cellCount() const;
            int  id() const;

        signals:

        public slots:

        private:
            void initialize();
            bool loadGroups(QSqlDatabase& database);
            QStringList loadColumns(QSqlDatabase& database, int group_id);
            var_list_t loadRows(QSqlDatabase& database, int group_id);
            var_t loadData(QSqlDatabase& database, const QString& index);
            int insertGroupHeader(const QString& text, int row);
            int insertColumnLabels(const QStringList& list, int row);
            int insertGroupRows(const var_list_t& var_list, int row);
            int columnMaxWidth(int col);

        private:
            CVaribaleList*         m_variablelist;
            QStringList            m_variables;
            QMap<int, var_group_t> m_groups;
            QVector<QLineEdit*>    m_line_var;
            int                    m_id;
            static QColor          m_background_item_color;
    };
#endif // CVARIABLEWIDGET_H
