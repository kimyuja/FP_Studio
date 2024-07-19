// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Lobby_Menu.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Lobby_Menu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Lobby_PlayerList* LobbyPlayerList;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* ReadyUp_Btn;

	UPROPERTY(BlueprintReadWrite, Category="Ready Up Button Image")
	UTexture2D* T_Color6_Hovered;
	UPROPERTY(BlueprintReadWrite, Category="Ready Up Button Image")
	UTexture2D* T_Color6_Pressed;
	UPROPERTY(BlueprintReadWrite, Category="Ready Up Button Image")
	UTexture2D* T_Color5_Normal;
	UPROPERTY(BlueprintReadWrite, Category="Ready Up Button Image")
	UTexture2D* T_Color5_Hovered;
	UPROPERTY(BlueprintReadWrite, Category="Ready Up Button Image")
	UTexture2D* T_Color5_Pressed;

	UFUNCTION()
	void OnReadyUp_BtnClicked();

	void Set_ReadyUp_Button(const FText& Text, bool bWarningState, bool bHideButton, bool bLockButton);
	void Init_ReadyUp_Button();

protected:
	virtual void NativeConstruct() override;

private:
	void InitCasts();
	void Server_Toggle_Launchevent();
	void Client_Toggle_ReadyUp();
	void Init_Server_ReadyUp_Btn();
	void Init_Client_ReadyUp_Btn();

	FSlateBrush CreateSlateBrushFromTexture(UTexture2D* Texture) const;
	FButtonStyle CreateButtonStyle(const FSlateBrush& NormalBrush, const FSlateBrush& HoveredBrush, const FSlateBrush& PressedBrush) const;

	AGS_Lobby* GS_Lobby;
	APS_Lobby* PS_Lobby;
	APC_Lobby* PC_Lobby;
};
