// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Lobby.h"
#include "PC_Lobby.h"
#include "TestPlayer.h"

void APS_Lobby::OnRep_Player_Appearance_OR()
{
	if (HasAuthority())
	{
		// OC_Update_CharacterCustomizationPanel()
		auto* pc_lobby = Cast<APC_Lobby>(GetPlayerController());
		pc_lobby->ClientRPC_Update_Character_Customization_Panel();

		auto* player = Cast<ATestPlayer>(pc_lobby->AController::K2_GetPawn());

		// player�� Current_SkeletalMesh�� Player_Appearance.Character.Mesh�� �ٲ۴�
		player->Current_SkeletalMesh = Player_Appearance.Character.Mesh;
		player->Current_Accessories = Player_Appearance.Accessories_Slot.Mesh;
		player->Current_Skins = Player_Appearance.Skins_Slot.Material;
		player->Current_Eyes = Player_Appearance.Eyes_Slot.Material;

		// player�� OnRep_Current_SkeletalMesh�� ����ȴ�.
		player->OnRep_Current_SkeletalMesh();
		player->OnRep_Current_Accessories();
		player->OnRep_Current_Skins();
		player->OnRep_Current_Eyes();
	}
}