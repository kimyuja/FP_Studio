// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Lobby_HUD.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Lobby_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UBackgroundBlur* BackgroundBlur;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UCircularThrobber* CircularThrobber;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* LobbyName_Text;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* LobbyStatus_Text;
	
	UPROPERTY(EditAnywhere, Category = "Launch Event")
	int32 CountDown;

	UPROPERTY(EditAnywhere, Category = "Launch Event")
	bool bLaunchingGame;

	UPROPERTY(EditAnywhere, Category = "Launch Event")
	bool bLaunchActive;

	UPROPERTY(EditAnywhere, Category = "References")
	class AGS_Lobby* GS_Lobby;
	
	UPROPERTY(EditAnywhere, Category = "References")
	class APS_Lobby* PS_Lobby;
	
	UPROPERTY(EditAnywhere, Category = "References")
	class APC_Lobby* PC_Lobby;

	UFUNCTION(Category = "Setup Widget")
	void Set_ServerName(const FText& ServerName);

	void Toggle_Launchevent(bool _bLaunchingGame);
	void Countdown_Launchevent();
	void InitCasts();
	void Toggle_BackgroundBlur(bool bEnableBlur);
	void Update_LobbyStatus_Indicator(ESlateVisibility _CircularThrobberVisibility, const FText& _LobbyStatus_Text);

protected:
	virtual void NativeConstruct() override;

private:
	void Setup();
	void InitLobbyStatusIndicator();
	FTimerHandle Timer_Countdown;
	
};
