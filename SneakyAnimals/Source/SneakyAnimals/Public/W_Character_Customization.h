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
	class UW_Base_Button* Characters_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Accessoires_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Skins_Btn;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Clothes_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UW_Base_Button* Close_Btn;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UWrapBox* AvailableCharacters_Wrapbox;

	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UWrapBox* AvailableAccessoires_Wrapbox;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UWrapBox* AvailableSkins_Wrapbox;
	
	UPROPERTY(EditAnywhere, Category = "MySettings", meta = (BindWidget))
	class UWrapBox* AvailableClothes_Wrapbox;
	
	
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

	void Refresh_Widget();

protected:
	virtual void NativeConstruct() override;

private:
	int32 SkinsNum;
	int32 TopNum;
	int32 TopPlusBottomNum;
	int32 TopPlusBottomPlusOuterNum;

	class APS_Lobby* PS_Lobby;

	FSlateBrush CreateSlateBrushFromTexture(UTexture2D* Texture) const;
	void Load_Available_Characters();
	void Load_Available_Accessories();
	void Load_Available_Skins();
	void Load_Available_Eyes();
	void Load_Available_Top();
	void Load_Available_Bottom();
	void Load_Available_Outer();
	void Load_Available_Dress();
	void Find_Current_Selected_Character();
	void Find_Current_Selected_Accessories();
	void Find_Current_Selected_Skins();
	void Find_Current_Selected_Eyes();
	void Find_Current_Selected_Top();
	void Find_Current_Selected_Outer();
	void Find_Current_Selected_Bottom();
	void Find_Current_Selected_Dress();


	TObjectPtr<UDataTable> DT_Available_Characters;
	TObjectPtr<UDataTable> DT_Available_Accessories;
	TObjectPtr<UDataTable> DT_Available_Skins;
	TObjectPtr<UDataTable> DT_Available_Eyes;
	TObjectPtr<UDataTable> DT_Available_Top;
	TObjectPtr<UDataTable> DT_Available_Bottom;
	TObjectPtr<UDataTable> DT_Available_Outer;
	TObjectPtr<UDataTable> DT_Available_Dress;
	FStructure_Available_Characters S_Available_Characters;
	FStructure_Available_Accessories S_Available_Accessories;
	FStructure_Available_Skins S_Available_Skins;
	FStructure_Available_Eyes S_Available_Eyes;
	FStructure_Available_Top S_Available_Top;
	FStructure_Available_Bottom S_Available_Bottom;
	FStructure_Available_Outer S_Available_Outer;
	FStructure_Available_Dress S_Available_Dress;

	UFUNCTION()
	void OnCharacters_BtnClicked();

	UFUNCTION()
	void OnAccessoires_BtnClicked();
	
	UFUNCTION()
	void OnSkins_BtnClicked();
	
	UFUNCTION()
	void OnClothes_BtnClicked();

	UFUNCTION()
	void OnClose_BtnClicked();
};
