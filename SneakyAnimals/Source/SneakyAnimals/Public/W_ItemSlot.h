// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ItemSlot.generated.h"

/**
 *
 */
UCLASS()
class SNEAKYANIMALS_API UW_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;


public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UButton* itemBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UImage* itemIcon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	//class UTexture2D* itemImg;
	class UTextBlock* itemCost;

	// UPROPERTY(EditAnywhere, meta = (BindWidget))
	UPROPERTY(EditAnywhere)
	class UMapCustomWidget* mapCustomWidget;

	UFUNCTION()
	void OnItemBtnClicked();

	UFUNCTION(BlueprintCallable)
	void SetMapCustomWidget(UMapCustomWidget* Widget);
	
};
