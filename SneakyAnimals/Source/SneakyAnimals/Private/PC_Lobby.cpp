// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "I_LobbyToggleInterface.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "TestPlayer.h"
#include "W_Character_Customization.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
#include "W_Lobby_HUD.h"
#include "FL_General.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GI_SneakyAnimals.h"

void APC_Lobby::BeginPlay()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(Lobby_InputMappingContext, 0);
	}

	// 부모 비긴 플레이에서는 인풋 모드를 게임 온리로 한다.
	Super::BeginPlay();

	ValidatePlayerState();
	Setup_PC();
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
	// WB_Character_Customization 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_Character_Customization> WidgetBPClass(TEXT("/Game/KYJ/Widgets/Lobby/WB_Character_Customization.WB_Character_Customization_C"));
	if (WidgetBPClass.Succeeded())
	{
		Character_Customization_bp = WidgetBPClass.Class;
	}

	// WB_Lobby_HUD 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_Lobby_HUD> WidgetBPClass2(TEXT("/Game/KYJ/Widgets/Lobby/WB_Lobby_HUD.WB_Lobby_HUD_C"));
	if (WidgetBPClass2.Succeeded())
	{
		Lobby_HUD_bp = WidgetBPClass2.Class;
	}

	// Loading Screen Texture2D 경로 설정
	FSoftObjectPath TextureRef(TEXT("/Game/KYJ/Assets/Widgets/General/T_Color4_Normal.T_Color4_Normal"));

	// Loading Screen Texture2D 로드
	T_Color4_Normal = Cast<UTexture2D>(TextureRef.TryLoad());
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

void APC_Lobby::ClientRPC_Update_LobbyStatusIndicator_Implementation(ESlateVisibility eSlateVisibility, const FText& LobbyStatus_Text)
{
	Update_LobbyStatusIndicator(eSlateVisibility, LobbyStatus_Text);
}

void APC_Lobby::Update_Character_Customization_Widget()
{
	if (Character_Customization_inst->IsValidLowLevelFast())
	{
		Character_Customization_inst->Refresh_Widget();
	}
	return;
}

void APC_Lobby::ClientRPC_Update_ReadyUpButton_Implementation(APC_Lobby* target, const FText& NewText, bool _bWarningState, bool _bHideButton)
{
	
}

void APC_Lobby::Update_LobbyStatusIndicator(ESlateVisibility NewVisibility, const FText& NewStatusText)
{
}

void APC_Lobby::ClientRPC_Update_ServerName_Implementation(const FText& NewServerName)
{
	ServerName = NewServerName;
	Update_ServerName();
}

void APC_Lobby::ClientRPC_KickedFromLobby_Implementation()
{
	UFL_General::Create_LoadingScreen(GetWorld(), T_Color4_Normal, FText::FromString(TEXT("Going back to the menu because you got kicked")));

	FTimerHandle TimerHandle_KickedFromLobby;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_KickedFromLobby, [&]() {
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("StartMap")));
		// 세션을 제거합니다.
		if (UGI_SneakyAnimals* GameInstance = Cast<UGI_SneakyAnimals>(GetGameInstance()))
		{
			// 솔직히 이거 맞는지 모르겠음
			GameInstance->ExitRoom();
		}
		}, 1.5f, false);

}

void APC_Lobby::ValidatePlayerState()
{
	// Ensures that the player state is Valid before we setup the PlayerController
	// 플레이어 컨트롤러를 설정하기 전에, 플레이어 스테이트가 유효한지 보장한다.
	FTimerHandle TimerHandle_PlayerStateCheck;

	if (PlayerState->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is valid!"));
		return;
	} 
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_PlayerStateCheck, [&](){
			APC_Lobby::ValidatePlayerState();
		}, 0.2f, false);
	}
}

void APC_Lobby::Setup_PC()
{
	if (IsLocalPlayerController())
	{
		Super::CleanUp_UI();

		Create_WB_Lobby_HUD();
	}
	
}

void APC_Lobby::Create_WB_Lobby_HUD()
{
	// Lobby_HUD_inst가 잘 생성 됐니?
	if (Lobby_HUD_inst->IsValidLowLevelFast())
	{
		// 뷰 포트에 없으면
		if (!Lobby_HUD_inst->IsInViewport())
		{
			// 뷰 포트에 붙여라
			Lobby_HUD_inst->AddToViewport(0);
		}
	} 
	else
	{
		Lobby_HUD_inst = CreateWidget<UW_Lobby_HUD>(this, Lobby_HUD_bp, FName("WB_Lobby_HUD"));
		Lobby_HUD_inst->AddToViewport(0);
	}

	Update_ServerName();
	return;
}

void APC_Lobby::Update_ServerName()
{
	if (Lobby_HUD_inst->IsValidLowLevelFast())
	{
		Lobby_HUD_inst->Set_ServerName(ServerName);
	}
}

void APC_Lobby::Save_ReadyUpButtonState(const FText& _ReadyUpButtonText, bool _bWarningState, bool _bHideButton)
{
	ReadyUpButton_Text = _ReadyUpButtonText;
	bWarningState = _bWarningState;
	bHideButton = _bHideButton;
	
	return;
}

void APC_Lobby::Update_ReadyUpButton()
{
	if (bReadyUpButton_Init)
	{
		
	} 
	else
	{
	}
}
