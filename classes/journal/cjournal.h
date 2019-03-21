#ifndef CJOURNAL_H
    #define CJOURNAL_H
    //----------------
    #include "cjournalwidget.h"
    #include "modbusdataunit.h"
    #include "cfilter.h"
    //------------
    class CJournal
    {
        private:
            int m_msg_size; // размер сообщения (количество ячеек, которое занимает сообщение)
            int m_request_size; // размер запроса по умолчанию (содержит количество сообщений в одном запросе)
            int m_request_last_count; // размер последнего запроса (для проверки пришедших данных)
            int m_msg_read_on_page; // текущее сообщение на странице (локальный счетчик прочитанных сообщений на странице)
            int m_msg_read_count; // количество прочитанных сообщений (глобальный счетчик прочитанных сообщений)
            int m_addr_page_start; // адрес начальной страницы
            int m_page_addr_cur; // адрес текущей страницы
            int m_addr_msg_num_r; // адрес чтения доступных сообщений в устройстве (только чтение)
            int m_addr_page_ptr_rw; // адрес чтения/записи указателя на текущую страницу (окно чтения)
            int m_msg_total_num; // количество сообщений доступных для чтения
            int m_msg_start_ptr; // указатель на сообщение с которого начинается чтение
            int m_msg_limit; // количество сообщений которое необходимо прочитать
            int m_request_time; // скорость выполнения запроса (накопитель времени запросов)
            bool m_is_msg_read_state; // состояние чтения
            bool m_is_msg_part; // запрос был частью от одного запроса
            CModBusDataUnit::vlist_t m_msg_buffer; // буфер запросов
            CJournalWidget* m_widget; // указатель на виджет журнала
            CFilter         m_filter; // фильтр чтения сообщений

        public:
            enum { PAGE_LIMIT = 8192, PAGE_SIZE = 4096 }; // константа ограничения размера страницы (если дошли до этой границы, то необходимо сместить окно чтения)
            CJournal();
            CJournal(int addr_page_start, int msg_size, int request_size, int addr_msg_num, int addr_page_ptr, CJournalWidget *widget);
            int addrMsgNum() const;
            int addrPagePtr() const;
            int addrPageStart() const;
            void clear();
            CFilter& filter();
            bool isMsgPart() const;
            CModBusDataUnit::vlist_t& msgBuffer();
            int msgReadOnPage() const;
            int msgLimit() const;
            int msgReadCount() const;
            int msgSize() const;
            int msgStartPtr() const;
            int msgTotalNum() const;
            int nextPageAddr(bool *isShift);
            int nextRequestSize();
            int pageAddrCur() const;
            void print(const CModBusDataUnit &unit);
            int requestSize() const;
            bool isMsgReadState() const;
            CJournalWidget *widget() const;

            void appendMsgToBuffer(CModBusDataUnit::vlist_t &msg_buf);
            void setAddrMsgNum(int value);
            void setAddrPagePtr(int value);
            void setAddrPageStart(int value);
            void setFilter(CFilter& filter);
            void setMsgOnPage(int value);
            void setMsgLimit(int value);
            void setMsgPart(bool state);
            void setMsgReadCount(int value);
            void setMsgSize(int value);
            void setMsgStartPtr(int value);
            void setMsgTotalNum(int value);
            void setPageAddrCur(int value);
            void setRequestSize(int value);
            void setMsgReadState(bool state);
            void widget(CJournalWidget *widget);
    };
    typedef CJournal *JournalPtr;
    Q_DECLARE_METATYPE(JournalPtr)
#endif // CJOURNAL_H
