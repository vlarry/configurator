#ifndef QCELL_H
    #define QCELL_H
    //----------------
    #include <QWidget>
    #include <QDebug>
    //----------
    namespace Ui 
    {
        class QCell;
    }
    //-------------------------
    class QCell: public QWidget
    {
        Q_OBJECT
    
        public:
            explicit QCell(QWidget* parent = nullptr);
            explicit QCell(const QString& cell_name, QWidget* parent = nullptr);
            ~QCell();
            void setCellName(const QString& name);
            void setCellValue(float value);
            void setCellValue(int value);

            QSize labelSize() const;
            QSize fieldSize() const;
        
        private:
            Ui::QCell* ui;
    };
#endif // QCELL_H
