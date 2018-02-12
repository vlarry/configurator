#ifndef CHEADERJOURNAL_H
    #define CHEADERJOURNAL_H
    //----------------
    #include <QWidget>
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

            void setTextDeviceCountMessages(int value, int total);
            void setTextElapsedTime(int value);
            void setTextTableCountMessages(int value);

        private:
            Ui::CHeaderJournal* ui;
    };
#endif // CHEADERJOURNAL_H
