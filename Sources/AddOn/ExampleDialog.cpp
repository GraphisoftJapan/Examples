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