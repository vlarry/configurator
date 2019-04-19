#ifndef CJOURNAL_H
    #define CJOURNAL_H
    //----------------
    #include <QHeaderView>
    #include "cjournalwidget.h"
    #include "modbusdataunit.h"
    #include "cfilter.h"
    //------------
    class CJournal
    {
        private:
            const int PAGE_SIZE = 4096; // размер страницы в ячейках протокола модбас (1 ячейка = 2 байта)
            const int m_msg_size; // размер сообщения в ячейках
            const int m_addr_msg_count; // адрес чтения количества доступных сообщений
            const int m_addr_shift_ptr; // адрес чтения/записи указателя смещения окна чтения
            const int m_page_addr_start; // начало чтения, т.е. с этого адреса идет чтение прии переходе
            const int m_msg_on_page; // максимальное количество сообщений на странице
            const int m_default_reques_size; // размер запроса по умолчанию (количество сообщений в одном запросе)

            int m_shift_ptr_cur; // позиция указателя смещения окна чтения
            int m_msg_count; // счетчик прочитанных сообщений
            int m_msg_to_shift; // количество сообщений до смещения указателя окна
            int m_msg_read; // количество сообщений которое необходимо прочитать
            CFilter m_filter; // данные фильтра
            CJournalWidget *m_widget; // виджет журнала
            CModBusDataUnit::vlist_t m_buffer; // буфер для хранения ответов устройства
            bool m_isRead; // флаг чтения журнала (для отслеживания оставновки)
            bool m_isShiftPrt; // флаг смещения окна чтения

        public:
            CJournal(int msg_size, int addr_msg_count, int addr_shift_ptr, int page_addr_start,
                     CJournalWidget *widget = nullptr);
            int msgSize() const;
            int addrMsgCount() const;
            int addrShiftPtr() const;
            int pageAddrStart() const;
            int msgOnPage() const;
            int defaultRequesSize() const;
            int shiftPtrCur() const;
            int msgCount() const;
            int msgToShift() const;
            int msgRead() const;
            CFilter &filter();
            CJournalWidget *widget();
            void initRead();
            CModBusDataUnit read(int id, int type, bool *isShift);
            void receiver(const CModBusDataUnit::vlist_t &data);
            bool isReadState() const;
            bool isShiftPtr() const;
            void setReadState(bool state);
            void reset();
            void print();
    };
    typedef CJournal *JournalPtr;
    Q_DECLARE_METATYPE(JournalPtr)
#endif // CJOURNAL_H
