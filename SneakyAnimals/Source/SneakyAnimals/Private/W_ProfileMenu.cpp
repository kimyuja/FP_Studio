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
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "W_SmallProfile.h"
#include "W_MainMenu.h"
#include "W_Tooltip.h"
#include "GI_SneakyAnimals.h"

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

	New_Username = FText::FromString(TextStr);

	Username_Textbox->Textbox->SetText(New_Username);
}

void UW_ProfileMenu::OnSave_BtnClicked()
{
	// 중복된 유저 이름인가?
	if (Cast<UGI_SneakyAnimals>(GetGameInstance())->bContains_UserName(New_Username.ToString())) {

		UFL_General::Create_PopUp(GetWorld(), FText::FromString(TEXT("This username is already in use")), FText::FromString(TEXT("Close")), false, FText::FromString(TEXT("")));
		return;
	}

	// structure user profile 만들기
	S_UserProfile.Username = New_Username;
	S_UserProfile.User_Avatar = Selected_Avatar;

	// function library 의 save user profile 함수 만들기. 거기에 save game에다가 structure user profile 을 저장한다.
	if (UFL_General::Save_UserProfile(S_UserProfile))
	{
		Get_UserProfile();

	}
	// wb_small_profile
	TArray<UUserWidget*> foundWidgets;
	TSubclassOf<UW_SmallProfile> widgetClass = UW_SmallProfile::StaticClass();
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), foundWidgets, widgetClass, false);

	for (UUserWidget* u : foundWidgets)
	{
		// refresh widget
		UW_SmallProfile* w = Cast<UW_SmallProfile>(u);
		w->Refresh_Widget();
	}
}

void UW_ProfileMenu::OnBack_BtnClicked()
{
	UWidget::RemoveFromParent();
	UW_MainMenu* m = Cast<UW_MainMenu>(ParentWidget);
	m->SetVerticalBoxButtonContainerVisibility(ESlateVisibility::Visible);
}

void UW_ProfileMenu::OnCancel_BtnClicked()
{
	Get_UserProfile();
}

UW_ProfileMenu::UW_ProfileMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// DT_Available_Avatars 를 로드한다.
	static ConstructorHelpers::FObjectFinder<UDataTable> InGameAvailableAvatarsTable(TEXT("/Game/KYJ/Data/DT_Available_Avatars"));
	if (InGameAvailableAvatarsTable.Succeeded())
	{
		DT_Available_Avatars = InGameAvailableAvatarsTable.Object;
	}


	// WB_AvailableAvatar 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_AvailableAvatar> WidgetBPClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/MainMenu/WB_AvailableAvatar.WB_AvailableAvatar_C'"));
	if (WidgetBPClass.Succeeded())
	{
		AvailableAvatar_bp = WidgetBPClass.Class;
	}

	// WB_Tooltip 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_Tooltip> WidgetBPClass2(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYJ/Widgets/General/WB_Tooltip.WB_Tooltip_C'"));
	if (WidgetBPClass2.Succeeded())
	{
		Tooltip_bp = WidgetBPClass2.Class;
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

	Get_UserProfile();

	Tooltip_inst = CreateWidget<UW_Tooltip>(this, Tooltip_bp, FName("tooltip1"));

	if (Tooltip_inst)
	{
		Tooltip_inst->Tooltip_Title = FText::FromString(TEXT("Set a username"));
		Tooltip_inst->Tooltip_Description = FText::FromString(TEXT("Max 12 characters"));
	}
	Username_Textbox->SetToolTip(Tooltip_inst);

	// Event Bind
	if (Username_Textbox)
	{
		Username_Textbox->Textbox->OnTextChanged.AddDynamic(this, &UW_ProfileMenu::OnUsername_Textbox_Changed);
	}

	if (Save_Btn)
	{
		Save_Btn->Button->OnClicked.AddDynamic(this, &UW_ProfileMenu::OnSave_BtnClicked);
	}

	if (Back_Btn)
	{
		Back_Btn->Button->OnClicked.AddDynamic(this, &UW_ProfileMenu::OnBack_BtnClicked);
	}
	if (Cancel_Btn)
	{
		Cancel_Btn->Button->OnClicked.AddDynamic(this, &UW_ProfileMenu::OnCancel_BtnClicked);
	}
}

void UW_ProfileMenu::Populate_AvailableAvatars()
{
	TArray<FName> MyOutRowNames;
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

void UW_ProfileMenu::Get_UserProfile()
{
	FUserProfileResult UserProfileResult = UFL_General::Get_UserProfile();

	if (UserProfileResult.success)
	{
		S_UserProfile = UserProfileResult.S_UserProfile;
		// User Profile이 성공적으로 로드 되었다면
		Username_Text->SetText(UserProfileResult.S_UserProfile.Username);
		Username_Textbox->Textbox->SetText(FText::FromString(""));
		New_Username = UserProfileResult.S_UserProfile.Username;
		Username_Textbox->Textbox->SetHintText(New_Username);
		Selected_Avatar = UserProfileResult.S_UserProfile.User_Avatar;
		CurrentAvatar_Image->SetBrushFromTexture(UserProfileResult.S_UserProfile.User_Avatar);
	}
}
