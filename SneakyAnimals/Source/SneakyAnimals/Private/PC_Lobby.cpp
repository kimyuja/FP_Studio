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
	//// WB_MainMenu 위젯 블루프린트 클래스를 로드한다.
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
	// 플레이어한테서 실행할 함수 있으면 이런 식으로 호출하면 된다.
    if (UKismetSystemLibrary::DoesImplementInterface(testPlayer, UI_LobbyToggleInterface::StaticClass())) {
        II_LobbyToggleInterface::Execute_Toggle_CharacterCustomization(testPlayer);
    }

	// 게임 메뉴가 뷰포트에 있니? PC_Base에서 함수 실행해

	// 없으면, WB_Character_Customization가 valid 하니?

	// WB_Character_Customization is valid
	// 라면, WB_Character_Customization가 뷰포트에 있니?
	// 뷰포트에 있으면
	// WB_Character_Customization Remove From Parent
	// Show Mouse Cursor false
	// Set Input Mode Game Only


	// 뷰포트에 없으면
	// Remove WB_Lobby_Menu
	// WB_Character_Customization Add to Viewport
	// Set Input Mode Game And UI Do Not Lock(코드는 PC_MainMenu.cpp 참고)
	// Show Mouse Cursor true


	// WB_Character_Customization is not valid
	// WB_Character_Customization를 create 해라
	// WB_Character_Customization가 뷰포트에 없으면 하는 로직으로 똑같이 실행한다.

}
