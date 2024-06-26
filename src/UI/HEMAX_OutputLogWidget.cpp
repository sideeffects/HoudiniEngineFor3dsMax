#include "HEMAX_OutputLogWidget.h"

#include "moc_HEMAX_OutputLogWidget.cpp"

#include <functional>

HEMAX_OutputLogWidget::HEMAX_OutputLogWidget()
{
    Layout = new QVBoxLayout;

    LogText = new QTextEdit;
    LogText->setReadOnly(true);

    ClearButton = new QPushButton("Clear");

    Layout->addWidget(LogText);
    Layout->addWidget(ClearButton);

    this->setLayout(Layout); 

    HEMAX_Logger::Instance().RegisterOnEntryAddedCallback(
        std::bind(&HEMAX_OutputLogWidget::LogEntryAdded,
            this, std::placeholders::_1));

    QObject::connect(ClearButton, SIGNAL(clicked()), this, SLOT(ClearLog()));
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
    LogText->append(Entry.Text.c_str());
}

void
HEMAX_OutputLogWidget::ClearLog()
{
    LogText->clear();
}
