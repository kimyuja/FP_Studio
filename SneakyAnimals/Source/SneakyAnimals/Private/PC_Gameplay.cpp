// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Gameplay.h"
#include <SAModeBase.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>

void APC_Gameplay::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ������Ʈ�� ��ȿ���� �˻��ϰ� ��ȿ�� ��쿡�� Setup_PC �Լ��� ȣ��
	if (ValidatePlayerState())
	{
		Setup_PC();
	}
}
void APC_Gameplay::ServerRPC_Spawn_Character_Implementation()
{
	// KYJ 0726 Spawn character �Լ� ȣ��
	Cast<ASAModeBase>(GetWorld()->GetAuthGameMode())->Spawn_Character(this);
}
bool APC_Gameplay::ValidatePlayerState()
{
	// Ensures that the player state is Valid before we setup the PlayerController
	// �÷��̾� ��Ʈ�ѷ��� �����ϱ� ����, �÷��̾� ������Ʈ�� ��ȿ���� �����Ѵ�.
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
