// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataStructure.h"
#include "W_Character_Customization.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UW_Character_Customization : public UUserWidget
{
	GENERATED_BODY()

public:
	UW_Character_Customization(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Accessoires_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Characters_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Close_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UWrapBox* AvailableAccessoires_Wrapbox;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UWrapBox* AvailableCharacters_Wrapbox;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	void Switch_ActiveTab(int32 ActiveIndex);

	void Set_ActiveTab(class UButton* Button_Target);

	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	class UTexture2D* T_Empty;
	
	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	class UTexture2D* T_Color1_Normal;
	
	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	class UTexture2D* T_Color1_Hovered;
	
	UPROPERTY(EditAnywhere, Category = "MyButton Configuration", BlueprintReadWrite)
	class UTexture2D* T_Color1_Pressed;
	
	UPROPERTY()
	TSubclassOf<class UW_Character_Customization_Item> Character_Customization_Item_bp;
	
	UPROPERTY()
	class UW_Character_Customization_Item* Character_Customization_Item_inst;

protected:
	virtual void NativeConstruct() override;

private:
	FSlateBrush CreateSlateBrushFromTexture(UTexture2D* Texture) const;
	void Load_Available_Characters();
	void Find_Current_Selected_Character();


	TObjectPtr<UDataTable> DT_Available_Characters;
	FStructure_Available_Characters* S_Available_Characters;

	UFUNCTION()
	void OnAccessoires_BtnClicked();

	UFUNCTION()
	void OnCharacters_BtnClicked();

	UFUNCTION()
	void OnClose_BtnClicked();
};
