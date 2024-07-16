// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Base.h"
#include "PC_Lobby.h"

void AGM_Base::KickPlayer(int32 PlayerID)
{
	Cast<APC_Lobby>(All_PCs[PlayerID])->ClientRPC_KickedFromLobby();
}
