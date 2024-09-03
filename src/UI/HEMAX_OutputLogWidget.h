#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025)
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qboxlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qwidget.h>
#endif

#include "../HEMAX_Logger.h"

class HEMAX_OutputLogWidget : public QWidget
{
    Q_OBJECT

public:

                                HEMAX_OutputLogWidget();
                                ~HEMAX_OutputLogWidget() = default;

    void                        Initialize();

    void                        LogEntryAdded(const HEMAX_LogEntry& Entry);

private:

    QVBoxLayout*                Layout = nullptr;
    QTextEdit*                  LogText = nullptr;
    QPushButton*                ClearButton = nullptr;
    QPushButton*                SaveLogButton = nullptr;

private slots:

    void                        ClearLog();
    void                        SaveLog();

};
