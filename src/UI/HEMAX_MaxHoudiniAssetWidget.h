#pragma once

#include "HEMAX_ParameterWidget.h"

#include "../HEMAX_Types.h"

class HEMAX_3dsmaxHda;
class HEMAX_Plugin;

class HEMAX_MaxHoudiniAssetWidget : public HEMAX_ParameterWidget
{
    Q_OBJECT

    public:

	HEMAX_MaxHoudiniAssetWidget(HEMAX_Plugin* ActivePlugin);
	virtual ~HEMAX_MaxHoudiniAssetWidget() = default;

	HEMAX_3dsmaxHda* GetCurrentHdaSelection();

    private:
        
        HEMAX_Plugin* Plugin;
	HEMAX_3dsmaxHda* Selection;
	void UpdateWidget();
	void PushSubnetworkInputNames();
	void PushParameterInputNames();

    private:

	QWidget* HdaActionButtons;
	QGridLayout* HdaActionButtonsLayout;

	QPushButton* RecookHdaButton;
	QPushButton* ReloadHdaButton;

	QPushButton* BakeHdaButton;
	QPushButton* CloneHdaButton;
	QPushButton* CopyToNodeButton;

	// Advanced Options Box //

	QGroupBox* AdvancedOptionsBox;
	QGridLayout* AdvancedOptionsLayout;
	QCheckBox* AdvancedOptionsEnabledCheckbox;

	QWidget* AdvancedOptions_Widget;
	QGridLayout* AdvancedOptions_WidgetLayout;
	QLabel* AdvancedOptions_HDAPathLabel;
	QLineEdit* AdvancedOptions_HDAPath;
	QPushButton* AdvancedOptions_HDAPathSave;

	//////////////////////////

	void CreateAdvancedOptionsUI();

	void UpdateAdvancedOptionsUI();
	void HideHdaActionButtonsUI();
	void UpdateHdaActionButtonsUI(HEMAX_HdaType HdaType);

    private slots:

	void Slot_BakeHdaButton();
	void Slot_CloneHdaButton();
	void Slot_CopyToNodeButton();

	void Slot_AdvancedOptionsCheckbox(int State);
	void Slot_AdvancedOptionsHdaPathSave();
};
