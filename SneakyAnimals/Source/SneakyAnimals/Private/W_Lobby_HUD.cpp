// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Lobby_HUD.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GS_Lobby.h"
#include "PS_Lobby.h"
#include "PC_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CircularThrobber.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void UW_Lobby_HUD::Set_ServerName(const FText& ServerName)
{
	// set lobby name
	LobbyName_Text->SetText(FText::FromString(ServerName.ToString() + "\'s lobby"));
	
	return;
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
	 PS_Lobby = Cast<APS_Lobby>(UWidget::GetOwningPlayer()->PlayerState);
	 PC_Lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UW_Lobby_HUD::Setup()
{
	InitLobbyStatusIndicator();
}

void UW_Lobby_HUD::InitLobbyStatusIndicator()
{
	if (PS_Lobby->bCanHostForceLaunchGame) {
		Update_LobbyStatus_Indicator(ESlateVisibility::Visible, FText::FromString(TEXT("방장이 게임 시작하기를 기다리는 중...")));
	}
	else {
		if (GS_Lobby->bReadyForLaunch)
		{
			Update_LobbyStatus_Indicator(ESlateVisibility::Visible, FText::FromString(TEXT("방장이 게임 시작하기를 기다리는 중...")));
		} 
		else
		{
			Update_LobbyStatus_Indicator(ESlateVisibility::Visible, FText::FromString(TEXT("참가자 모두 게임 준비가 되기를 기다리는 중...")));
		}
	}
}

void UW_Lobby_HUD::Update_LobbyStatus_Indicator(ESlateVisibility eSlateVisibility, FText LobbyStatusText)
{
	CircularThrobber->SetVisibility(eSlateVisibility);
	LobbyStatus_Text->SetText(LobbyStatusText);
}
