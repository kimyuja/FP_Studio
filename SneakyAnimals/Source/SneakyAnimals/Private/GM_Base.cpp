// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Base.h"
#include "PC_Lobby.h"
#include "TestPlayer.h"
#include "GS_Lobby.h"

void AGM_Base::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	Handle_PlayerConnection(NewPlayer);
}

void AGM_Base::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	Handle_PlayerConnection(NewPC);
}

void AGM_Base::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	Handle_PlayerDisconnection(Exiting);
}

void AGM_Base::KickPlayer(int32 PlayerID)
{
	Cast<APC_Lobby>(All_PCs[PlayerID])->ClientRPC_KickedFromLobby();

	FTimerHandle t;
	GetWorld()->GetTimerManager().SetTimer(t, [&](){
		Cast<AGS_Lobby>(GetWorld()->GetGameState())->Update_Lobby();
	}, 3.0f, false);
}

void AGM_Base::Handle_PlayerConnection(APlayerController* NewPlayer)
{
	All_PCs.Add(NewPlayer);
}

void AGM_Base::Handle_PlayerDisconnection(UObject* Player)
{
	APC_Base* pc_base = Cast<APC_Base>(Player);
	All_PCs.Remove(pc_base);
}

void AGM_Base::Spawn_Character(APlayerController* player)
{
	// player가 null인지 확인
	if (!player)
	{
		UE_LOG(LogTemp, Error, TEXT("Player controller is null"));
		return;
	}

	// ATestPlayer로 캐스트가 성공하는지 확인
	ATestPlayer* TestPlayer = Cast<ATestPlayer>(player->GetPawn());
	if (!TestPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to ATestPlayer failed"));
		return;
	}

	TestPlayer->ClientRPC_Init_Appearance();
}

void AGM_Base::ServerTravel_ToGameplayMap()
{
	for (auto* pc : All_PCs)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(pc);	
	}
	GetWorld()->ServerTravel("/Game/RTY/LEVELS/GameMaps");
}
