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
	// ServerBrowserItem 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_ServerBrowserItem> WidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_ServerBrowserItem.WB_ServerBrowserItem_C'"));
	if (WidgetBPClass.Succeeded())
	{
		ServerBrowserItem_bp = WidgetBPClass.Class;
	}
}

void UW_ServerBrowserMenu::OnMyAddRoomInfoUI(const FSessionInfo& info)
{
	// room list add
	// ServerBrowserItem_bp를 이용해서 위젯을 만들고
	auto ui = CreateWidget<UW_ServerBrowserItem>(this, ServerBrowserItem_bp);
	// info를  전달하고싶다.
	ui->Setup(info);
	// 생성한 위젯을 ServerList_Scrollbox에 붙이고싶다.
	ServerList_Scrollbox->AddChild(ui);
}

void UW_ServerBrowserMenu::OnMySetActiveFindingText(bool bSearching)
{
	// bSearching : true - 찾는 중 circular throbber Active
	// bSearching : false - 찾기 완료. circular throbber Deactive
	
	// Refresh buttons 비활성화, server list 초기화, and feedbacks user that we are refreshing
	Refresh_Btn->SetIsEnabled(!bSearching);
	CircularThrobber->SetVisibility(bSearching? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ServerBrowser_Feedback_Text->SetText(bSearching ? FText::FromString(TEXT("Refreshing...")) : FText::FromString(TEXT("Refreshed")));

}

void UW_ServerBrowserMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// gi을 채우고싶다.
	gi = Cast<UGI_SneakyAnimals>(GetWorld()->GetGameInstance());

	// gi의 OnMySessionSearchCompleteDelegate에 AddRoomInfoUI를 연결하고싶다.
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
	// 로그 추가
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
