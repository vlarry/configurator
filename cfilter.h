#ifndef CFILTER_H
    #define CFILTER_H
    //--------------
    #include <QDate>
    //-----------
    class CFilter
    {
        public:
            struct FilterIntervalType
            {
                int max;
                int begin;
                int end;
            };
            struct FilterDateType
            {
                QDate begin;
                QDate end;
            };
            enum FilterType { INTERVAL, DATE };
            enum { OFF, ON };

        public:
            CFilter();
            CFilter(const FilterIntervalType& interval, const FilterDateType& date,
                    const QTime& time = QTime::fromString("00:00:00", "HH:mm:ss"));

            const FilterDateType&     date() const;
            const FilterIntervalType& interval() const;
            vod                       reset();
            QTime                     time() const;
            bool                      state() const;
            FilterType                type() const;

            void setDate(const FilterDateType& date);
            void setInterval(const FilterIntervalType& interval);
            void setTime(const QTime& time);
            void setState(bool state);
            void setType(FilterType type);

            operator bool() const
            {
                return m_state;
            }

        private:
            FilterIntervalType m_interval;
            FilterDateType     m_date;
            QTime              m_time;
            bool               m_state;
            FilterType         m_type;
    };
#endif // CFILTER_H
