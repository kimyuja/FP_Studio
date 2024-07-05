// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Character_Customization_Item.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Character_Customization_Item : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* Checkmark_Icon;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* Item_Image;

	struct FStructure_Available_Characters* Character;
	
	//struct FStructure_Available_Accessories* Accessory;

	UPROPERTY()
	FString ItemType;

	class APS_Lobby* PS_Lobby;

	UFUNCTION()
	void OnClickedButton();
	
protected:
	virtual void NativeConstruct() override;

private:
	void Setup();
};
