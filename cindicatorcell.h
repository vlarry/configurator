#ifndef CINDICATORCELL_H
    #define CINDICATORCELL_H
    //----------------
    #include <QWidget>
    //----------
    namespace Ui
    {
        class CIndicatorCell;
    }
    //----------------------------------
    class CIndicatorCell: public QWidget
    {
        Q_OBJECT

        public:
            explicit CIndicatorCell(QWidget* parent = nullptr);
            ~CIndicatorCell();

            void setText(const QString& text);
            void setIcon(const QPixmap& icon);

        private:
            Ui::CIndicatorCell* ui;
    };
#endif // CINDICATORCELL_H
