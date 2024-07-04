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

		//Toggle_CharacterCustomization
		EnhancedInputComponent->BindAction(Toggle_CharacterCustomizationAction, ETriggerEvent::Triggered, this, &APC_Lobby::Toggle_CharacterCustomization);
	}
}

void APC_Lobby::Toggle_CharacterCustomization()
{
    // Get the pawn and cast it to ATestPlayer
    APawn* pawn = this->GetPawn();
    if (!pawn) {
        UE_LOG(LogTemp, Warning, TEXT("Pawn is null"));
        return;
    }

    ATestPlayer* testPlayer = Cast<ATestPlayer>(pawn);
    if (!testPlayer) {
        UE_LOG(LogTemp, Warning, TEXT("Pawn is not ATestPlayer"));
        return;
    }

    // Check if the pawn implements the interface
    if (UKismetSystemLibrary::DoesImplementInterface(testPlayer, UI_LobbyToggleInterface::StaticClass())) {
        II_LobbyToggleInterface::Execute_Toggle_CharacterCustomization(testPlayer);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Pawn does not implement UI_LobbyToggleInterface"));
    }
}
