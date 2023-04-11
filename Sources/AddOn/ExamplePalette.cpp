#include "ExamplePalette.h"

#include "ResourceIds.hpp"



ExamplePalette::ExamplePalette(GS::DisposeHandler& disposeHandler)
	: DG::Palette(ACAPI_GetOwnResModule(), ID_EXAMPLE_PALETTE, ACAPI_GetOwnResModule(), ExamplePaletteGuid)
	, listBox(GetReference(), listBoxID)
	, updateButton(GetReference(),updateButtonID)
{
	SetDefaultGarbageCollector();
	SetDisposeHandler(disposeHandler);

	BeginEventProcessing();
	SetClientSize(GetOriginalClientWidth(), GetOriginalClientHeight());

	listBox.Attach(*this);
	updateButton.Attach(*this);

	listBox.SetTabFieldCount(1);
}


ExamplePalette::~ExamplePalette()
{
	DeleteListBoxUserData();

	listBox.Detach(*this);
	updateButton.Detach(*this);

	EndEventProcessing();
}


void ExamplePalette::ResizeItem(short dh, short dv)
{
	listBox.Resize(dh, dv);
	updateButton.Move(dh, dv);
}


void ExamplePalette::ZoomToElement(const API_Guid& guid)
{
	API_Neig neig = {};
	neig.guid = guid;
	ACAPI_Element_DeselectAll();
	ACAPI_Element_Select({ neig }, true);

	GS::Array<API_Guid> guidArray;
	guidArray.Push(guid);
	ACAPI_Automate(APIDo_ZoomToElementsID, &guidArray);
}


void ExamplePalette::ListBoxClicked(const DG::ListBoxClickEvent& ev)
{
	if (ev.GetSource() == &listBox)
	{
		short selecedItem = listBox.GetSelectedItem();
		API_Guid* pGuid = (API_Guid*)listBox.GetItemValue(selecedItem);
		if (pGuid == nullptr)
			return;

		ZoomToElement(*pGuid);
	}
}


void ExamplePalette::ButtonClicked(const DG::ButtonClickEvent& ev)
{
	if (ev.GetSource() == &updateButton)
	{
		UpdateListBox();
	}
}


void ExamplePalette::UpdateListBox()
{
	if (listBox.GetItemCount() > 0) 
	{
		DeleteListBoxUserData();
		listBox.DeleteItem(DG::ListBox::AllItems);
	}

	GSErrCode err = NoError;
	GS::Array<API_Guid> elemGuids;
	err = ACAPI_Element_GetElemList(API_ZombieElemID, &elemGuids, APIFilt_OnVisLayer + APIFilt_OnActFloor);
	if (err != NoError)
		return;

	for (API_Guid guid : elemGuids)
	{
		API_Element elem = {};
		elem.header.guid = guid;

		err = ACAPI_Element_Get(&elem);
		if (err != NoError)
			continue;

		GS::UniString elemNameStr;
		ACAPI_Goodies_GetElemTypeName(elem.header.type.typeID, elemNameStr);

		listBox.AppendItem();
		listBox.SetTabItemText(DG::SingleSelListBox::BottomItem, 1, elemNameStr);

		API_Guid* pGuid = new API_Guid(guid);
		listBox.SetItemValue(DG::SingleSelListBox::BottomItem, (DGUserData)pGuid);
	}
}


void ExamplePalette::DeleteListBoxUserData()
{
	for (short i = 1; i <= listBox.GetItemCount(); i++)
	{
		API_Guid* pGuid = (API_Guid*)listBox.GetItemValue(i);
		if (pGuid != nullptr) {
			delete pGuid;
		}
	}
}


ExamplePaletteController::ExamplePaletteController()
{

}


ExamplePaletteController::~ExamplePaletteController()
{
	ClosePalette();
}

void ExamplePaletteController::TogglePalette()
{
	if (IsPaletteVisible()) {
		HidePalette();
	}
	else {
		OpenPalette();
	}
}

void ExamplePaletteController::OpenPalette()
{
	if (palette == nullptr)
	{
		palette = GS::NewOwned<ExamplePalette>(*this);
		palette->Attach(*this);
	}

	ShowPalette();
}

void ExamplePaletteController::ClosePalette()
{
	if (palette == nullptr) {
		return;
	}

	palette->Detach(*this);
	palette->MarkAsDisposable();
}


GSErrCode ExamplePaletteController::PaletteAPIControlCallBack(Int32 referenceID, API_PaletteMessageID messageID, GS::IntPtr param)
{
	static bool wasVisibleBeforeHide = false;

	if (!(referenceID == ExamplePaletteGuidRefId))
		return NoError;

	ExamplePaletteController& controller = GetInstance();

	switch (messageID) {
	case APIPalMsg_OpenPalette:
		controller.ShowPalette();
		wasVisibleBeforeHide = true;
		break;

	case APIPalMsg_ClosePalette:
		controller.ClosePalette();
		break;

	case APIPalMsg_HidePalette_Begin:
		if (controller.IsPaletteVisible())
		{
			controller.HidePalette();
			wasVisibleBeforeHide = true;
		}
		break;

	case APIPalMsg_HidePalette_End:
		if (wasVisibleBeforeHide) {
			controller.ShowPalette();
			wasVisibleBeforeHide = false;
		}
		break;

	case APIPalMsg_DisableItems_Begin:
		break;

	case APIPalMsg_DisableItems_End:
		break;

	case APIPalMsg_IsPaletteVisible:
		(*reinterpret_cast<bool*> (param)) = controller.IsPaletteVisible();
		break;

	default:
		break;
	}

	return NoError;
}

void ExamplePaletteController::ShowPalette()
{
	if (palette == nullptr) {
		return;
	}

	palette->Show();
}


void ExamplePaletteController::HidePalette()
{
	if (palette == nullptr) {
		return;
	}

	palette->Hide();
}


bool ExamplePaletteController::IsPaletteVisible()
{
	return palette != nullptr && palette->IsVisible();
}


void ExamplePaletteController::PanelResized(const DG::PanelResizeEvent& ev)
{
	short dh = ev.GetHorizontalChange();
	short dv = ev.GetVerticalChange();

	if (dh != 0 || dv != 0) {
		palette->ResizeItem(dh, dv);
	}
}

void ExamplePaletteController::PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted)
{
	UNUSED_VARIABLE(ev);
	UNUSED_VARIABLE(accepted);

	HidePalette();
}

void ExamplePaletteController::DisposeRequested(GS::DisposableObject& source)
{
	if (&source == palette.Get())
		palette.Release();
}
