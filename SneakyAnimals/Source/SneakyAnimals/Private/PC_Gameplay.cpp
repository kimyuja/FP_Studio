// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Gameplay.h"
#include <SAModeBase.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>

void APC_Gameplay::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 스테이트가 유효한지 검사하고 유효한 경우에만 Setup_PC 함수를 호출
	if (ValidatePlayerState())
	{
		Setup_PC();
	}
}
void APC_Gameplay::ServerRPC_Spawn_Character_Implementation()
{
	// KYJ 0726 Spawn character 함수 호출
	Cast<ASAModeBase>(GetWorld()->GetAuthGameMode())->Spawn_Character(this);
}
bool APC_Gameplay::ValidatePlayerState()
{
	// Ensures that the player state is Valid before we setup the PlayerController
	// 플레이어 컨트롤러를 설정하기 전에, 플레이어 스테이트가 유효한지 보장한다.
	FTimerHandle TimerHandle_PlayerStateCheck;
	
	if (PlayerState->IsValidLowLevel())
	{
		return true;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_PlayerStateCheck, [&]() {
			if (ValidatePlayerState())
			{
				Setup_PC();
			}
			}, 0.2f, false);

		return false;
	}
}

void APC_Gameplay::Setup_PC()
{
	if (IsLocalPlayerController()) 
	{
		ServerRPC_Spawn_Character();
	}
}
