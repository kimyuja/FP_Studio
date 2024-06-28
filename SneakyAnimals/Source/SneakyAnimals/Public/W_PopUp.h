// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_PopUp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyClickedConfirm);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyClickedDecline);

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_PopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* PopUp_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* WB_Button_Confirm;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* WB_Button_Decline;
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	FText PopUp_Message;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FText Left_Btn_Text;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	FText Right_Btn_Text;
	
	UPROPERTY(EditAnywhere, Category = "MySettings")
	bool bContains_Right_Button;
	
    UFUNCTION()
    void OnWB_Button_ConfirmClicked();
	
	UFUNCTION()
    void OnWB_Button_DeclineClicked();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMyClickedConfirm OnClicked_Confirm;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMyClickedDecline OnClicked_Decline;

protected:
	virtual void NativeConstruct() override;
};
