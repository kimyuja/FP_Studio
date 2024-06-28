// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ServerBrowserMenu.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CheckBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CircularThrobber.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void UW_ServerBrowserMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Event Bind
	if (Refresh_Btn)
	{
		Refresh_Btn->Button->OnClicked.AddDynamic(this, &UW_ServerBrowserMenu::RefreshServerList);
	}
	if (Back_Btn)
	{
		Back_Btn->Button->OnClicked.AddDynamic(this, &UW_ServerBrowserMenu::OnMyBack_BtnClicked);
	}
	if (bLAN_Checkbox)
	{
		bLAN_Checkbox->OnCheckStateChanged.AddDynamic(this, &UW_ServerBrowserMenu::OnMyCheckStateChanged);
	}
}

void UW_ServerBrowserMenu::RefreshServerList()
{
	FindSessions();
}

void UW_ServerBrowserMenu::OnMyCheckStateChanged(bool IsChecked)
{
	bSearchForLAN = IsChecked;
}

void UW_ServerBrowserMenu::OnMyBack_BtnClicked()
{
	RemoveFromParent();
}

void UW_ServerBrowserMenu::FindSessions()
{
	// Refresh buttons 비활성화, server list 초기화, and feedbacks user that we are refreshing
	Refresh_Btn->SetIsEnabled(false);
	ServerList_Scrollbox->ClearChildren();
	CircularThrobber->SetVisibility(ESlateVisibility::Visible);
	ServerBrowser_Feedback_Text->SetText(FText::FromString(TEXT("Refreshing...")));
	
	// find sessions



}
