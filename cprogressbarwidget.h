#ifndef CPROGRESSBARWIDGET_H
    #define CPROGRESSBARWIDGET_H
    //----------------
    #include <QWidget>
    #include <QProgressBar>
    #include <QLabel>
    #include <QFont>
    #include <QFontMetrics>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CProgressBarWidget;
    }
    //--------------------------------------
    class CProgressBarWidget: public QWidget
    {
        Q_OBJECT

        public:
            explicit CProgressBarWidget(QWidget* parent = nullptr);
            ~CProgressBarWidget();

            void progressTitleClear();
            int  progressValue();
            void setProgressFormat(const QString& text);
            void setProgressRange(int min, int max);
            void setProgressTitle(const QString& title);
            void setProgressValue(int value);

        public slots:
            void progressIncrement(int value = 1);
            void progressStart();
            void progressStop();
            void setSettings(int min, int max, const QString& format = "");

        signals:
            void settingChanged(int min, int max, const QString& format);
            void increment(int = 1);

        private:
            Ui::CProgressBarWidget* ui;
            QString                 m_format;
    };
#endif // CPROGRESSBARWIDGET_H
