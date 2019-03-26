#ifndef CFILTER_H
    #define CFILTER_H
    //--------------
    #include <QDate>
    //-----------
    class CFilter
    {
        public:
            struct RangeType
            {
                int min;
                int max;
            };
            //--------------
            struct LimitType
            {
                int low;
                int upper;
            };
            //-------------
            struct DateType
            {
                QDate from;
                QDate to;
            };
            //-------------
            enum FilterType
            {
                FilterDateType,
                FilterLimitType
            };

        private:
            RangeType  m_range;
            LimitType  m_limit;
            DateType   m_date;
            QTime      m_time;
            FilterType m_type;
            bool       m_state;

        public:
            CFilter();
            CFilter(int min, int max, int low, int upper, QDate& from, QDate& to);
            CFilter(RangeType &range, LimitType &limit, DateType& date);
            RangeType& range();
            LimitType& limit();
            DateType& date();
            int rangeMinValue() const;
            int rangeMaxValue() const;
            int limitLowValue() const;
            int limitUpperValue() const;
            QDate &dateFrom();
            QDate &dateTo();
            QTime &time();
            FilterType type() const;
            bool state() const;
            void reset();
            operator bool() const;

            void setRange(RangeType &range);
            void setRange(int min, int max);
            void setLimit(LimitType &limit);
            void setLimit(int low, int upper);
            void setDate(DateType &date);
            void setDate(const QDate &from, const QDate &to);
            void setTime(const QTime &time);
            void setType(FilterType type);
            void setState(bool state);
    };
#endif // CFILTER_H
