#ifndef CHEADERJOURNAL_H
    #define CHEADERJOURNAL_H
    //----------------
    #include <QWidget>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CHeaderJournal;
    }
    //----------------------------------
    class CHeaderJournal: public QWidget
    {
        Q_OBJECT

        public:
            explicit CHeaderJournal(QWidget* parent = nullptr);
            ~CHeaderJournal();

            void clear();

            void setTextDeviceCountMessages(int value, int total);
            void setTextElapsedTime(int value);
            void setTextTableCountMessages(int value);
            bool stateCheckbox();

        signals:
            void clickedButtonRead(bool = false);
            void clickedButtonClear(bool = false);
            void stateButtonReadOff(bool = false);

        public slots:
            void stateEnabledButtonReadChanged(bool checked = false);
            void stateButtonReadChanged(bool checked = false);

        private:
            Ui::CHeaderJournal* ui;
    };
#endif // CHEADERJOURNAL_H
