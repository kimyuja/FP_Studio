// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemComponent.h"
#include "W_ItemSlot.h"
#include "MapCustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UMapCustomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

protected:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
    class UW_CustomMap* CustomMapWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	float tileSize;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* maxCost;

public:
	int32 maxCostAsInt(int32 cost) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateMaxCost(int32 newCost);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeItemSlots();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<UW_ItemSlot*> itemSlotWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> itemSlotWidgetClass;

	UW_ItemSlot* itemSlotWidget;

};
