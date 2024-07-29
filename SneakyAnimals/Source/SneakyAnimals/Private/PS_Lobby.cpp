// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Lobby.h"
#include "PC_Lobby.h"
#include "TestPlayer.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GS_Lobby.h"

void APS_Lobby::Toggle_ReadyStatus()
{
	if (Get_ReadyStatus())
	{
		Set_ReadyStatus(false);
		APC_Lobby* pc_lobby = Cast<APC_Lobby>(GetPlayerController());
		pc_lobby->ClientRPC_Update_ReadyUpButton(pc_lobby, FText::FromString(TEXT("Ready up")), false, false);
	} 
	else
	{
		Set_ReadyStatus(true);
		APC_Lobby* pc_lobby = Cast<APC_Lobby>(GetPlayerController());
		pc_lobby->ClientRPC_Update_ReadyUpButton(pc_lobby, FText::FromString(TEXT("Cancel ready")), true, false);
	}
}

void APS_Lobby::BeginPlay()
{
	Super::BeginPlay();
	ClientRPC_Init();
}

void APS_Lobby::ClientRPC_Init_Implementation()
{
	Load_Player_ConnectionInfo(false);
	Super::Load_Player_UserProfile();
	Super::Load_Player_Appearance();
}

void APS_Lobby::Load_Player_ConnectionInfo(bool _ClientReadyStatus)
{
	Super::Load_Player_ConnectionInfo(bCanHostForceLaunchGame);
}

void APS_Lobby::ClientRPC_Init()
{
	ClientRPC_Init_Implementation();
}

void APS_Lobby::OnRep_Player_Appearance_OR()
{
    if (HasAuthority())
    {
        // OC_Update_CharacterCustomizationPanel()
        auto* pc_lobby = Cast<APC_Lobby>(GetPlayerController());
        if (pc_lobby)
        {
            pc_lobby->ClientRPC_Update_Character_Customization_Panel();

            auto* player = Cast<ATestPlayer>(pc_lobby->AController::K2_GetPawn());
            if (player)
            {
                // player의 Current_SkeletalMesh를 Player_Appearance.Character.Mesh로 바꾼다
                player->Current_SkeletalMesh = Player_Appearance.Character.Mesh;
                player->Current_Accessories = Player_Appearance.Accessories_Slot.Mesh;
                player->Current_Skins = Player_Appearance.Skins_Slot.Material;
                player->Current_Eyes = Player_Appearance.Eyes_Slot.Material;
                player->Current_Top = Player_Appearance.Top_Slot.Mesh;
                player->Current_Bottom = Player_Appearance.Bottom_Slot.Mesh;
                player->Current_Outer = Player_Appearance.Outer_Slot.Mesh;
                player->Current_Dress = Player_Appearance.Dress_Slot.Mesh;

                // player의 OnRep_Current_SkeletalMesh가 실행된다.
                player->OnRep_Current_SkeletalMesh();
                player->OnRep_Current_Accessories();
                player->OnRep_Current_Skins();
                player->OnRep_Current_Eyes();
                player->OnRep_Current_Top();
                player->OnRep_Current_Bottom();
                player->OnRep_Current_Outer();
                player->OnRep_Current_Dress();
            }
            else
            {
                GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APS_Lobby::OnRep_Player_Appearance_OR);
            }
        }
    }
}


void APS_Lobby::OnRep_Player_UserProfile_OR()
{
	if (HasAuthority())
	{
		AGS_Lobby* gs_lobby = Cast<AGS_Lobby>(UGameplayStatics::GetGameState(GetWorld()));
		gs_lobby->Update_Lobby();
	}
}

void APS_Lobby::OnRep_Player_ConnectionInfo_OR()
{
	if (HasAuthority())
	{
		AGS_Lobby* gs_lobby = Cast<AGS_Lobby>(UGameplayStatics::GetGameState(GetWorld()));
		gs_lobby->Update_Lobby();
	}
}

bool APS_Lobby::Get_ReadyStatus()
{
	return Player_ConnectionInfo.bReadyStatus;
}

void APS_Lobby::Set_ReadyStatus(bool _bReadyStatus)
{
	Player_ConnectionInfo.bReadyStatus = _bReadyStatus;

	// 만약 이 부분이 안 먹힌다면 OnRep_Player_ConnectionInfo_OR()을 호출하기.
	OnRep_Player_ConnectionInfo();
}

void APS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APS_Lobby, bCanHostForceLaunchGame);
}

void APS_Lobby::ServerRPC_Toggle_ReadyStatus_Implementation()
{
	Toggle_ReadyStatus();
}
