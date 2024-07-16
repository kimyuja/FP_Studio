// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "I_LobbyToggleInterface.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "TestPlayer.h"
#include "W_Character_Customization.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>

void APC_Lobby::BeginPlay()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(Lobby_InputMappingContext, 0);
	}

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

void APC_Lobby::SetupInputComponent()
{

	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		// Toggle_CharacterCustomization
		EnhancedInputComponent->BindAction(Toggle_CharacterCustomizationAction, ETriggerEvent::Completed, this, &APC_Lobby::Toggle_CharacterCustomization);
	}
}

APC_Lobby::APC_Lobby()
{
	// WB_MainMenu 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_Character_Customization> WidgetBPClass(TEXT("/Game/KYJ/Widgets/Lobby/WB_Character_Customization.WB_Character_Customization_C"));
	if (WidgetBPClass.Succeeded())
	{
		Character_Customization_bp = WidgetBPClass.Class;
	}
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
	if (Character_Customization_inst->IsValidLowLevelFast())
	{
		// WB_Character_Customization is valid
		// 라면, WB_Character_Customization가 뷰포트에 있니?
		if (Character_Customization_inst->IsInViewport())
		{
			// 뷰포트에 있으면
			// WB_Character_Customization Remove From Parent
			Character_Customization_inst->RemoveFromParent();
			// Show Mouse Cursor false
			this->bShowMouseCursor = false;
			// Set Input Mode Game Only
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		}
		else
		{
			// 뷰포트에 없으면
			// Remove WB_Lobby_Menu
			
			// WB_Character_Customization Add to Viewport
			Character_Customization_inst->AddToViewport(0);
			// Set Input Mode Game And UI Do Not Lock(코드는 PC_MainMenu.cpp 참고)
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, Character_Customization_inst, EMouseLockMode::DoNotLock, false, false);
			// Show Mouse Cursor true
			this->bShowMouseCursor = true;
		}

	}
	else
	{
		// WB_Character_Customization is not valid
		// WB_Character_Customization를 create 해라
		// MainMenu_inst = CreateWidget<UW_MainMenu>(this, MainMenu_bp, FName("WB_MainMenu"));
		Character_Customization_inst = CreateWidget<UW_Character_Customization>(this, Character_Customization_bp, FName("WB_Character_Customization"));
		// WB_Character_Customization가 뷰포트에 없으면 하는 로직으로 똑같이 실행한다.

		// 뷰포트에 없으면
		// Remove WB_Lobby_Menu

		// WB_Character_Customization Add to Viewport
		Character_Customization_inst->AddToViewport(0);
		// Set Input Mode Game And UI Do Not Lock(코드는 PC_MainMenu.cpp 참고)
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, Character_Customization_inst, EMouseLockMode::DoNotLock, false, false);
		// Show Mouse Cursor true
		this->bShowMouseCursor = true;

	}


}

void APC_Lobby::ClientRPC_Update_Character_Customization_Panel_Implementation()
{
	Update_Character_Customization_Widget();
}

void APC_Lobby::Update_Character_Customization_Widget()
{
	if (Character_Customization_inst->IsValidLowLevelFast())
	{
		Character_Customization_inst->Refresh_Widget();
	}
	return;
}
