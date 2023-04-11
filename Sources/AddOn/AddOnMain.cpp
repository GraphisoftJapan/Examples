#include "APIEnvir.h"
#include "ACAPinc.h"

#include "ResourceIds.hpp"

#include "ExampleDialog.hpp"
#include "ExamplePalette.h"



static const GSResID AddOnInfoID			= ID_ADDON_INFO;
	static const Int32 AddOnNameID			= 1;
	static const Int32 AddOnDescriptionID	= 2;

static const short AddOnMenuID				= ID_ADDON_MENU;
	

static GSErrCode MenuCommandHandler (const API_MenuParams *menuParams)
{
	switch (menuParams->menuItemRef.menuResID) {
		case AddOnMenuID:
			switch (menuParams->menuItemRef.itemIndex) {
				case 1:
				{
					ExampleDialog dlg;
					dlg.Invoke();
				}
					break;
				case 2:
				{
					ExamplePaletteController::GetInstance().TogglePalette();
				}
			}
			break;
	}
	return NoError;
}

API_AddonType __ACDLL_CALL CheckEnvironment (API_EnvirParams* envir)
{
	RSGetIndString (&envir->addOnInfo.name, AddOnInfoID, AddOnNameID, ACAPI_GetOwnResModule ());
	RSGetIndString (&envir->addOnInfo.description, AddOnInfoID, AddOnDescriptionID, ACAPI_GetOwnResModule ());

	return APIAddon_Normal;
}

GSErrCode __ACDLL_CALL RegisterInterface (void)
{
	return ACAPI_Register_Menu (AddOnMenuID, 0, MenuCode_UserDef, MenuFlag_Default);
}

GSErrCode __ACENV_CALL Initialize (void)
{
	// Archicad‚ÉƒpƒŒƒbƒg‚ð“o˜^‚·‚é
	GSErrCode err = ACAPI_RegisterModelessWindow(ExamplePaletteGuidRefId, ExamplePaletteController::PaletteAPIControlCallBack,
		API_PalEnabled_FloorPlan + API_PalEnabled_Section + API_PalEnabled_Elevation + API_PalEnabled_InteriorElevation +
		API_PalEnabled_3D + API_PalEnabled_Detail + API_PalEnabled_Worksheet + API_PalEnabled_Layout + API_PalEnabled_DocumentFrom3D,
		GSGuid2APIGuid(ExamplePaletteGuid));

	err = ACAPI_Install_MenuHandler(AddOnMenuID, MenuCommandHandler);

	return err;
}

GSErrCode __ACENV_CALL FreeData (void)
{
	ACAPI_UnregisterModelessWindow(ExamplePaletteGuidRefId);
	return NoError;
}
