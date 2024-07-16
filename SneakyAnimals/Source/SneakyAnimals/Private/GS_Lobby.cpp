// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Lobby.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PC_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SlateWrapperTypes.h>

void AGS_Lobby::OnRep_bReadyForLaunch()
{
	if (bReadyForLaunch)
	{
		// Update Lobby Status Indicator
		Update_LobbyStatus_Indicator(ESlateVisibility::HitTestInvisible, FText::FromString(TEXT("방장이 게임 시작하기를 기다리는 중...")));

		// Update host's Ready Up button
	} 
	else
	{
		// Update Lobby Status Indicator
		Update_LobbyStatus_Indicator(ESlateVisibility::HitTestInvisible, FText::FromString(TEXT("참가자 모두 준비되기를 기다리는 중...")));

		// Update host's Ready Up button
	}
}

void AGS_Lobby::Update_Host_ReadyUpButton(const FText& Text, bool bWarningState)
{
	if (HasAuthority())
	{
		Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

void AGS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_Lobby, bReadyForLaunch);
}

void AGS_Lobby::Update_LobbyStatus_Indicator(ESlateVisibility, FText LobbyStatus_Text)
{
	APC_Lobby* pc_lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// pc_lobby->OC_Update_LobbyStatus_Indicator();
}
