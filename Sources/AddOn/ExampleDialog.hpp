#pragma once

#include "APIEnvir.h"
#include "ACAPinc.h"

#include "DG.h"
#include "DGModule.hpp"

#include "ResourceIds.hpp"


class ExampleDialog :	public DG::ModalDialog,
						public DG::PanelObserver,
						public DG::ButtonItemObserver,
						public DG::CompoundItemObserver
{
public:
	enum DialogResourceIds
	{
		ExampleDialogResourceId = ID_ADDON_DLG,
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
