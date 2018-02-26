#ifndef CLISTWIDGETITEM_H
    #define CLISTWIDGETITEM_H
    //----------------
    #include <QWidget>
    //----------
    namespace Ui
    {
        class CListWidgetItem;
    }
    //-----------------------------------
    class CListWidgetItem: public QWidget
    {
        Q_OBJECT

        public:
            explicit CListWidgetItem(QWidget* parent = nullptr);
            ~CListWidgetItem();

            void setPropertyData(const QString& name, const QString& value);
            void setPropertyName(const QString& name);
            void setPropertyValue(const QString& value);

        private:
            Ui::CListWidgetItem* ui;
    };
#endif // CLISTWIDGETITEM_H
