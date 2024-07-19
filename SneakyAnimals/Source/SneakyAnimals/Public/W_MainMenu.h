// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_MainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UW_MainMenu(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* HostGame_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* JoinGame_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Profile_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* QuitGame_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Settings_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UVerticalBox* VerticalBox_ButtonContainer;

    UFUNCTION()
    void OnProfile_BtnClicked();
	
	UFUNCTION()
    void OnHostGame_BtnClicked();
	
	UFUNCTION()
    void OnJoinGame_BtnClicked();
	
	UFUNCTION()
    void OnQuitGame_BtnClicked();
	
	UFUNCTION()
    void QuitGame_Btn_Confirm();

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class UW_ProfileMenu> ProfileMenu_bp;
	
	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class UW_HostGameMenu> HostGameMenu_bp;
	
	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class UW_ServerBrowserMenu> JoinGameMenu_bp;

	UPROPERTY()
	class UW_ProfileMenu* ProfileMenu_inst;
	
	UPROPERTY()
	class UW_HostGameMenu* HostGameMenu_inst;
	
	UPROPERTY()
	class UW_ServerBrowserMenu* JoinGameMenu_inst;

	void SetVerticalBoxButtonContainerVisibility(ESlateVisibility InVisibility);

protected:
	virtual void NativeConstruct() override;
};
