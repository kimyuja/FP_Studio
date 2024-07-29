// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Base.h"
#include "PC_Lobby.h"

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

void AGM_Base::ServerTravel_ToGameplayMap()
{
	for (auto* pc : All_PCs)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(pc);	
	}
	GetWorld()->ServerTravel("/Game/RTY/LEVELS/GameMaps");
}
