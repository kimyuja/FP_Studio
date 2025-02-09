// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataStructure.h"
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

	FStructure_Available_Characters Character;
	FStructure_Available_Accessories Accessories;
	FStructure_Available_Skins Skins;
	FStructure_Available_Eyes Eyes;
	FStructure_Available_Top Top;
	FStructure_Available_Bottom Bottom;
	FStructure_Available_Outer Outer;
	FStructure_Available_Dress Dress;
	

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
