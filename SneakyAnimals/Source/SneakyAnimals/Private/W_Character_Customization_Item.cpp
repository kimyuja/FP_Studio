// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Character_Customization_Item.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include "PS_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "FL_General.h"

void UW_Character_Customization_Item::OnClickedButton()
{
	// 이거 나중에 enum 타입으로 고치기
	if (ItemType == "Character")
	{
		PS_Lobby->Player_Appearance.Character = Character;

		// Save_Player_Appearance
		UFL_General::Save_Player_Appearance(PS_Lobby->Player_Appearance);

		// SR_Update_Player_Appearance
		PS_Lobby->ServerRPC_Update_Player_Appearance(PS_Lobby->Player_Appearance);

	}
	else if (ItemType == "Accessory_Hat")
	{

	}
}

void UW_Character_Customization_Item::NativeConstruct()
{
	Super::NativeConstruct();

	Setup();

	PS_Lobby = Cast<APS_Lobby>(GetOwningPlayer()->PlayerState);
	
	Button->OnClicked.AddDynamic(this, &UW_Character_Customization_Item::OnClickedButton);
}

void UW_Character_Customization_Item::Setup()
{
	// 이거 나중에 enum 타입으로 고치기
	if (ItemType == "Character")
	{
		Item_Image->SetBrushFromTexture(Character.ItemImage);
	}
	else if (ItemType == "Accessory_Hat")
	{
		
	}
}
