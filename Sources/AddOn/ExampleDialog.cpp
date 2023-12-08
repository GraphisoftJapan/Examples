#include "ExampleDialog.hpp"

ExampleDialog::ExampleDialog()
    : DG::ModalDialog (ACAPI_GetOwnResModule (), ExampleDialogResourceId, ACAPI_GetOwnResModule ())
    , okButton (GetReference (), OKButtonId)
    , cancelButton (GetReference (), CancelButtonId)
    , separator (GetReference (), SeparatorId)
{
    AttachToAllItems (*this);
    Attach (*this);
}


ExampleDialog::~ExampleDialog ()
{
    Detach (*this);
    DetachFromAllItems (*this);
}


void ExampleDialog::PanelResized (const DG::PanelResizeEvent& ev)
{
    BeginMoveResizeItems ();
    okButton.Move (ev.GetHorizontalChange (), ev.GetVerticalChange ());
    cancelButton.Move (ev.GetHorizontalChange (), ev.GetVerticalChange ());
    separator.MoveAndResize (0, ev.GetVerticalChange (), ev.GetHorizontalChange (), 0);
    EndMoveResizeItems ();
}

void ExampleDialog::ButtonClicked (const DG::ButtonClickEvent& ev)
{
    if (ev.GetSource () == &okButton) {
        PostCloseRequest (DG::ModalDialog::Accept);
    } else if (ev.GetSource () == &cancelButton) {
        PostCloseRequest (DG::ModalDialog::Cancel);
    }
}



// Rolldown Dialog Settings
ExampleRollDownDialog::ExampleRollDownDialog() 
    : UD::RollDownModalDialog("OK", 70, "Cancel", 70, ExampleRollDownDialogGuid)
	, headerPage(nullptr)
	, leftPage(nullptr)
	, middlePage(nullptr)
	, firstPage(nullptr)
	, secondPage(nullptr)
{
	DG::Dialog::Attach(*this);
	Initialize();
}

ExampleRollDownDialog::~ExampleRollDownDialog()
{
	if (headerPage != nullptr)
		delete headerPage;

	if (leftPage != nullptr)
		delete leftPage;
	
	DestroyRollDownPage();
	DG::Dialog::Detach(*this);
}

void ExampleRollDownDialog::Initialize()
{
	UD::IRollDownSurface* rollDownSurface = GetRollDownSurface();
	UD::IRollDownPanelSet* rollDownPanelSet = nullptr;

	bool valid = true;
	valid = (valid && rollDownSurface != nullptr && UD::RollDownModalDialog::IsValid());
	if (valid) {
		SetTitle("Example Rolldown Dialog");
		rollDownPanelSet = rollDownSurface->CreateRollDownPanelSet();
		if (rollDownPanelSet == nullptr)
			valid = false;
	}

	rollDownSurface->SetLayoutMethod(UD::IRollDownSurface::LayoutMethod::SizeableLayout);
	SetGrowType(NoGrow);

	// -----------------------------------------------------------------------------
	// Header Tab Page
	// -----------------------------------------------------------------------------
	if (valid)
	{
		UD::IPanel* headerPanel = rollDownSurface->CreateHeaderPanel();
		if (headerPanel != nullptr) 
		{
			try {
				headerPage = new HeaderPage(headerPanel->GetTabControl());
			}
			catch (...) {
				DBBREAK();
			}
			if (headerPage == nullptr || !headerPage->IsValid())
				valid = false;
		}
		else {
			valid = false;
		}
	}

	// -----------------------------------------------------------------------------
	// Left Roll Down
	// -----------------------------------------------------------------------------
	if (valid)
	{
		UD::IRollPanel* leftPanel = rollDownSurface->CreateRollLeftPanel();
		if (leftPanel != nullptr)
		{
			leftPanel->SetGrowType(UD::IRollPanel::NoGrow);
			try {
				leftPage = new LeftPage(leftPanel->GetTabControl());
			}
			catch (...) {
				DBBREAK();
			}

			if (leftPage == nullptr || !leftPage->IsValid()) {
				valid = false;
			}
		}
		else {
			valid = false;
		}
	}

	// -----------------------------------------------------------------------------
	// Middle Roll Down
	// -----------------------------------------------------------------------------
	if (valid)
	{
		UD::IMiddlePanel* middlePanel = rollDownSurface->CreateMiddlePanel();
		if (middlePanel != nullptr)
		{
			middlePanel->SetGrowType(UD::IMiddlePanel::NoGrow);
			try {
				middlePage = new MiddlePage(middlePanel->GetTabControl());
			}
			catch (...) {
				DBBREAK();
			}

			if (middlePage == nullptr || !middlePage->IsValid()) {
				valid = false;
			}
		}
		else {
			valid = false;
		}
	}

	// -----------------------------------------------------------------------------
	// First Roll Down
	// -----------------------------------------------------------------------------

	if (valid)
	{
		UD::IRollPanel* firstRollDownPanel = rollDownPanelSet->CreateRollDownPanel();

		if (firstRollDownPanel != nullptr)
		{
			firstRollDownPanel->SetIcon(DG::Icon(InvalidResModule, DG_NO_ICON));
			firstRollDownPanel->SetGrowType(UD::IRollPanel::HGrow);
			firstRollDownPanel->SetTitle("First Rolldown");
			
			try {
				firstPage = new FirstPage(firstRollDownPanel->GetTabControl());
			}
			catch (...) {
				DBBREAK();
			}

			if (firstPage == nullptr || !firstPage->IsValid()) {
				valid = false;
			}
		}
		else {
			valid = false;
		}
	}


	// -----------------------------------------------------------------------------
	// Second Roll Down
	// -----------------------------------------------------------------------------
	if (valid)
	{
		UD::IRollPanel* secondRollDownPanel = rollDownPanelSet->CreateRollDownPanel();
		if (secondRollDownPanel != nullptr)
		{
			secondRollDownPanel->SetIcon(DG::Icon(InvalidResModule, DG_NO_ICON));
			secondRollDownPanel->SetGrowType(UD::IRollPanel::HVGrow);
			secondRollDownPanel->SetTitle("Second Rolldown");

			try {
				secondPage = new SecondPage(secondRollDownPanel->GetTabControl());
			}
			catch (...) {
				DBBREAK();
			}

			if (secondPage == nullptr || !secondPage->IsValid()) {
				valid = false;
			}

		}
		else {
			valid = false;
		}
	}
}

void ExampleRollDownDialog::DestroyPage(const UD::IPanel& panel)
{
    DG::TabPage* tabPage = panel.GetTabControl().GetTabPage(1);
    if (DBVERIFY(tabPage != nullptr))
        delete tabPage;
}

void ExampleRollDownDialog::DestroyRollDownPage()
{
	UD::IRollDownSurface* rollDownSurface = GetRollDownSurface();

	if (rollDownSurface != nullptr) {
		for (UD::IRollDownSurface::ConstIterator its = rollDownSurface->EnumerateRollDownPanelSets(); its != nullptr; ++its) {
			for (UD::IRollDownPanelSet::ConstIterator itp = (*its)->EnumerateRollDownPanels(); itp != nullptr; ++itp)
			{
				(**itp).UnregisterObserver(*this);
				DestroyPage(**itp);
			}
		}
	}
}



// header page
HeaderPage::HeaderPage(const DG::TabControl& tabControl)
	: DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), ID_ROLLDOWN_HEADERPAGE, ACAPI_GetOwnResModule())
	, leftText(GetReference(), leftTextID)
{

}

HeaderPage::~HeaderPage()
{

}



// header page
LeftPage::LeftPage(const DG::TabControl& tabControl)
	: DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), ID_ROLLDOWN_LEFTPAGE, ACAPI_GetOwnResModule())
	, leftText(GetReference(), leftTextID)
{

}

LeftPage::~LeftPage()
{

}



// middle page
MiddlePage::MiddlePage(const DG::TabControl& tabControl)
	: DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), ID_ROLLDOWN_MIDDLEPAGE, ACAPI_GetOwnResModule())
	, leftText(GetReference(), leftTextID)
{

}

MiddlePage::~MiddlePage()
{

}



// first page
FirstPage::FirstPage(const DG::TabControl& tabControl)
	: DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), ID_ROLLDOWN_FIRSTPAGE, ACAPI_GetOwnResModule())
	, leftText(GetReference(), leftTextID)
	, listBox(GetReference(), listBoxID)
{
	listBox.Attach(*this);

	// initialize listbox
	listBox.SetTabFieldCount(1);
	listBox.SetTabFieldProperties(1, 0, listBox.GetWidth(), DG::ListBox::Left, DG::ListBox::NoTruncate);

	// add item
	listBox.AppendItem();
	listBox.SetTabItemText(-1, 1, "Test Item");

}

FirstPage::~FirstPage()
{
	listBox.Detach(*this);
}

void FirstPage::CreateMenuItemRowArray(short listItem, const DG::ListBox& source, UD::ItemInfoTag::InfoTagContentCreator::MenuItemRowArray& out) const
{
	if (source == listBox)
	{
		{
			FloatingWindow::FloatingMenuItemPtr menuItem = new UD::ItemInfoTag::InfoTagContentCreator::InfoTagItem;
			GS::UniString tooltip1("Tool tip text - Bold");

			menuItem->SetText(tooltip1);
			menuItem->SetTextStyle(DG::Font::Bold);
			out.Push(new FloatingWindow::FloatingMenu::MenuItemRow(menuItem));
		}

		{
			FloatingWindow::FloatingMenuItemPtr menuItem = new UD::ItemInfoTag::InfoTagContentCreator::InfoTagItem;
			GS::UniString tooltip2("Tool tip text - Color");

			menuItem->SetText(tooltip2);
			menuItem->SetTextColor(new Gfx::Color(Gfx::Color::Red));
			out.Push(new FloatingWindow::FloatingMenu::MenuItemRow(menuItem));
		}
	}
}

// second page
SecondPage::SecondPage(const DG::TabControl& tabControl)
	: DG::TabPage(tabControl, 1, ACAPI_GetOwnResModule(), ID_ROLLDOWN_SECONDPAGE, ACAPI_GetOwnResModule())
	, leftText(GetReference(), leftTextID)
{

}

SecondPage::~SecondPage()
{

}
