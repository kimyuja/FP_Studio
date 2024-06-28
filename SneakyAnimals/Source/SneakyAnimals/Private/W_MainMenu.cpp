// Fill out your copyright notice in the Description page of Project Settings.


#include "W_MainMenu.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "W_ProfileMenu.h"
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/VerticalBox.h>
#include "UObject/ConstructorHelpers.h"
#include "W_HostGameMenu.h"
#include "W_ServerBrowserMenu.h"

UW_MainMenu::UW_MainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// WB_ProfileMenu 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_ProfileMenu> WidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_ProfileMenu.WB_ProfileMenu_C'"));
	if (WidgetBPClass.Succeeded())
	{
		ProfileMenu_bp = WidgetBPClass.Class;
	}

	// WB_HostGameMenu 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_HostGameMenu> WidgetBPClass2(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_HostGameMenu.WB_HostGameMenu_C'"));
	if (WidgetBPClass2.Succeeded())
	{
		HostGameMenu_bp = WidgetBPClass2.Class;
	}
	
	// WB_ServerBrowserMenu 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_ServerBrowserMenu> WidgetBPClass3(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_ServerBrowserMenu.WB_ServerBrowserMenu_C'"));
	if (WidgetBPClass3.Succeeded())
	{
		JoinGameMenu_bp = WidgetBPClass3.Class;
	}
}

void UW_MainMenu::OnProfile_BtnClicked()
{
	// ProfileMenu 를 화면에 출력한다.
	if (ProfileMenu_bp != nullptr)
	{
		ProfileMenu_inst = CreateWidget<UW_ProfileMenu>(GetOwningPlayer(), ProfileMenu_bp, FName("WB_ProfileMenu"));

		if (ProfileMenu_inst)
		{
			ProfileMenu_inst->AddToViewport(0);
			ProfileMenu_inst->SetParentWidget(this);
			SetVerticalBoxButtonContainerVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UW_MainMenu::OnHostGame_BtnClicked()
{
	// HostGameMenu 를 화면에 출력한다.
	if (HostGameMenu_bp != nullptr)
	{
		HostGameMenu_inst = CreateWidget<UW_HostGameMenu>(GetOwningPlayer(), HostGameMenu_bp, FName("WB_HostGameMenu"));

		if (HostGameMenu_inst)
		{
			HostGameMenu_inst->AddToViewport(0);
		}
	}
}

void UW_MainMenu::OnJoinGame_BtnClicked()
{
	// JoinGameMenu 를 화면에 출력한다.
	if (JoinGameMenu_bp != nullptr)
	{
		JoinGameMenu_inst = CreateWidget<UW_ServerBrowserMenu>(GetOwningPlayer(), JoinGameMenu_bp, FName("WB_ServerBrowserMenu"));

		if (JoinGameMenu_inst)
		{
			JoinGameMenu_inst->AddToViewport(0);
		}
	}
}

void UW_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Button Event Bind
	if (HostGame_Btn)
	{
	    HostGame_Btn->Button->OnClicked.AddDynamic(this, &UW_MainMenu::OnHostGame_BtnClicked);
	}

	if (Profile_Btn)
	{
		Profile_Btn->Button->OnClicked.AddDynamic(this, &UW_MainMenu::OnProfile_BtnClicked);
	}
	
	if (JoinGame_Btn)
	{
		JoinGame_Btn->Button->OnClicked.AddDynamic(this, &UW_MainMenu::OnJoinGame_BtnClicked);
	}
}

void UW_MainMenu::SetVerticalBoxButtonContainerVisibility(ESlateVisibility InVisibility)
{
	VerticalBox_ButtonContainer->SetVisibility(InVisibility);
}
