#ifndef QCELL_H
    #define QCELL_H
    //----------------
    #include <QWidget>
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
            explicit QCell(const QString& cell_name, QWidget* parent = Q_NULLPTR);
            ~QCell();
            void setCellName(const QString& name);
            void setCellValue(float value);
        
        private:
            Ui::QCell* ui;
    };
#endif // QCELL_H
