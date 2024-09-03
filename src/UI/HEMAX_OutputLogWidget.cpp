#include "HEMAX_OutputLogWidget.h"

#include "moc_HEMAX_OutputLogWidget.cpp"

#include <functional>

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025)
#include <QtCore/QTime>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qmessagebox.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtCore/QTime>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>
#include <QtGui/qerrormessage.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qmessagebox.h>
#endif

#include <sstream>

HEMAX_OutputLogWidget::HEMAX_OutputLogWidget()
{
    Layout = new QVBoxLayout;

    LogText = new QTextEdit;
    LogText->setReadOnly(true);

    ClearButton = new QPushButton("Clear");
    SaveLogButton = new QPushButton("Save Log to File");

    Layout->addWidget(LogText);
    Layout->addWidget(ClearButton);
    Layout->addWidget(SaveLogButton);

    this->setLayout(Layout); 

    HEMAX_Logger::Instance().RegisterOnEntryAddedCallback(
        std::bind(&HEMAX_OutputLogWidget::LogEntryAdded,
            this, std::placeholders::_1));

    QObject::connect(ClearButton, SIGNAL(clicked()), this, SLOT(ClearLog()));
    QObject::connect(SaveLogButton, SIGNAL(clicked()), this, SLOT(SaveLog()));
}

void
HEMAX_OutputLogWidget::Initialize()
{
    const std::vector<HEMAX_LogEntry>& Entries =
        HEMAX_Logger::Instance().GetEntries();

    for (auto&& Entry : Entries)
    {
        LogText->append(Entry.Text.c_str());
    }
}

void
HEMAX_OutputLogWidget::LogEntryAdded(const HEMAX_LogEntry& Entry)
{
    std::stringstream LogStream;

    if (Entry.Level == HEMAX_LOG_LEVEL_ERROR)
    {
        LogStream << "<font color=\"#e33636\"><b>";
    }
    else if (Entry.Level == HEMAX_LOG_LEVEL_WARN)
    {
        LogStream << "<font color=\"yellow\"><b>";
    }
    else
    {
        LogStream << "<font><b>";
    } 

    QTime NowTime = QTime::currentTime();
    LogStream << "[" << NowTime.toString().toStdString() << "]:</font></b> "
        << Entry.Text.c_str();
    std::string LogEntryText = LogStream.str();
    LogText->append(LogEntryText.c_str());
}

void
HEMAX_OutputLogWidget::ClearLog()
{
    LogText->clear();
}

void
HEMAX_OutputLogWidget::SaveLog()
{
    QFileDialog FileChooser(this, "Save Log to File");
    FileChooser.setAcceptMode(QFileDialog::AcceptSave);
    FileChooser.setFileMode(QFileDialog::AnyFile);
    FileChooser.setNameFilter("Text File (*.txt)");
    int Result = FileChooser.exec();

    if (Result == QDialog::Accepted)
    {
        if (FileChooser.selectedFiles().size() == 1)
        {
            int ErrorOccurred = false;
            QFile LogFile(FileChooser.selectedFiles()[0]);
            if (LogFile.open(QIODevice::WriteOnly))
            {
                QTextStream OutStream(&LogFile);
                OutStream << LogText->toPlainText();
                if (OutStream.status() != QTextStream::Ok)
                    ErrorOccurred = true;
            }
            else
            {
                ErrorOccurred = true;
            }
            LogFile.close();

            if (ErrorOccurred)
            {
                QMessageBox ErrorDialog(this);
                ErrorDialog.setIcon(QMessageBox::Critical);
                ErrorDialog.setWindowTitle("Error Saving Log to File");
                ErrorDialog.setText("An error occurred while saving the log "
                    "file. Either the file could not be opened or an error "
                    "occurred while writing.");
                ErrorDialog.exec();
            }
        }
    }
}
