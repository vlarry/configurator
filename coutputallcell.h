#ifndef COUTPUTALLCELL_H
    #define COUTPUTALLCELL_H
    //----------------
    #include <QWidget>
    //----------
    namespace Ui
    {
        class COutputAllCell;
    }
    //----------------------------------
    class COutputAllCell: public QWidget
    {
        Q_OBJECT

        public:
            explicit COutputAllCell(const QString& name, QWidget* parent = nullptr);
            ~COutputAllCell();

            QString name() const;
            bool state() const;

            void setName(const QString& name);
            void setState(bool state);

        private:
            Ui::COutputAllCell* ui;
    };
#endif // COUTPUTALLCELL_H
