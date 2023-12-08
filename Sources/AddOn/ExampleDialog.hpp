#pragma once

#include "APIEnvir.h"
#include "ACAPinc.h"

#include "DG.h"
#include "DGModule.hpp"

#include "UDRollDownDialogs.hpp"
#include "UDRollPanelObserver.hpp"
#include "UDImprovedListBox.hpp"

#include "ResourceIds.hpp"


// Modal dialog example
class ExampleDialog :	public DG::ModalDialog,
						public DG::PanelObserver,
						public DG::ButtonItemObserver,
						public DG::CompoundItemObserver
{
public:
	enum DialogResourceIds
	{
		ExampleDialogResourceId = ID_EXAMPLE_MODAL_DLG,
		OKButtonId = 1,
		CancelButtonId = 2,
		SeparatorId = 3
	};

	ExampleDialog ();
	~ExampleDialog ();

private:
	virtual void PanelResized(const DG::PanelResizeEvent& ev) override;
	virtual void ButtonClicked(const DG::ButtonClickEvent& ev) override;

	DG::Button		okButton;
	DG::Button		cancelButton;
	DG::Separator	separator;
};



const GS::Guid ExampleRollDownDialogGuid { "D87B1DDD-4093-4C9D-8C13-BC84E4EEAEFC" };

class HeaderPage;
class LeftPage;
class MiddlePage;
class FirstPage;
class SecondPage;


// Roll down dialog example
class ExampleRollDownDialog
	: public UD::RollDownModalDialog
	, public UD::RollPanelObserver
{
public:
	HeaderPage* headerPage;
	LeftPage* leftPage;
	MiddlePage* middlePage;
	FirstPage* firstPage;
	SecondPage* secondPage;

public:
	ExampleRollDownDialog();
	~ExampleRollDownDialog();

private:
	void Initialize();
	void DestroyPage(const UD::IPanel& panel);
	void DestroyRollDownPage();


};

// header page
class HeaderPage
	: public DG::TabPage
{
public:
	enum 
	{
		leftTextID = 1,
	};

	DG::LeftText leftText;

public:
	HeaderPage(const DG::TabControl& tabControl);
	~HeaderPage();
};

// left page
class LeftPage
	: public DG::TabPage
{
public:
	enum
	{
		leftTextID = 1,
	};

	DG::LeftText leftText;

public:
	LeftPage(const DG::TabControl& tabControl);
	~LeftPage();
};

// middle page
class MiddlePage
	: public DG::TabPage
{
public:
	enum
	{
		leftTextID = 1,
	};

	DG::LeftText leftText;

public:
	MiddlePage(const DG::TabControl& tabControl);
	~MiddlePage();
};

// first roll down page
class FirstPage
	: public DG::TabPage
	, public UD::ImprovedListBoxObserver
{
public:
	enum
	{
		leftTextID = 1,
		listBoxID = 2,
	};

	DG::LeftText leftText;
	UD::ImprovedSingleSelListBox listBox;


public:
	FirstPage(const DG::TabControl& tabControl);
	~FirstPage();

	virtual void CreateMenuItemRowArray(short listItem, const DG::ListBox& source, UD::ItemInfoTag::InfoTagContentCreator::MenuItemRowArray& out) const;
};

// second roll down page
class SecondPage
	: public DG::TabPage
{
public:
	enum
	{
		leftTextID = 1,
	};

	DG::LeftText leftText;

public:
	SecondPage(const DG::TabControl& tabControl);
	~SecondPage();
};