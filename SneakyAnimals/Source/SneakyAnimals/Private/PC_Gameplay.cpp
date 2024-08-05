// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Gameplay.h"
#include <SAModeBase.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>

void APC_Gameplay::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ������Ʈ�� ��ȿ���� �˻��ϰ� ��ȿ�� ��쿡�� Setup_PC �Լ��� ȣ��
	if (ValidatePlayerState())
	{
		Setup_PC();
	}
}
void APC_Gameplay::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		// Toggle_Key_Infom
		EnhancedInputComponent->BindAction(Toggle_KeyInform, ETriggerEvent::Completed, this, &APC_Gameplay::Toggle_Key_Inform);
	}
}
void APC_Gameplay::ServerRPC_Spawn_Character_Implementation()
{
	Cast<ASAModeBase>(GetWorld()->GetAuthGameMode())->Spawn_Character(this);
}

void APC_Gameplay::Toggle_Key_Inform()
{


	// Get the pawn and cast it to ATestPlayer
	APawn* pawn = this->GetPawn();
	if (!pawn) {
		return;
	}

	ATestPlayer* testPlayer = Cast<ATestPlayer>(pawn);
	if (!testPlayer) {
		return;
	}

	// ���� �޴��� ����Ʈ�� �ִ�? PC_Base���� �Լ� ������

	// ������, WB_Character_Customization�� valid �ϴ�?
	if (KeyInfom_inst->IsValidLowLevelFast())
	{
		// WB_Character_Customization is valid
		// ���, WB_Character_Customization�� ����Ʈ�� �ִ�?
		if (KeyInfom_inst->IsInViewport())
		{
			// ����Ʈ�� ������
			// WB_Character_Customization Remove From Parent
			KeyInfom_inst->RemoveFromParent();
			// Show Mouse Cursor false
			this->bShowMouseCursor = false;
			// Set Input Mode Game Only
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		}
		else
		{
			// ����Ʈ�� ������
			// Remove WB_Lobby_Menu
			KeyInfom_inst->AddToViewport(0);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, KeyInfom_inst, EMouseLockMode::DoNotLock, false, false);
			// Show Mouse Cursor true
			this->bShowMouseCursor = true;
		}

	}
	else
	{
		// WB_Character_Customization is not valid
		// WB_Character_Customization�� create �ض�
		// MainMenu_inst = CreateWidget<UW_MainMenu>(this, MainMenu_bp, FName("WB_MainMenu"));
		KeyInfom_inst = CreateWidget<UUserWidget>(this, KeyInfom_bp, FName("WBP_KeyInformLobby"));
		// WB_Character_Customization�� ����Ʈ�� ������ �ϴ� �������� �Ȱ��� �����Ѵ�.

		// ����Ʈ�� ������
		// Remove WB_Lobby_Menu
		// WB_Character_Customization Add to Viewport
		KeyInfom_inst->AddToViewport(0);
		// Set Input Mode Game And UI Do Not Lock(�ڵ�� PC_MainMenu.cpp ����)
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, KeyInfom_inst, EMouseLockMode::DoNotLock, false, false);
		// Show Mouse Cursor true
		this->bShowMouseCursor = true;

	}

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
