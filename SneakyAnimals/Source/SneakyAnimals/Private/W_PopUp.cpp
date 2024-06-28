// Fill out your copyright notice in the Description page of Project Settings.


#include "W_PopUp.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>

void UW_PopUp::OnWB_Button_ConfirmClicked()
{
	OnClicked_Confirm.Broadcast();
	RemoveFromParent();
}

void UW_PopUp::OnWB_Button_DeclineClicked()
{
	OnClicked_Decline.Broadcast();
	RemoveFromParent();
}

void UW_PopUp::NativeConstruct()
{
	PopUp_Text->SetText(PopUp_Message);
	WB_Button_Confirm->TextBlock->SetText(Left_Btn_Text);
	if (bContains_Right_Button)
	{
		WB_Button_Decline->SetVisibility(ESlateVisibility::Visible);
		WB_Button_Decline->TextBlock->SetText(Right_Btn_Text);
	}
	else
	{
		WB_Button_Decline->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (WB_Button_Confirm)
	{
		WB_Button_Confirm->Button->OnClicked.AddDynamic(this, &UW_PopUp::OnWB_Button_ConfirmClicked);
	}
	if (WB_Button_Decline)
	{
		WB_Button_Decline->Button->OnClicked.AddDynamic(this, &UW_PopUp::OnWB_Button_DeclineClicked);
	}
}
