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

	// HostGame 기능 구현할 때 주석 풀기
    //UFUNCTION()
    //void OnHostGame_BtnClicked();

    UFUNCTION()
    void OnProfile_BtnClicked();

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class UW_ProfileMenu> ProfileMenu_bp;

	UPROPERTY()
	class UW_ProfileMenu* ProfileMenu_inst;

protected:
	virtual void NativeConstruct() override;

private:
	void SetVerticalBoxButtonContainerVisibility(ESlateVisibility InVisibility);
};
