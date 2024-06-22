// Fill out your copyright notice in the Description page of Project Settings.


#include "W_MainMenu.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "W_ProfileMenu.h"
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/VerticalBox.h>
#include "UObject/ConstructorHelpers.h"

UW_MainMenu::UW_MainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// WB_ProfileMenu 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_ProfileMenu> WidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_ProfileMenu.WB_ProfileMenu'"));
	if (WidgetBPClass.Succeeded())
	{
		ProfileMenu_bp = WidgetBPClass.Class;
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

void UW_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	//if (HostGame_Btn)
	//{
	//    HostGame_Btn->OnClicked.AddDynamic(this, &UW_MainMenu::OnHostGame_BtnClicked);
	//}

	// Button Event Bind
	if (Profile_Btn)
	{
		Profile_Btn->Button->OnClicked.AddDynamic(this, &UW_MainMenu::OnProfile_BtnClicked);
	}
}

void UW_MainMenu::SetVerticalBoxButtonContainerVisibility(ESlateVisibility InVisibility)
{
	VerticalBox_ButtonContainer->SetVisibility(InVisibility);
}
