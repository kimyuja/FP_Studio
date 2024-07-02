// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ServerBrowserMenu.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CheckBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CircularThrobber.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "GI_SneakyAnimals.h"
#include <W_ServerBrowserItem.h>

UW_ServerBrowserMenu::UW_ServerBrowserMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// ServerBrowserItem ���� �������Ʈ Ŭ������ �ε��Ѵ�.
	static ConstructorHelpers::FClassFinder<UW_ServerBrowserItem> WidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_ServerBrowserItem.WB_ServerBrowserItem_C'"));
	if (WidgetBPClass.Succeeded())
	{
		ServerBrowserItem_bp = WidgetBPClass.Class;
	}
}

void UW_ServerBrowserMenu::OnMyAddRoomInfoUI(const FSessionInfo& info)
{
	// room list add
	// ServerBrowserItem_bp�� �̿��ؼ� ������ �����
	auto ui = CreateWidget<UW_ServerBrowserItem>(this, ServerBrowserItem_bp);
	// info��  �����ϰ�ʹ�.
	ui->Setup(info);
	// ������ ������ ServerList_Scrollbox�� ���̰�ʹ�.
	ServerList_Scrollbox->AddChild(ui);
}

void UW_ServerBrowserMenu::OnMySetActiveFindingText(bool bSearching)
{
	// bSearching : true - ã�� �� circular throbber Active
	// bSearching : false - ã�� �Ϸ�. circular throbber Deactive
	
	// Refresh buttons ��Ȱ��ȭ, server list �ʱ�ȭ, and feedbacks user that we are refreshing
	Refresh_Btn->SetIsEnabled(!bSearching);
	CircularThrobber->SetVisibility(bSearching? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ServerBrowser_Feedback_Text->SetText(bSearching ? FText::FromString(TEXT("Refreshing...")) : FText::FromString(TEXT("Refreshed")));

}

void UW_ServerBrowserMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// gi�� ä���ʹ�.
	gi = Cast<UGI_SneakyAnimals>(GetWorld()->GetGameInstance());

	// gi�� OnMySessionSearchCompleteDelegate�� AddRoomInfoUI�� �����ϰ�ʹ�.
	gi->OnMySessionSearchCompleteDelegate.AddDynamic(this, &UW_ServerBrowserMenu::OnMyAddRoomInfoUI);
	gi->OnMySessionSearchFinishedDelegate.AddDynamic(this, &UW_ServerBrowserMenu::OnMySetActiveFindingText);

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
	RefreshServerList();
}

void UW_ServerBrowserMenu::RefreshServerList()
{
	ServerList_Scrollbox->ClearChildren();
	// �α� �߰�
	if (gi)
	{
		gi->FindOtherSessions();
	}
}

void UW_ServerBrowserMenu::OnMyCheckStateChanged(bool IsChecked)
{
	bSearchForLAN = IsChecked;
}

void UW_ServerBrowserMenu::OnMyBack_BtnClicked()
{
	RemoveFromParent();
}
