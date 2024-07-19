// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataStructure.h"
#include "W_Lobby_PlayerList_Item.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Lobby_PlayerList_Item : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class USizeBox* HostIndicator;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UButton* Kick_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* KickButton_Text;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* Player_Avatar_Image;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* Player_ReadyStatus;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* Player_Username_text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    FStructure_ConnectedPlayer ConnectedPlayer;

	UFUNCTION()
	void Set_KickButton();

	UFUNCTION()
	void KickPlayer();

protected:
	virtual void NativeConstruct() override;
	
private:
	void Setup();
	void Set_Avatar();
	void Set_Username();
	void Set_HostIndicator();
	void Set_ReadyStatus();

	FSlateColor Ready_Color;
	FSlateColor NotReady_Color;
	
};
