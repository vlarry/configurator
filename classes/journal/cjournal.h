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

            int m_shift_ptr_cur; // позиция указателя смещения окна чтения
            int m_msg_count; // счетчик прочитанных сообщений
            int m_msg_cur; // текущее сообщение (относительно страницы - при смещении указателя окна обнуляется)
            int m_msg_read; // количество сообщений которое необходимо прочитать
            CFilter m_filter; // данные фильтра
            CJournalWidget *m_widget; // виджет журнала
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
            int shiftPtrCur() const;
            int msgCount() const;
            int msgCurrent() const;
            int msgRead() const;
            CFilter &filter();
            CJournalWidget *widget();
            void initRead();
            CModBusDataUnit read();
            void receiver(CModBusDataUnit::cell_t &data);
            bool isReadState() const;
            bool isShiftPtr() const;
            void setReadState(bool state);
    };
    typedef CJournal *JournalPtr;
    Q_DECLARE_METATYPE(JournalPtr)
#endif // CJOURNAL_H
