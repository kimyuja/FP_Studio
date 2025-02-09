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
#include "W_Lobby_Menu.h"
#include "W_Lobby_PlayerList.h"
#include "PS_Lobby.h"
#include "W_PopUp.h"
#include "GM_Base.h"

void APC_Lobby::BeginPlay()
{
	// 부모 비긴 플레이에서는 인풋 모드를 게임 온리로 한다.
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(Lobby_InputMappingContext, 0);
	}

	// 플레이어 스테이트가 유효한지 검사하고 유효한 경우에만 Setup_PC 함수를 호출
	if (ValidatePlayerState())
	{
		Setup_PC();
	}
}

void APC_Lobby::SetupInputComponent()
{

	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

		// Toggle_CharacterCustomization
		EnhancedInputComponent->BindAction(Toggle_CharacterCustomizationAction, ETriggerEvent::Completed, this, &APC_Lobby::Toggle_CharacterCustomization);
		
		// Toggle_WB_Lobby_Menu
		EnhancedInputComponent->BindAction(Toggle_WB_Lobby_Menu_Action, ETriggerEvent::Completed, this, &APC_Lobby::Toggle_WB_Lobby_Menu);
		
		// Create Quit Game Pop Up
		EnhancedInputComponent->BindAction(IA_QuitGamePopup, ETriggerEvent::Completed, this, &APC_Lobby::Create_QuitGame_PopUp);
		
		// Toggle_Key_Infom
		EnhancedInputComponent->BindAction(Toggle_KeyInform, ETriggerEvent::Completed, this, &APC_Lobby::Toggle_Key_Inform);
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
	
	// WB_Lobby_Menu 위젯 블루프린트 클래스를 로드한다.
	static ConstructorHelpers::FClassFinder<UW_Lobby_Menu> WidgetBPClass3(TEXT("/Game/KYJ/Widgets/Lobby/WB_Lobby_Menu.WB_Lobby_Menu_C"));
	if (WidgetBPClass3.Succeeded())
	{
		Lobby_Menu_bp = WidgetBPClass3.Class;
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
			Remove_WB_LobbyMenu();
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

void APC_Lobby::Toggle_WB_Lobby_Menu()
{
	// 없으면, WB_Lobby_Menu가 valid 하니?
	if (Lobby_Menu_inst->IsValidLowLevelFast())
	{
		// WB_Lobby_Menu is valid
		// 라면, Lobby_Menu 가 뷰포트에 있니?
		if (Lobby_Menu_inst->IsInViewport())
		{
			// 뷰포트에 있으면
			// Lobby_Menu Remove From Parent
			Lobby_Menu_inst->RemoveFromParent();
			// Show Mouse Cursor false
			this->bShowMouseCursor = false;
			// Toggle Background Blur
			Lobby_HUD_inst->Toggle_BackgroundBlur(false);
			// Set Input Mode Game Only
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		}
		else
		{
			// 뷰포트에 없으면
			Remove_WB_CharacterCustomization();
			Lobby_Menu_inst->AddToViewport(0);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, Lobby_Menu_inst, EMouseLockMode::DoNotLock, false, false);
			this->bShowMouseCursor = true;
			Lobby_HUD_inst->Toggle_BackgroundBlur(true);
			Update_WB_LobbyMenu();
		}

	}
	else
	{
		// WB_Lobby_Menu is not valid
		// WB_Lobby_Menu를 create 해라
		Lobby_Menu_inst = CreateWidget<UW_Lobby_Menu>(this, Lobby_Menu_bp, FName("WB_Lobby_Menu"));

		// 뷰포트에 없으면
		Remove_WB_CharacterCustomization();
		Lobby_Menu_inst->AddToViewport(0);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, Lobby_Menu_inst, EMouseLockMode::DoNotLock, false, false);
		this->bShowMouseCursor = true;
		Lobby_HUD_inst->Toggle_BackgroundBlur(true);
		Update_WB_LobbyMenu();
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
	Save_ReadyUpButtonState(NewText, _bWarningState, _bHideButton);
	Update_ReadyUpButton();
}

void APC_Lobby::ClientRPC_Launchevent_Implementation(bool _bLaunchingGame)
{
	bLaunchingGame = _bLaunchingGame;
	Update_Launchevent();
}

void APC_Lobby::Update_LobbyStatusIndicator(ESlateVisibility NewVisibility, const FText& NewStatusText)
{
	if (Lobby_HUD_inst->IsValidLowLevelFast())
	{
		Lobby_HUD_inst->Update_LobbyStatus_Indicator(NewVisibility, NewStatusText);
	}
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
			GameInstance->ExitRoom(this->GetPlayerState<APS_Lobby>()->GetPlayerName());
		}
	}, 1.5f, false);
}

void APC_Lobby::ClientRPC_Update_ConnectedPlayersList_Implementation(const TArray<FStructure_ConnectedPlayer>& _ConnectedPlayers)
{
	ConnectedPlayers = _ConnectedPlayers;
	Update_LobbyPlayerList();
}

void APC_Lobby::Update_WB_LobbyMenu()
{
	Update_LobbyPlayerList();
	Update_ReadyUpButton();
	Update_Launchevent();
	return;
}

void APC_Lobby::Update_Launchevent()
{
	Lobby_HUD_inst->Toggle_Launchevent(bLaunchingGame);
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		return;
	} 
	else
	{
		// Lock the client Ready Up button incase we are launching the game so that they can't Unready and spam stop the Host from launching the game.
		// 게임 시작할 때 클라이언트가 레디 버튼 못 누르게 하기 -> 언레디 막아서 방장이 원활하게 게임 시작할 수 있도록 함.
		bLockButton = bLaunchingGame;
		Update_ReadyUpButton();
		return;
	}
}

void APC_Lobby::Create_QuitGame_PopUp()
{

	if (popup->IsValidLowLevelFast() && popup->IsInViewport())
	{
		// 이미 팝업이 있으면 삭제해라
		popup->RemoveFromParent();
		// Show Mouse Cursor false
		this->bShowMouseCursor = false;
		// Set Input Mode Game Only
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		return;
	}
	
	popup = UFL_General::Create_PopUp(GetWorld(), FText::FromString(TEXT("Are you sure to quit the game?")), FText::FromString(TEXT("Quit")), true, FText::FromString(TEXT("Cancel")));

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, popup, EMouseLockMode::DoNotLock, false, false);
	this->bShowMouseCursor = true;
	popup->OnClicked_Confirm.AddDynamic(this, &APC_Lobby::QuitGame_Btn_Confirm);
	popup->OnClicked_Decline.AddDynamic(this, &APC_Lobby::QuitGame_Btn_Decline);

}

void APC_Lobby::QuitGame_Btn_Confirm()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void APC_Lobby::QuitGame_Btn_Decline()
{
	this->bShowMouseCursor = false;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

void APC_Lobby::ServerRPC_Spawn_Character_Implementation()
{
	Cast<AGM_Base>(GetWorld()->GetAuthGameMode())->Spawn_Character(this);
}

void APC_Lobby::Toggle_Key_Inform()
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

	// 게임 메뉴가 뷰포트에 있니? PC_Base에서 함수 실행해

	// 없으면, WB_Character_Customization가 valid 하니?
	if (KeyInfom_inst->IsValidLowLevelFast())
	{
		// WB_Character_Customization is valid
		// 라면, WB_Character_Customization가 뷰포트에 있니?
		if (KeyInfom_inst->IsInViewport())
		{
			// 뷰포트에 있으면
			// WB_Character_Customization Remove From Parent
			KeyInfom_inst->RemoveFromParent();
			// Show Mouse Cursor false
			this->bShowMouseCursor = false;
			// Set Input Mode Game Only
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		}
		else
		{
			// 뷰포트에 없으면
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
		// WB_Character_Customization를 create 해라
		// MainMenu_inst = CreateWidget<UW_MainMenu>(this, MainMenu_bp, FName("WB_MainMenu"));
		KeyInfom_inst = CreateWidget<UUserWidget>(this, KeyInfom_bp, FName("WBP_KeyInformLobby"));
		// WB_Character_Customization가 뷰포트에 없으면 하는 로직으로 똑같이 실행한다.

		// 뷰포트에 없으면
		// Remove WB_Lobby_Menu

		// WB_Character_Customization Add to Viewport
		KeyInfom_inst->AddToViewport(0);
		// Set Input Mode Game And UI Do Not Lock(코드는 PC_MainMenu.cpp 참고)
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, KeyInfom_inst, EMouseLockMode::DoNotLock, false, false);
		// Show Mouse Cursor true
		this->bShowMouseCursor = true;

	}

}

bool APC_Lobby::ValidatePlayerState()
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
			Lobby_HUD_inst->InitCasts();
			Update_ServerName();
			return;
		}
	} 
	else
	{
		Lobby_HUD_inst = CreateWidget<UW_Lobby_HUD>(this, Lobby_HUD_bp, FName("WB_Lobby_HUD"));
		if (Lobby_HUD_inst)
		{
			Lobby_HUD_inst->AddToViewport(0);
			Update_ServerName();
			return;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create Lobby_HUD_inst"));
		}
	}
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
		if (Lobby_Menu_inst->IsValidLowLevelFast())
		{
			Lobby_Menu_inst->Set_ReadyUp_Button(ReadyUpButton_Text, bWarningState, bHideButton, bLockButton);
			return;
		}
	} 
	else
	{
		bReadyUpButton_Init = true;
		if (Lobby_Menu_inst->IsValidLowLevelFast())
		{
			Lobby_Menu_inst->Init_ReadyUp_Button();
			return;
		}
	}
}

void APC_Lobby::Update_LobbyPlayerList()
{
	if (Lobby_Menu_inst)
	{
		// Update Widget
		Lobby_Menu_inst->LobbyPlayerList->Update_Widget(ConnectedPlayers);
		return;
	} 
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APC_Lobby::Update_LobbyPlayerList() Lobby_Menu_inst is invalid!!!!!!"));
		return;
	}
}

void APC_Lobby::Remove_WB_CharacterCustomization()
{
	if (Character_Customization_inst->IsValidLowLevelFast())
	{
		Character_Customization_inst->RemoveFromParent();
		return;
	} 
	else
	{
		return;
	}
}

void APC_Lobby::Remove_WB_LobbyMenu()
{
	if (Lobby_Menu_inst->IsValidLowLevelFast())
	{
		Lobby_Menu_inst->RemoveFromParent();
		Lobby_HUD_inst->Toggle_BackgroundBlur(false);
		return;
	}
	else
	{
		return;
	}
}
