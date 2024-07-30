#pragma once

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include <vector>

#include "../HEMAX_Parameter.h"

class HEMAX_3dsmaxHda;
class HEMAX_Node;
class HEMAX_ParameterWidget;
class HEMAX_Plugin;

class QFrame;
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
                                    HEMAX_HDAWidget(HEMAX_Plugin* ThePlugin);

    void                            Update();

    HEMAX_3dsmaxHda*                GetSelection() { return Selection; }
    void                            SetSelection(HEMAX_3dsmaxHda* SelectedHda,
                                                 bool ForceUnlock = false);

    void                            UpdateParameters(
                                        bool DeleteWidgetsLater = true);

    void                            SetLocked(bool IsLocked)
                                    {
                                        Locked = IsLocked;
                                    }

private:

    HEMAX_3dsmaxHda*                Selection                       = nullptr;
    HEMAX_Plugin*                   Plugin                          = nullptr;

    bool                            Locked                          = false;

    void                            UpdateSessionStatusWidget();
    void                            UpdateSelectionWidget();

    bool                            LoadHoudiniEngineLogo();

private:

    QVBoxLayout*                    HDAWidgetLayout                 = nullptr;

    QLabel*                         HoudiniEngineBannerLabel        = nullptr;
    QImage*                         HoudiniEngineLogoImage          = nullptr;
    QFrame*                         HoudiniEngineLogoSeparator      = nullptr;

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
    QPushButton*                    CopyToObjectButton              = nullptr;

    QWidget*                        ParametersWidget                = nullptr;
    QVBoxLayout*                    ParametersWidgetLayout          = nullptr;
    QToolButton*                    ParametersHeader                = nullptr;
    HEMAX_ParameterWidget*          ParametersContentWidget         = nullptr;

private slots:

    void                            CookControlsHeaderClickedSlot();
    void                            BakeControlsHeaderClickedSlot();
    void                            CloneControlsHeaderClickedSlot();
    void                            ParametersHeaderClickedSlot();

    void                            RecookButtonClickedSlot();
    void                            RebuildButtonClickedSlot();
    void                            ResetParametersButtonClickedSlot();

    void                            LockSelectionButtonClickedSlot();

    void                            CookNodeSlot(HEMAX_Node*);
    void                            InputSelectionSlot(
                                        HEMAX_Node*, HEMAX_Parameter, bool);
    void                            SubnetworkInputSelectionSlot(
                                        HEMAX_Node*, int, bool);
    void                            UpdateParameterIntValuesSlot(
                                        HEMAX_Node*, HEMAX_Parameter,
                                        std::vector<int>, bool);
    void                            UpdateParameterFloatValuesSlot(
                                        HEMAX_Node*, HEMAX_Parameter,
                                        std::vector<float>, bool);
    void                            UpdateParameterStringValuesSlot(
                                        HEMAX_Node*, HEMAX_Parameter,
                                        std::vector<std::string>);
    void                            UpdateMultiParameterListSlot(
                                        HEMAX_Node*, HEMAX_Parameter,
                                        HEMAX_MultiParameterChangeInfo);

};
