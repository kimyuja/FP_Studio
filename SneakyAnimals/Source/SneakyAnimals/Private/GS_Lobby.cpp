// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Lobby.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PC_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SlateWrapperTypes.h>
#include "PS_Base.h"
#include "PS_Lobby.h"

void AGS_Lobby::OnRep_bReadyForLaunch()
{
	if (bReadyForLaunch)
	{
		// Update Lobby Status Indicator
		Update_LobbyStatus_Indicator(ESlateVisibility::HitTestInvisible, FText::FromString(TEXT("Waiting on host to start the game")));

		// Update host's Ready Up button
		Update_Host_ReadyUpButton(FText::FromString(TEXT("Start game")), false);
	} 
	else
	{
		// Update Lobby Status Indicator
		Update_LobbyStatus_Indicator(ESlateVisibility::HitTestInvisible, FText::FromString(TEXT("Waiting for everyone to ready up")));

		// Update host's Ready Up button
		Update_Host_ReadyUpButton(FText::FromString(TEXT("Waiting")), true);
	}
}

void AGS_Lobby::OnRep_bLaunching()
{
	if (bLaunching)
	{
		// Update that we are launching
		Update_Lobby_Launchevent(true);
		Update_Host_ReadyUpButton(FText::FromString(TEXT("Cancel launch")), false);
	} 
	else
	{
		// Update that we canceled launch
		Update_Lobby_Launchevent(false);
		APS_Lobby* ps_lobby = Cast<APS_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
		if (ps_lobby->bCanHostForceLaunchGame)
		{
			// Incase host can force start match, then display following info
			Update_LobbyStatus_Indicator(ESlateVisibility::HitTestInvisible, FText::FromString(TEXT("Waiting on host to start the game")));
			Update_Host_ReadyUpButton(FText::FromString(TEXT("Start game")), false);
		} 
		else
		{
			// Incase host can not force start match, then reset all players, and update the Lobby Status Indicator
			Reset_Player_ReadyStatus();

			if (bReadyForLaunch)
			{
				Update_LobbyStatus_Indicator(ESlateVisibility::HitTestInvisible, FText::FromString(TEXT("Waiting on host to start the game")));
			} 
			else
			{
				Update_LobbyStatus_Indicator(ESlateVisibility::HitTestInvisible, FText::FromString(TEXT("Waiting for everyone to ready up")));
			}
		}
	}
}

void AGS_Lobby::OnRep_ServerName()
{
	APC_Lobby* pc_lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	pc_lobby->ClientRPC_Update_ServerName(ServerName);
}

void AGS_Lobby::OnRep_ConnectedPlayers_PlayerConnectionInfo()
{
	Update_Lobby_PlayerList();
	if (HasAuthority())
	{
		Check_EveryoneReady();
	}
}

void AGS_Lobby::Update_Lobby_PlayerList()
{
	APC_Lobby* pc_lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	pc_lobby->ClientRPC_Update_ConnectedPlayersList(ConnectedPlayers_PlayerConnectionInfo);
	return;
}

void AGS_Lobby::Update_Host_ReadyUpButton(const FText& Text, bool bWarningState)
{
	if (HasAuthority())
	{
		APC_Lobby* pc_lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		pc_lobby->ClientRPC_Update_ReadyUpButton(pc_lobby, Text, bWarningState, false);
		return;
	}
}

void AGS_Lobby::Update_ServerName()
{
	FText SessionName;
	for (APlayerState* ps : PlayerArray)
	{
		APS_Base* ps_base = Cast<APS_Base>(ps);
		if (ps_base->Player_ConnectionInfo.bIsHost)
		{
			SessionName = ps_base->Player_UserProfile.Username;
			break;
		}
		
	}
	ServerName = SessionName;
	OnRep_ServerName();
	return;
}

void AGS_Lobby::Update_ConnectedPlayers_Array()
{
	TArray<FStructure_ConnectedPlayer> Local_ConnectedPlayers;

	// KYJ Test : player state 먼저 실행 후 player list 업데이트 하라고 delegate 칠 것

	int32 idx = 0;
	for (APlayerState* ps : PlayerArray)
	{
		APS_Base* ps_base = Cast<APS_Base>(ps);
		if (ps_base)
		{
			// PlayerID 를 간단한 인덱스로 바꿔주기
			FStructure_PlayerConnectionInfo Local_PlayerConnectionInfo = ps_base->Player_ConnectionInfo;
			Local_PlayerConnectionInfo.PlayerID = idx;

			// ConnectedPlayer 구조체 만들어서 배열에 저장하기
			FStructure_ConnectedPlayer Local_ConnectedPlayer;
			Local_ConnectedPlayer.S_PlayerConnectionInfo = Local_PlayerConnectionInfo;
			Local_ConnectedPlayer.S_UserProfile = ps_base->Player_UserProfile;
			UE_LOG(LogTemp, Warning, TEXT("player array user name :  %s"), *ps_base->Player_UserProfile.Username.ToString());
			Local_ConnectedPlayer.S_Player_Appearance = ps_base->Player_Appearance;

			Local_ConnectedPlayers.Add(Local_ConnectedPlayer);
		}
		idx++;
	}
	
	ConnectedPlayers_PlayerConnectionInfo = Local_ConnectedPlayers;
	OnRep_ConnectedPlayers_PlayerConnectionInfo();
	Update_ServerName();
	return;

}

void AGS_Lobby::Update_Lobby()
{
	Update_ConnectedPlayers_Array();
	return;
}

void AGS_Lobby::Check_EveryoneReady()
{
	bool Local_bReadyForLaunch = false;

	// If one player is found that is not ready, then that means that we are Not Ready for Launch
	for (FStructure_ConnectedPlayer p : ConnectedPlayers_PlayerConnectionInfo)
	{
		if (p.S_PlayerConnectionInfo.bReadyStatus)
		{
			Local_bReadyForLaunch = true;
		} 
		else
		{
			Local_bReadyForLaunch = false;
			break;
		}
	}
	bReadyForLaunch = Local_bReadyForLaunch;
	OnRep_bReadyForLaunch();
	return;
	
}

void AGS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGS_Lobby, bReadyForLaunch);
	DOREPLIFETIME(AGS_Lobby, bLaunching);
	DOREPLIFETIME(AGS_Lobby, ServerName);
	DOREPLIFETIME(AGS_Lobby, ConnectedPlayers_PlayerConnectionInfo);
}

void AGS_Lobby::Update_KickCount()
{
	++KickCount;
}

int32 AGS_Lobby::Get_KickCount()
{
	return KickCount;
}

void AGS_Lobby::Update_LobbyStatus_Indicator(ESlateVisibility s, FText LobbyStatus_Text)
{
	APC_Lobby* pc_lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// pc_lobby->OC_Update_LobbyStatus_Indicator();
	pc_lobby->ClientRPC_Update_LobbyStatusIndicator(s, LobbyStatus_Text);
	return;
}

void AGS_Lobby::Update_Lobby_Launchevent(bool bLaunchingGame)
{
	APC_Lobby* pc_lobby = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	pc_lobby->ClientRPC_Launchevent(bLaunchingGame);
	return;
}

void AGS_Lobby::Reset_Player_ReadyStatus()
{
	// This event only get's called when the lobby is launching, and the host then cancels the launch.
	if (HasAuthority())
	{
		// Set the hosts Ready Up button depending on wether we are still Ready for Launch. Ready for launch can be true incase the host can 'ForceLaunchGame'.
		if (bReadyForLaunch)
		{
			Update_Host_ReadyUpButton(FText::FromString(TEXT("Start game")), false);
		} 
		else
		{
			Update_Host_ReadyUpButton(FText::FromString(TEXT("Waiting")), true);
		}
	} 
	else
	{
		Cast<APS_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState)->ServerRPC_Toggle_ReadyStatus();
	}
}
