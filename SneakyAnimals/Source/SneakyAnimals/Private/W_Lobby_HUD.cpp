// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Lobby_HUD.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GS_Lobby.h"
#include "PS_Lobby.h"
#include "PC_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CircularThrobber.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "GM_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/BackgroundBlur.h>

void UW_Lobby_HUD::Set_ServerName(const FText& ServerName)
{
	// set lobby name
	LobbyName_Text->SetText(FText::FromString(ServerName.ToString() + "\'s lobby"));
	
	return;
}

void UW_Lobby_HUD::Toggle_Launchevent(bool _bLaunchingGame)
{
	bLaunchingGame = _bLaunchingGame;
	if (bLaunchingGame) {
		if (!bLaunchActive)
		{
			bLaunchActive = true;
			CountDown = 5;
			Countdown_Launchevent();
		}
	}
	else {
		bLaunchActive = false;
	}
}

void UW_Lobby_HUD::Countdown_Launchevent()
{
	FText CountDownText = FText::Format(FText::FromString(TEXT("{0} seconds till game starts")), CountDown);

	if (!CountDownText.IsEmpty())
	{
		Update_LobbyStatus_Indicator(ESlateVisibility::Collapsed, CountDownText);
	}
	CountDown--;
	if (CountDown == -1)
	{
		if (UKismetSystemLibrary::IsServer(GetWorld())) {
			AGM_Lobby* gm_lobby = Cast<AGM_Lobby>(UGameplayStatics::GetGameMode(GetWorld()));
			gm_lobby->ServerTravel();
		}
	} 
	else
	{
		FTimerHandle Timer_Countdown;
		GetWorld()->GetTimerManager().SetTimer(Timer_Countdown, [&]() {
				if (bLaunchingGame)
				{
					Countdown_Launchevent();
				}
			}, 1.0f, false);
	}
}

void UW_Lobby_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	InitCasts();
	Setup();
}

void UW_Lobby_HUD::InitCasts()
{
	 GS_Lobby = Cast<AGS_Lobby>(UGameplayStatics::GetGameState(GetWorld()));
	 if (APlayerController* OwningPlayer = this->GetOwningPlayer())
	 {
		 PS_Lobby = Cast<APS_Lobby>(OwningPlayer->PlayerState);
	 }
	 else
	 {
		 UE_LOG(LogTemp, Error, TEXT("OwningPlayer is null"));
	 }
	 PC_Lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UW_Lobby_HUD::Toggle_BackgroundBlur(bool bEnableBlur)
{
	if (bEnableBlur)
	{
		BackgroundBlur->SetVisibility(ESlateVisibility::HitTestInvisible);
		BackgroundBlur->SetBlurStrength(15.0f);
	} 
	else
	{
		BackgroundBlur->SetVisibility(ESlateVisibility::Collapsed);
		BackgroundBlur->SetBlurStrength(0.0f);
	}
}

void UW_Lobby_HUD::Update_LobbyStatus_Indicator(ESlateVisibility _CircularThrobberVisibility, const FText& _LobbyStatus_Text)
{
	if(CircularThrobber)
	{
	CircularThrobber->SetVisibility(_CircularThrobberVisibility);
	}
	if(LobbyStatus_Text->IsValidLowLevel() && !_LobbyStatus_Text.IsEmpty()) 
	{
	LobbyStatus_Text->SetText(_LobbyStatus_Text);
	}
}

void UW_Lobby_HUD::Setup()
{
	InitLobbyStatusIndicator();
}

void UW_Lobby_HUD::InitLobbyStatusIndicator()
{
	if (PS_Lobby && PS_Lobby->bCanHostForceLaunchGame) {
		Update_LobbyStatus_Indicator(ESlateVisibility::Visible, FText::FromString(TEXT("방장이 게임 시작하기를 기다리는 중...")));
	}
	else {
		if (GS_Lobby->bReadyForLaunch)
		{
			Update_LobbyStatus_Indicator(ESlateVisibility::Visible, FText::FromString(TEXT("Waiting on host to start the game")));
		} 
		else
		{
			Update_LobbyStatus_Indicator(ESlateVisibility::Visible, FText::FromString(TEXT("Waiting for everyone to ready up")));
		}
	}
}