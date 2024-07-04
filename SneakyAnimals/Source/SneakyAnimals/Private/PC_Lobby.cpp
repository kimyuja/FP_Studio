// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "I_LobbyToggleInterface.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "TestPlayer.h"

void APC_Lobby::BeginPlay()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(Lobby_InputMappingContext, 0);
	}
}

void APC_Lobby::SetupInputComponent()
{

	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		// Toggle_CharacterCustomization
		EnhancedInputComponent->BindAction(Toggle_CharacterCustomizationAction, ETriggerEvent::Triggered, this, &APC_Lobby::Toggle_CharacterCustomization);
	}
}

APC_Lobby::APC_Lobby()
{
	//// WB_MainMenu ���� �������Ʈ Ŭ������ �ε��Ѵ�.
	//static ConstructorHelpers::FClassFinder<UW_Character_Customization> WidgetBPClass(TEXT("/Game/KYJ/Widgets/Lobby/WB_Character_Customization.WB_Character_Customization_C"));
	//if (WidgetBPClass.Succeeded())
	//{
	//	Character_Customization_bp = WidgetBPClass.Class;
	//}
}

void APC_Lobby::Toggle_CharacterCustomization()
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

    // Check if the pawn implements the interface
	// �÷��̾����׼� ������ �Լ� ������ �̷� ������ ȣ���ϸ� �ȴ�.
    if (UKismetSystemLibrary::DoesImplementInterface(testPlayer, UI_LobbyToggleInterface::StaticClass())) {
        II_LobbyToggleInterface::Execute_Toggle_CharacterCustomization(testPlayer);
    }

	// ���� �޴��� ����Ʈ�� �ִ�? PC_Base���� �Լ� ������

	// ������, WB_Character_Customization�� valid �ϴ�?

	// WB_Character_Customization is valid
	// ���, WB_Character_Customization�� ����Ʈ�� �ִ�?
	// ����Ʈ�� ������
	// WB_Character_Customization Remove From Parent
	// Show Mouse Cursor false
	// Set Input Mode Game Only


	// ����Ʈ�� ������
	// Remove WB_Lobby_Menu
	// WB_Character_Customization Add to Viewport
	// Set Input Mode Game And UI Do Not Lock(�ڵ�� PC_MainMenu.cpp ����)
	// Show Mouse Cursor true


	// WB_Character_Customization is not valid
	// WB_Character_Customization�� create �ض�
	// WB_Character_Customization�� ����Ʈ�� ������ �ϴ� �������� �Ȱ��� �����Ѵ�.

}
