#include "W_Base_Textbox.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableTextBox.h>

void UW_Base_Textbox::OnTextChanged(const FText& InText)
{
	// 텍스트가 바뀌면 delegate 실행
	OnMyTextChanged.Broadcast(InText);
}

void UW_Base_Textbox::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	// 텍스트 입력을 완료했을 때 delegate 실행
	OnMyTextCommitted.Broadcast(InText, CommitMethod);
}

void UW_Base_Textbox::NativeConstruct()
{
	Super::NativeConstruct();

	SizeBox->SetWidthOverride(Widget_Width);
	SizeBox->SetHeightOverride(Widget_Height);
	Textbox->SetText(Text);
	Textbox->SetHintText(HintText);
	Textbox->SetToolTipText(ToolTip_Text);

	if (Textbox)
	{
		Textbox->OnTextChanged.AddDynamic(this, &UW_Base_Textbox::OnTextChanged);
		Textbox->OnTextCommitted.AddDynamic(this, &UW_Base_Textbox::OnTextCommitted);
	}
}
