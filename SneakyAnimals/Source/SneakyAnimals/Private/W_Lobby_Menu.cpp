// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Lobby_Menu.h"
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "GS_Lobby.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "PS_Lobby.h"
#include "PC_Lobby.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void UW_Lobby_Menu::OnReadyUp_BtnClicked()
{
	if (UKismetSystemLibrary::IsServer(GetWorld())) {
		Server_Toggle_Launchevent();
	}
	else {
		Client_Toggle_ReadyUp();
	}
}

void UW_Lobby_Menu::Set_ReadyUp_Button(const FText& Text, bool bWarningState, bool bHideButton, bool bLockButton)
{
	if (bHideButton)
	{
		ReadyUp_Btn->SetVisibility(ESlateVisibility::Collapsed);
	} 
	else
	{
		ReadyUp_Btn->SetVisibility(ESlateVisibility::Visible);
	}
	ReadyUp_Btn->SetIsEnabled(!bLockButton);
	ReadyUp_Btn->TextBlock->SetText(Text);
	
	UTexture2D* NormalImage  = nullptr;
	UTexture2D* HoveredImage = nullptr;
	UTexture2D* PressedImage = nullptr;
	if (bWarningState)
	{
		NormalImage  = T_Color6_Hovered;
		HoveredImage = T_Color6_Hovered;
		PressedImage = T_Color6_Pressed;
	} 
	else
	{
		NormalImage  = T_Color5_Normal;
		HoveredImage = T_Color5_Hovered;
		PressedImage = T_Color5_Pressed;
	}
	// 이미지로 SlateBrush 만들기
	FSlateBrush NormalBrush  = CreateSlateBrushFromTexture(NormalImage);
	FSlateBrush HoveredBrush = CreateSlateBrushFromTexture(HoveredImage);
	FSlateBrush PressedBrush = CreateSlateBrushFromTexture(PressedImage);

	// 만든 SlateBrush로 버튼 스타일 지정하기
	FButtonStyle NewStyle = CreateButtonStyle(NormalBrush, HoveredBrush, PressedBrush);
	
	ReadyUp_Btn->Button->SetStyle(NewStyle);
	return;
}

void UW_Lobby_Menu::Init_ReadyUp_Button()
{
	// Initialize the server or client ready up button
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		Init_Server_ReadyUp_Btn();
	} 
	else
	{
		Init_Client_ReadyUp_Btn();
	}
}

void UW_Lobby_Menu::NativeConstruct()
{
	Super::NativeConstruct();

	InitCasts();

	ReadyUp_Btn->Button->OnClicked.AddDynamic(this, &UW_Lobby_Menu::OnReadyUp_BtnClicked);
}

void UW_Lobby_Menu::InitCasts()
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

void UW_Lobby_Menu::Server_Toggle_Launchevent()
{
	if (PS_Lobby->bCanHostForceLaunchGame)
	{
		GS_Lobby->bLaunching = !(GS_Lobby->bLaunching);
		GS_Lobby->OnRep_bLaunching();
	} 
	else
	{
		if (GS_Lobby->bReadyForLaunch)
		{
			GS_Lobby->bLaunching = !(GS_Lobby->bLaunching);
			GS_Lobby->OnRep_bLaunching();
		}
	}
}

void UW_Lobby_Menu::Client_Toggle_ReadyUp()
{
	PS_Lobby->ServerRPC_Toggle_ReadyStatus();
}

FSlateBrush UW_Lobby_Menu::CreateSlateBrushFromTexture(UTexture2D* Texture) const
{
	FSlateBrush Brush;
	if (Texture)
	{
		Brush.SetResourceObject(Texture);
		Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());
	}
	return Brush;
}

FButtonStyle UW_Lobby_Menu::CreateButtonStyle(const FSlateBrush& NormalBrush, const FSlateBrush& HoveredBrush, const FSlateBrush& PressedBrush) const
{
	FButtonStyle ButtonStyle;

	// 버튼 스타일에 SlateBrush를 지정한다.
	ButtonStyle.SetNormal(NormalBrush);
	ButtonStyle.SetHovered(HoveredBrush);
	ButtonStyle.SetPressed(PressedBrush);

	return ButtonStyle;
}

void UW_Lobby_Menu::Init_Server_ReadyUp_Btn()
{
	if (PS_Lobby->bCanHostForceLaunchGame)
	{
		PC_Lobby->ClientRPC_Update_ReadyUpButton(PC_Lobby, FText::FromString(TEXT("Start game")), false, false);
	} 
	else
	{
		PC_Lobby->ClientRPC_Update_ReadyUpButton(PC_Lobby, FText::FromString(TEXT("Waiting")), true, false);
	}
}

void UW_Lobby_Menu::Init_Client_ReadyUp_Btn()
{
	if (PS_Lobby->bCanHostForceLaunchGame)
	{
		PC_Lobby->ClientRPC_Update_ReadyUpButton(PC_Lobby, FText::FromString(TEXT("")), false, true);
	} 
	else
	{
		PC_Lobby->ClientRPC_Update_ReadyUpButton(PC_Lobby, FText::FromString(TEXT("Ready up")), false, false);
	}
}
