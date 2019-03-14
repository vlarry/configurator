#ifndef CJOURNAL_H
    #define CJOURNAL_H
    //----------------
    #include "cjournalwidget.h"
    #include "modbusdataunit.h"
    //------------
    class CJournal
    {
        public:
            typedef quint16 cell_t;
            typedef QVector<cell_t> data_t;

        private:
            int m_shift_prt; // указатель сдвига окна чтения
            int m_size_msg; // размер одного сообщения в байтах
            int m_size_request; // количество сообщений в одном запросе
            int m_count_msg; // счетчик принятых сообщений
            int m_count_limit; // счетчик сообщений, которые необходимо прочитать
            int m_count_total; // всего доступно сообщений
            int m_address_shift; // адрес чтения/записи указателя смщения окна чтения
            int m_address_number; // адрес чтения количества записей в журнале
            CModBusDataUnit::vlist_t m_buffer; // буфер хранения данных
            CJournalWidget* m_widget; // указатель на виджет журнала

            static bool m_state;

        public:
            CJournal();
            CJournal(int shift_ptr, int size_msg, int size_request, int addr_shift, int addr_num, CJournalWidget* widget);
            ~CJournal();
            int shiftPrt();
            int sizeMsg();
            int sizeRequest();
            CModBusDataUnit::vlist_t &buffer();
            CJournalWidget* widget();

            void appendData(const CModBusDataUnit::vlist_t &data);
            void setShiftPtr(int shift_ptr);
    };
    typedef CJournal *JournalPtr;
    Q_DECLARE_METATYPE(JournalPtr)
#endif // CJOURNAL_H
