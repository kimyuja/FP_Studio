// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Lobby_PlayerList.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>

void UW_Lobby_PlayerList::Update_Widget(TArray<FStructure_ConnectedPlayer> _Connected_Players)
{
	Connected_Players = _Connected_Players;

}

void UW_Lobby_PlayerList::Update_PlayerList()
{
	PlayerList->ClearChildren();
	
	// Populates Player List with players

}

void UW_Lobby_PlayerList::Update_PlayerCount()
{
}
