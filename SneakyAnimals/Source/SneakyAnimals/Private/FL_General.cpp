// Fill out your copyright notice in the Description page of Project Settings.


#include "FL_General.h"
#include "SG_UserProfile.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "W_LoadingScreen.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include "W_PopUp.h"
#include "GI_SneakyAnimals.h"
#include "SG_PlayerAppearance.h"

TSubclassOf<UUserWidget> UFL_General::WB_LoadingScreen_bp = nullptr;
TSubclassOf<UUserWidget> UFL_General::WB_PopUp_bp = nullptr;

UFL_General::UFL_General()
{
	if (!WB_LoadingScreen_bp)
	{
		ConstructorHelpers::FClassFinder<UW_LoadingScreen> WidgetFinder(TEXT("/Game/KYJ/Widgets/General/WB_LoadingScreen.WB_LoadingScreen_C"));
		if (WidgetFinder.Succeeded())
		{
			WB_LoadingScreen_bp = WidgetFinder.Class;
		}
	}

	if (!WB_PopUp_bp)
	{
		ConstructorHelpers::FClassFinder<UW_PopUp> WidgetFinder2(TEXT("/Game/KYJ/Widgets/General/WB_PopUp.WB_PopUp_C"));
		if (WidgetFinder2.Succeeded())
		{
			WB_PopUp_bp = WidgetFinder2.Class;
		}
	}
}

bool UFL_General::Save_UserProfile(const FStructure_UserProfile& S_UserData)
{
	// Save Game 생성
	USG_UserProfile* SG_UserProfile_inst = Cast<USG_UserProfile>(UGameplayStatics::CreateSaveGameObject(USG_UserProfile::StaticClass()));

	// User Data 저장
	SG_UserProfile_inst->S_UserProfile = S_UserData;

	// 저장 슬롯에 데이터를 저장
	return UGameplayStatics::SaveGameToSlot(SG_UserProfile_inst, TEXT("UserProfile_Slot"), 0);

}

FUserProfileResult UFL_General::Get_UserProfile()
{
	FUserProfileResult result;

	if (UGameplayStatics::DoesSaveGameExist(TEXT("UserProfile_Slot"), 0))
	{
		// UserProfile_Slot 이 있으면 S_UserProfile과 bool success = true 리턴하기
		USG_UserProfile* sg = Cast<USG_UserProfile>(UGameplayStatics::LoadGameFromSlot(TEXT("UserProfile_Slot"), 0));
		if (sg)
		{
			result.S_UserProfile = sg->S_UserProfile;
			result.success = true;
			return result;
		}
	}

	// UserProfile_Slot 이 없으면 비어있는 S_UserProfile과 bool success = false 리턴하기
	result.S_UserProfile = FStructure_UserProfile();
	result.success = false;
	return result;
}

bool UFL_General::Save_Player_Appearance(FStructure_Player_Appearance S_Player_Appearance)
{
	// Save Game 생성
	USG_PlayerAppearance* SaveGameObject = Cast<USG_PlayerAppearance>(UGameplayStatics::CreateSaveGameObject(USG_PlayerAppearance::StaticClass()));

	// 데이터 저장
	SaveGameObject->S_PlayerAppearance = S_Player_Appearance;

	// 저장 슬롯에 데이터를 저장
	return UGameplayStatics::SaveGameToSlot(SaveGameObject, TEXT("PlayerAppearance_Slot"), 0);
}

FStructure_Player_Appearance_Result UFL_General::Get_Player_Appearance()
{
	FStructure_Player_Appearance_Result result;

	if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerAppearance_Slot"), 0))
	{
		// PlayerAppearance_Slot 이 있으면
		USG_PlayerAppearance* sg = Cast<USG_PlayerAppearance>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerAppearance_Slot"), 0));
		if (sg)
		{
			result.Player_Appearance = sg->S_PlayerAppearance;
			result.bSuccess = true;
			return result;
		}
	}

	// UserProfile_Slot 이 없으면 비어있는 Player_Appearance과 bool success = false 리턴하기
	result.Player_Appearance = FStructure_Player_Appearance();
	result.bSuccess = false;

	return result;
}

UW_LoadingScreen* UFL_General::Create_LoadingScreen(UObject* WorldContextObject, UTexture2D* LoadingScreenImage, FText LoadingScreenFeedbackText)
{
	if (WB_LoadingScreen_bp && WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
		if (World)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
			if (PlayerController && PlayerController->IsLocalController())
			{
				UW_LoadingScreen* UserWidget = CreateWidget<UW_LoadingScreen>(PlayerController, WB_LoadingScreen_bp);
				if (UserWidget)
				{
					UserWidget->LoadingScreen_Image = LoadingScreenImage;
					UserWidget->LoadingScreen_FeedbackText = LoadingScreenFeedbackText;

					UserWidget->AddToViewport();
					return UserWidget;
				}
			}
		}
	}
	return nullptr;
}

class UW_PopUp* UFL_General::Create_PopUp(UObject* WorldContextObject, FText PopUp_Message, FText Left_Btn_Text, bool bContains_Right_Button, FText Right_Btn_Text)
{
	if (WB_PopUp_bp && WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
		if (World)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
			if (PlayerController && PlayerController->IsLocalController())
			{
				UW_PopUp* UserWidget = CreateWidget<UW_PopUp>(PlayerController, WB_PopUp_bp);
				if (UserWidget)
				{
					UserWidget->PopUp_Message = PopUp_Message;
					UserWidget->Left_Btn_Text = Left_Btn_Text;
					UserWidget->Right_Btn_Text = Right_Btn_Text;
					UserWidget->bContains_Right_Button = bContains_Right_Button;

					UserWidget->AddToViewport();
					return UserWidget;
				}
			}
		}
	}
	return nullptr;
}

void UFL_General::Set_SessionInfo(UObject* WorldContextObject, FString ServerName, int32 MaxPlayers)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World)
	{
		UGI_SneakyAnimals* gi = Cast<UGI_SneakyAnimals>(World->GetGameInstance());

		#pragma region old version
		// old version
		//FStructure_SessionInfo S_SessionInfo;
		//S_SessionInfo.ServerName = ServerName;
		//S_SessionInfo.MaxPlayers = MaxPlayers;
		#pragma endregion

		// new version
		FSessionInfo S_SessionInfo;
		S_SessionInfo.hostName = ServerName;
		S_SessionInfo.maxPlayerCount = MaxPlayers;
		gi->S_SessionInfo = S_SessionInfo;
	}
	return;
}

FSessionInfo UFL_General::Get_SessionInfo(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World)
	{
		UGI_SneakyAnimals* gi = Cast<UGI_SneakyAnimals>(World->GetGameInstance());

		return gi->S_SessionInfo;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("World is not valid"));
		return FSessionInfo();
	}
}
