#pragma once

#include "APIEnvir.h"
#include "ACAPinc.h"

#include "DG.h"
#include "DGModule.hpp"

#include "DisposableObject.hpp"
#include "DisposeHandler.hpp"
#include "Singleton.hpp"


const GS::Guid ExamplePaletteGuid{ "C67E8137-A2AC-42FD-BDE6-413F509731B8" };
const Int32    ExamplePaletteGuidRefId = (Int32)GS::CalculateHashValue(ExamplePaletteGuid);

class ExamplePalette
	: public DG::Palette
	, public DG::ListBoxObserver
	, public DG::ButtonItemObserver
	, public GS::DisposableObject
{
public:
	ExamplePalette(GS::DisposeHandler& disposeHandler);
	virtual ~ExamplePalette();

	void ResizeItem(short dh, short dv);
	void ZoomToElement(const API_Guid& guid);

protected:
	virtual void ListBoxClicked(const DG::ListBoxClickEvent& ev) override;
	virtual void ButtonClicked(const DG::ButtonClickEvent& ev) override;

private:
	void UpdateListBox();
	void DeleteListBoxUserData();

private:
	enum
	{
		listBoxID = 1,
		updateButtonID,
	};

	DG::SingleSelListBox listBox;
	DG::Button			 updateButton;
};



class ExamplePaletteController
	: public GS::Singleton<ExamplePaletteController>
	, public DG::PanelObserver
	, public GS::DisposeHandler
{
	friend LazyInstantiationPolicy;

public:
	virtual ~ExamplePaletteController();
	void TogglePalette();
	void ClosePalette();

	static GSErrCode PaletteAPIControlCallBack(Int32 referenceID, API_PaletteMessageID messageID, GS::IntPtr param);


private:
	ExamplePaletteController();
	GS::Owner<ExamplePalette> palette;

	void OpenPalette();
	void ShowPalette();
	void HidePalette();
	bool IsPaletteVisible();

	virtual void PanelResized(const DG::PanelResizeEvent& ev) override;
	virtual	void PanelCloseRequested(const DG::PanelCloseRequestEvent& ev, bool* accepted) override;
	virtual void DisposeRequested(GS::DisposableObject& source) override;

};
