// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PC_Base.h"
#include "PC_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API APC_Lobby : public APC_Base
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;
	APC_Lobby();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* Lobby_InputMappingContext;

	/** Toggle_CharacterCustomization Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Toggle_CharacterCustomizationAction;

	UFUNCTION(BlueprintCallable)
	void Toggle_CharacterCustomization();
	
	/** Toggle_WB_Lobby_Menu Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Toggle_WB_Lobby_Menu_Action;

	UFUNCTION()
	void Toggle_WB_Lobby_Menu();

	UPROPERTY(EditAnywhere, Category="References")
	TSubclassOf<class UW_Character_Customization> Character_Customization_bp;

	UPROPERTY(EditAnywhere, Category = "References")
	class UW_Character_Customization* Character_Customization_inst;

	UFUNCTION(Client, Reliable)
	void ClientRPC_Update_Character_Customization_Panel();

	void Update_Character_Customization_Widget();

	// -----------------위 : 캐릭터 커스터마이징, 아래 : Lobby Ready HUD 관련-----------------

	UFUNCTION(Client, Reliable)
	void ClientRPC_Update_LobbyStatusIndicator(ESlateVisibility _eSlateVisibility, const FText& _LobbyStatus_Text);
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_Update_ReadyUpButton(APC_Lobby* target, const FText& NewText, bool _bWarningState, bool _bHideButton);
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_Launchevent(bool _bLaunchingGame);

	UFUNCTION(Client, Reliable)
	void ClientRPC_Update_ServerName(const FText& NewServerName);

	UFUNCTION(Client, Reliable)
	void ClientRPC_KickedFromLobby();
	
	UFUNCTION(Client, Reliable)
	void ClientRPC_Update_ConnectedPlayersList(const TArray<FStructure_ConnectedPlayer>& _ConnectedPlayers);
	
	void Update_LobbyStatusIndicator(ESlateVisibility NewVisibility, const FText& NewStatusText);

	void Update_WB_LobbyMenu();
	void Update_Launchevent();

	UPROPERTY(EditAnywhere, Category="References")
	TSubclassOf<class UW_Lobby_HUD> Lobby_HUD_bp;

	UPROPERTY(EditAnywhere, Category = "References")
	class UW_Lobby_HUD* Lobby_HUD_inst;
	
	UPROPERTY(EditAnywhere, Category="References")
	TSubclassOf<class UW_Lobby_Menu> Lobby_Menu_bp;

	UPROPERTY(EditAnywhere, Category = "References")
	class UW_Lobby_Menu* Lobby_Menu_inst;

	void Update_LobbyPlayerList();
	// ----------------------------------
	/** Quit Game Pop Up */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_QuitGamePopup;
	UFUNCTION()
	void Create_QuitGame_PopUp();
	UFUNCTION()
	void QuitGame_Btn_Confirm();
	UFUNCTION()
	void QuitGame_Btn_Decline();
	//-----------------------------------

	//----------------Key Inform---------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Toggle_KeyInform;
	
	UPROPERTY(EditAnywhere, Category="References")
	TSubclassOf<class UUserWidget> KeyInfom_bp;

	UPROPERTY(EditAnywhere, Category = "References")
	class UUserWidget* KeyInfom_inst;

	UFUNCTION()
	void Toggle_Key_Inform();
	//-----------------------------------

	UFUNCTION(Server, Reliable)
	void ServerRPC_Spawn_Character();

private:
	bool ValidatePlayerState();
	void Setup_PC();
	void Create_WB_Lobby_HUD();
	void Update_ServerName();

	void Save_ReadyUpButtonState(const FText& _ReadyUpButtonText, bool _bWarningState, bool _bHideButton);
	void Update_ReadyUpButton();


	void Remove_WB_CharacterCustomization();
	void Remove_WB_LobbyMenu();

	// Local Saves
	FText ServerName;

	// Local Saves - Ready Up Button
	FText ReadyUpButton_Text;
	bool bWarningState;
	bool bHideButton;
	bool bReadyUpButton_Init;
	bool bLockButton;

	// Lobby Data - Players
	TArray<FStructure_ConnectedPlayer> ConnectedPlayers;
	 
	// Lobby Data - State
	bool bLaunchingGame;

	UTexture2D* T_Color4_Normal;


};
