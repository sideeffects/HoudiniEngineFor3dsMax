#pragma once

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

class HEMAX_3dsmaxHda;
class HEMAX_ParameterWidget;

class QHBoxLayout;
class QImage;
class QLabel;
class QPushButton;
class QToolButton;
class QVBoxLayout;

class HEMAX_HDAWidget : public QWidget
{
    Q_OBJECT

public:
                                    HEMAX_HDAWidget();

    void                            Update();
    void                            SetSelection(HEMAX_3dsmaxHda* SelectedHda);

private:

    HEMAX_3dsmaxHda*                Selection                       = nullptr;

    bool                            Locked                          = false;

    void                            UpdateSessionStatusWidget();
    void                            UpdateSelectionWidget();

    bool                            LoadHoudiniEngineLogo();

private:

    QVBoxLayout*                    HDAWidgetLayout                 = nullptr;

    QLabel*                         HoudiniEngineBannerLabel        = nullptr;
    QImage*                         HoudiniEngineLogoImage          = nullptr;

    QWidget*                        SessionStatusWidget             = nullptr;
    QHBoxLayout*                    SessionStatusWidgetLayout       = nullptr;
    QLabel*                         SessionStatusLabel              = nullptr;

    QWidget*                        SelectionWidget                 = nullptr;
    QHBoxLayout*                    SelectionWidgetLayout           = nullptr;
    QLabel*                         CurrentSelectionLabel           = nullptr;
    QPushButton*                    LockSelectionButton             = nullptr;

    QWidget*                        CookControlsWidget              = nullptr;
    QVBoxLayout*                    CookControlsWidgetLayout        = nullptr;
    QToolButton*                    CookControlsHeader              = nullptr;
    QWidget*                        CookControlsCollapsibleWidget   = nullptr;
    QHBoxLayout*                    CookControlsContentLayout       = nullptr;
    QPushButton*                    RecookButton                    = nullptr;
    QPushButton*                    RebuildButton                   = nullptr;
    QPushButton*                    ResetParametersButton           = nullptr;

    QWidget*                        BakeControlsWidget              = nullptr;
    QVBoxLayout*                    BakeControlsWidgetLayout        = nullptr;
    QToolButton*                    BakeControlsHeader              = nullptr;
    QWidget*                        BakeControlsCollapsibleWidget   = nullptr;
    QHBoxLayout*                    BakeControlsContentLayout       = nullptr;
    QPushButton*                    BakeButton                      = nullptr;

    QWidget*                        CloneControlsWidget             = nullptr;
    QVBoxLayout*                    CloneControlsWidgetLayout       = nullptr;
    QToolButton*                    CloneControlsHeader             = nullptr;
    QWidget*                        CloneControlsCollapsibleWidget  = nullptr;
    QHBoxLayout*                    CloneControlsContentLayout      = nullptr;
    QPushButton*                    CloneButton                     = nullptr;

    QWidget*                        ParametersWidget                = nullptr;
    QVBoxLayout*                    ParametersWidgetLayout          = nullptr;
    QToolButton*                    ParametersHeader                = nullptr;
    HEMAX_ParameterWidget*          ParametersContentWidget         = nullptr;

private slots:

    void                            CookControlsHeaderClickedSlot();
    void                            BakeControlsHeaderClickedSlot();
    void                            CloneControlsHeaderClickedSlot();
    void                            ParametersHeaderClickedSlot();

};
