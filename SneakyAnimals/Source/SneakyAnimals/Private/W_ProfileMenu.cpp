// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ProfileMenu.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/DataTableFunctionLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include "W_AvailableAvatar.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WrapBox.h>
#include "W_Base_Textbox.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/EditableTextBox.h>
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include "DataStructure.h"
#include "FL_General.h"

void UW_ProfileMenu::SetParentWidget(UUserWidget* InParentWidget)
{
	ParentWidget = InParentWidget;
}

void UW_ProfileMenu::OnUsername_Textbox_Changed(const FText& Text)
{
	if (Text.IsEmpty())
	{
		return;
	}
	// 텍스트가 안 비어있으면
	FString TextStr = UKismetStringLibrary::GetSubstring(Text.ToString(), 0, 12);
	
	New_Username = UKismetTextLibrary::Conv_StringToText(TextStr);

	Username_Textbox->Textbox->SetText(New_Username);
}

void UW_ProfileMenu::OnSave_BtnClicked()
{
	// structure user profile 만들기
	S_UserProfile->Username = New_Username;
	S_UserProfile->User_Avatar = Selected_Avatar;

	// function library 의 save user profile 함수 만들기. 거기에 save game에다가 structure user profile 을 저장한다.
	if (UFL_General::Save_UserProfile(*S_UserProfile)) 
	{
		// Get User Profile 함수 만들기
	}
	// save user profile (save game 만들어서)
	
	// function library 의 get user profile 만들기

	// wb_small_profile 만들기 widget blueprint

	// refresh widget 함수 만들기
}

UW_ProfileMenu::UW_ProfileMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// DT_Available_Avatars 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableAvatarsTable(TEXT("/Game/KYJ/Data/DT_Available_Avatars"));
	if (InGameAvailableAvatarsTable.Succeeded())
	{
		DT_Available_Avatars = InGameAvailableAvatarsTable.Object;
	}


	// WB_ProfileMenu 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_AvailableAvatar> WidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_AvailableAvatar.WB_AvailableAvatar_C'"));
	if (WidgetBPClass.Succeeded())
	{
		AvailableAvatar_bp = WidgetBPClass.Class;
	}
}

void UW_ProfileMenu::Set_SelectedAvatar(FName Avatar_Name)
{
	Selected_Avatar_Name = Avatar_Name;

	// 구조체로 캐스팅하여 데이터 가져오기
	S_Available_Avatars = DT_Available_Avatars->FindRow<FStructure_Available_Avatars>(Selected_Avatar_Name, TEXT(""));

	// 구조체로부터 Avatar 이미지 뽑아오기
	if (S_Available_Avatars)
	{
		Selected_Avatar = S_Available_Avatars->Avatar;
	}
	// bind된 Current Avatar Image에다가 Avatar 이미지 넣기
	CurrentAvatar_Image->SetBrushFromTexture(Selected_Avatar);
}

void UW_ProfileMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Populate_AvailableAvatars();

	// Get User Profile 해야 함 Function Library

	// Event Bind
	if (Username_Textbox)
	{
		Username_Textbox->Textbox->OnTextChanged.AddDynamic(this, &UW_ProfileMenu::OnUsername_Textbox_Changed);
	}

	if (Save_Btn)
	{
		Save_Btn->Button->OnClicked.AddDynamic(this, &UW_ProfileMenu::OnSave_BtnClicked);
	}

	// 나머지 버튼 만들 때 주석 해제해 알겠지
	//if (Back_Btn)
	//{
	//	Back_Btn->Button->OnClicked.AddDynamic(this, &UW_ProfileMenu::OnBack_BtnClicked);
	//}
	//if (Cancel_Btn)
	//{
	//	Cancel_Btn->Button->OnClicked.AddDynamic(this, &UW_ProfileMenu::OnCancel_BtnClicked);
	//}
}

void UW_ProfileMenu::Populate_AvailableAvatars()
{
	FPermissionListOwners MyOutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(DT_Available_Avatars, MyOutRowNames);

	for (FName RowName : MyOutRowNames)
	{
		// WB_Available_Avatar_Widget 위젯 생성하기
		// Avatar 이미지를 넘겨줘서 WB_Available_Avatar Widget을 생성해야 함
		// 구조체로 캐스팅하여 데이터 가져오기
		S_Available_Avatars = DT_Available_Avatars->FindRow<FStructure_Available_Avatars>(RowName, TEXT(""));

		AvailableAvatar_inst = CreateWidget<UW_AvailableAvatar>(this, AvailableAvatar_bp, RowName);

		if (AvailableAvatar_inst)
		{
			if (S_Available_Avatars)
			{
				AvailableAvatar_inst->Avatar = S_Available_Avatars->Avatar;
			}
			AvailableAvatar_inst->ParentWidget = this;
			AvailableAvatar_inst->Avatar_Name = RowName;
			// Add Child
			AvailableAvatars_WrapBox->AddChild(AvailableAvatar_inst);
		}
	}
}
