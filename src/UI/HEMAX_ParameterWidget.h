#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022)
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qcolordialog.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qslider.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qcolordialog.h>
#endif

#include "../HEMAX_Node.h"

#include <string>
#include <vector>

#define HEMAX_MULTIPARAM_MAX_BUTTON_WIDTH 20
#define HEMAX_SPACER_ITEM_WIDTH 40

class HEMAX_ParameterWidget_Parameter : public QWidget
{
    public:
	HEMAX_ParameterWidget_Parameter(int ParameterId);
	virtual void MarkInvalid();
	virtual void SetHelpToolTip(std::string HelpString);
	bool IsUIInvalid();
	int GetParameterId();

    protected:
	int ParameterId;
	bool InvalidUI;
};

class HEMAX_ParameterWidget : public QWidget
{
    Q_OBJECT

    public:
	HEMAX_ParameterWidget();
	~HEMAX_ParameterWidget();

	void SelectHDA(HEMAX_Node* TheSelectedNode);
	void RefreshUI(bool DeleteLater = true);

	HEMAX_Node* GetCurrentSelectedNode();

    protected:
	QVBoxLayout* MainBoxLayout;

	bool SelectionLocked;

	void DisableSubnetworkInputUI(int Subnetwork);

        void SetSelectionLocked(bool Locked);

	std::vector<QWidget*> SubnetworkInputs;
	std::vector<QWidget*> ParameterWidgets;

	std::vector<QWidget*> OpParmWidgets;

    private:

	void UpdateParameterUI(bool ScheduleDeleteLater);

	QVBoxLayout* MainLayout;

	QWidget* MainBox;

	QGroupBox* NodeInputBox;
	QGridLayout* NodeInputBoxLayout;

	QGroupBox* ParametersBox;
	QGridLayout* ParametersBoxLayout;
	QLabel* ParametersSelectedAssetLabel;
	QLabel* ParametersSelectedAssetName;
	QPushButton* ParametersSelectionLockedButton;

	QGroupBox* NodeOptionsBox;
	QGridLayout* NodeOptionsBoxLayout;
	QCheckBox* NodeOptions_AutoRecook;
	QCheckBox* NodeOptions_RealtimeRecook;
	QCheckBox* NodeOptions_InputUpdate;

	QWidget* ParametersDetailBox;
	QGridLayout* ParametersDetailGridLayout;
	std::vector<QHBoxLayout*> ParametersDetailRows;

	HEMAX_Node* CurrentNode;

	std::vector<std::string> LastActiveFolderTabs;

	void AddWidgetToNewRow(QWidget* TheWidget);
	void AppendWidgetToPreviousRow(QWidget* TheWidget);

	HEMAX_ParameterWidget_Parameter* CreateParameterWidget_Color(HEMAX_Parameter& Parameter, std::string Label);

	int CurrentRow;

    protected slots:

	virtual void Slot_LockSelectionButton_Clicked();

    private slots:

	void Slot_NodeOptions_AutoRecook_StateChanged(int State);
	void Slot_NodeOptions_RealtimeRecook_StateChanged(int State);
	void Slot_NodeOptions_InputUpdate_StateChanged(int State);

	void Slot_PWINTEGER_EditingFinished();
	void Slot_PWINTEGER_SliderDone();
	void Slot_PWINTEGER_SliderDrag();
	void Slot_PWINTEGERCHOICE_Selection(int Index);

	void Slot_PWSTRING_EditingFinished();
	void Slot_PWSTRINGCHOICE_Selection(int Index);

	void Slot_PWFLOAT_EditingFinished();
	void Slot_PWFLOAT_SliderDone();
	void Slot_PWFLOAT_SliderDrag();

	void Slot_PWTOGGLE_StateChanged();

	void Slot_PWCOLOR_Update();

	void Slot_PWBUTTON_Clicked();
	void Slot_PWBUTTONCHOICE_Selection(int Index);

	void Slot_PWFILEPATH_Updated();
	void Slot_PWFILEPATH_Selected();

	void Slot_PWNODE_InputSelection();
	void Slot_PWNODE_ClearSelection();
	void Slot_PWNODE_SubnetworkInputSelection();
	void Slot_PWNODE_SubnetworkClearSelection();

	void Slot_PWMULTIPARAMETER_Add(int Position, bool DoNotCook);
	void Slot_PWMULTIPARAMETER_Remove(int Position, bool DoNotCook);

	void Slot_PWFOLDERLIST_FolderChanged(int Index);

    signals:

        void Signal_CookNode(HEMAX_Node *);
	void Signal_ReloadAssetDefinition(HEMAX_Node *);
	void Signal_InputSelection(HEMAX_Node*, HEMAX_Parameter, bool);
	void Signal_SubnetworkInputSelection(HEMAX_Node*, int, bool);
	void Signal_UpdateParameterIntValues(HEMAX_Node*, HEMAX_Parameter, std::vector<int>, bool);
	void Signal_UpdateParameterFloatValues(HEMAX_Node*, HEMAX_Parameter, std::vector<float>, bool);
	void Signal_UpdateParameterStringValues(HEMAX_Node*, HEMAX_Parameter, std::vector<std::string>);
	void Signal_UpdateMultiParameterList(HEMAX_Node*, HEMAX_Parameter, HEMAX_MultiParameterChangeInfo);

};

class HEMAX_ParameterWidget_Folder : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    public:
	HEMAX_ParameterWidget_Folder(int ParameterId, std::string FolderName);
	~HEMAX_ParameterWidget_Folder();

	std::string GetFolderName();
	void AddWidget(QWidget* Widget);
	void AddWidgetToPreviousRow(QWidget* Widget);

	void MarkInvalid() override;
	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* MainLayout;

	std::vector<QWidget*> ParameterWidgets;
	std::vector<QHBoxLayout*> Rows;

	std::string FolderName;

	int CurrentRow;
	int CurrentColumn;
};

class HEMAX_ParameterWidget_Folderlist : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Folderlist(int ParameterId);
	~HEMAX_ParameterWidget_Folderlist();

	void AppendFolder(HEMAX_ParameterWidget_Folder* Folder);
	HEMAX_ParameterWidget_Folder* RetrieveFolder(int FolderIndex);
	void BuildDirectory();

	void MarkInvalid() override;
	void SetHelpToolTip(std::string HelpString) override;

    private:

	QVBoxLayout* Layout;
	QTabWidget* FolderContainer;

	int CurrentlyActiveIndex;

	std::vector<HEMAX_ParameterWidget_Folder*> Folders;
};

class HEMAX_ParameterWidget_Label : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Label(int ParameterId, std::string LabelText);
	~HEMAX_ParameterWidget_Label();

	void SetHelpToolTip(std::string HelpString) override;
    private:
	QGridLayout* Layout;
	QLabel* Label;
};

class HEMAX_ParameterWidget_Integer : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Integer(int ParameterId, std::string Label, std::vector<int> Values, int ParamSize);
	HEMAX_ParameterWidget_Integer(int ParameterId, std::string Label, std::vector<int> Values, int ParamSize, int UIMin, int UIMax);
	~HEMAX_ParameterWidget_Integer();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* Layout;
	QLabel* ParameterLabel;
	std::vector<QLineEdit*> IntegerValues;
	QSlider* IntegerSlider;

	int CalculateSliderPosition();
	void UpdateSliderPosition();
	int CalculateValueFromSliderPosition();

    private slots:

	void Slot_Slider(int Value);
	void Slot_SliderReleased();

    signals:

	void Signal_Integer_SliderDone();
	void Signal_Integer_SliderDrag();

};

class HEMAX_ParameterWidget_Integer_Choice : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Integer_Choice(int ParameterId, std::string Label, int ChoiceSize, std::vector<std::string> Choices, int CurrentChoiceIndex);
	~HEMAX_ParameterWidget_Integer_Choice();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* Layout;
	QLabel* ParameterLabel;
	QComboBox* IntegerChoiceValues;

};

class HEMAX_ParameterWidget_String : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_String(int ParameterId, std::string Label, std::vector<std::string> Values, int ParamSize);
	~HEMAX_ParameterWidget_String();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* Layout;
	QLabel* ParameterLabel;
	std::vector<QLineEdit*> StringValues;

};

class HEMAX_ParameterWidget_String_Choice : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_String_Choice(int ParameterId, std::string ParameterLabel, std::vector<std::string> Choices, int CurrentChoiceIndex);
	~HEMAX_ParameterWidget_String_Choice();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* Layout;
	QLabel* ParameterLabel;
	QComboBox* StringChoiceValues;

};

class HEMAX_ParameterWidget_Float : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Float(int ParameterId, std::string Label, std::vector<float> Values, int ParamSize);
	HEMAX_ParameterWidget_Float(int ParameterId, std::string Label, std::vector<float> Values, int ParamSize, float UIMin, float UIMax);
	~HEMAX_ParameterWidget_Float();

	void SetHelpToolTip(std::string HelpString) override;

        static std::string FloatToString(float Val);

    private:

	QGridLayout* Layout;
	QLabel* ParameterLabel;
	std::vector<QLineEdit*> FloatValues;
	QSlider* FloatSlider;

	int CalculateSliderPosition();
	void UpdateSliderPosition();
	float CalculateFloatValueFromSliderPosition();
	float CalculateFloatValueFromSliderPosition(int Position);

	float UIMin;
	float UIMax;

    private slots:

	void Slot_Slider(int Value);
	void Slot_SliderReleased();

    signals:

	void Signal_Float_SliderDrag();
	void Signal_Float_SliderDone();

};

class HEMAX_ParameterWidget_Toggle : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Toggle(int ParameterId, std::string Label, bool Checked);
	~HEMAX_ParameterWidget_Toggle();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QHBoxLayout* Layout;
	QCheckBox* ToggleValue;
	QLabel* ParameterLabel;
	QSpacerItem* Spacer;

};

class HEMAX_ParameterWidget_Button : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Button(int ParameterId, std::string Label);
	~HEMAX_ParameterWidget_Button();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* Layout;
	QPushButton* Button;

};

class HEMAX_ParameterWidget_Button_Choice : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Button_Choice(int ParameterId, HEMAX_Parameter* Parameter, std::string Title, std::vector<HEMAX_ParmChoice> Choices);
	~HEMAX_ParameterWidget_Button_Choice();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* Layout;
	std::string ChoiceTitle;
	QComboBox* ChoiceValues;

	HEMAX_Parameter* Parameter;

};

class HEMAX_ParameterWidget_FilePath : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_FilePath(int ParameterId, std::string Label,
		std::string CurrentPathValue, HAPI_ParmType ParmType,
                HAPI_Permissions BrowseMode);
        HEMAX_ParameterWidget_FilePath(int ParameterId, std::string Label,
                std::string CurrentPathValue, HAPI_ParmType ParmType,
                std::vector<HEMAX_ParmChoice> Choices,
                HAPI_Permissions BrowseMode);
	~HEMAX_ParameterWidget_FilePath();

	void SetHelpToolTip(std::string HelpString) override;

    private:

	QHBoxLayout* Layout;
	QLabel* ParameterLabel;
	QLineEdit* PathEdit;
	QPushButton* BrowseButton;
        QComboBox* ChoiceValues;

	HAPI_ParmType Type;
        HAPI_Permissions MyBrowseMode;

    private slots:

	void Slot_BrowseButton_Clicked();
        void Slot_FileChoiceSelection(int);

    signals:

	void Signal_FilePath_Selected();

};

class HEMAX_ParameterWidget_Node : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Node(int ParameterId, std::string Label, std::string InputNodeName);
	~HEMAX_ParameterWidget_Node();

	void SetHelpToolTip(std::string HelpString) override;
	void SetInputName(std::string Name);

    private:

	QHBoxLayout* Layout;
	QLabel* ParameterLabel;
	QLineEdit* NodeEdit;
	QPushButton* SelectButton;
	QPushButton* ClearButton;

};

class HEMAX_ParameterWidget_Color : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_Color(int ParameterId, std::string Label, std::vector<float> ColorVals, int Size);
	~HEMAX_ParameterWidget_Color();

	void SetHelpToolTip(std::string HelpString) override;

    private:
	QGridLayout * Layout;
	QLabel* ParameterLabel;
	QPushButton* ColorButton;
	QColorDialog* ColorDialog;

    private slots:
	void Slot_OpenColorPickerDialog();

    signals:
	void Signal_ColorChosen();
};

class HEMAX_ParameterWidget_MultiParameter_Instance : public HEMAX_ParameterWidget_Parameter
{

    Q_OBJECT

    friend class HEMAX_ParameterWidget_MultiParameter;

    public:

	HEMAX_ParameterWidget_MultiParameter_Instance(int InstancePosition);
	~HEMAX_ParameterWidget_MultiParameter_Instance();

	void AddParameter(QWidget* Parameter);

	int InstancePosition;

	void MarkInvalid() override;
	void SetHelpToolTip(std::string HelpString) override;

    private:

	QGridLayout* Layout;

	QPushButton* AddBefore;
	QPushButton* Remove;

	std::vector<QWidget*> Parameters;

	int CurrentGridLayoutRow;

};

class HEMAX_ParameterWidget_MultiParameter : public HEMAX_ParameterWidget_Parameter
{
    Q_OBJECT

    friend class HEMAX_ParameterWidget;

    public:
	HEMAX_ParameterWidget_MultiParameter(int ParameterId, std::string Label, int InstanceCount);
	~HEMAX_ParameterWidget_MultiParameter();

	void AddParameterToInstance(QWidget* InstanceParameter, int Instance);

	void MarkInvalid() override;
	void SetHelpToolTip(std::string HelpString) override;

    private:

	int InstanceCount;

	std::vector<HEMAX_ParameterWidget_MultiParameter_Instance*> Instances;

	QVBoxLayout* Layout;

	QWidget* ControlWidget;
	QHBoxLayout* ControlLayout;
	QLabel* ParameterLabel;
	QLineEdit* InstanceParameterCount;
	QPushButton* ClearButton;
	QPushButton* AddChildParameterButton;
	QPushButton* RemoveChildParameterButton;

    private slots:

	void Slot_ChildParameterCount_returnPressed();
	void Slot_AddChildParameterButton_Clicked();
	void Slot_RemoveChildParameterButton_Clicked();
	void Slot_ClearButton_Clicked();

	void Slot_AddBefore_Clicked();
	void Slot_Remove_Clicked();

    signals:

	void AddMultiParameterInstance(int, bool);
	void RemoveMultiParameterInstance(int, bool);

};
