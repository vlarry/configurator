#ifndef CMONITORPURPOSE_H
    #define CMONITORPURPOSE_H
    //----------------
    #include <QWidget>
    #include <QTableView>
    #include "cmatrixpurposemodel.h"
    //----------
    namespace Ui
    {
        class CMonitorPurpose;
    }
    //-----------------------------------
    class CMonitorPurpose: public QWidget
    {
        Q_OBJECT

        public:
            explicit CMonitorPurpose(const QString& text, QWidget* parent = nullptr);
            ~CMonitorPurpose();

            void        setHeaders(const QStringList& row_labels, const QStringList& column_labels);
            QTableView* table() const;

        private slots:
            void clearTable();

        protected:
            void closeEvent(QCloseEvent* event);

        signals:
            void closeWindow(bool = false);
            void readPurpose();

        private:
            Ui::CMonitorPurpose* ui;
    };
#endif // CMONITORPURPOSE_H
