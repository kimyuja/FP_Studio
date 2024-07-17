// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemComponent.h"
#include "W_ItemSlot.h"
#include "Input/Reply.h"
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
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override; 

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UW_CustomMap* CustomMapGridWidget;*/

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UNewGridWidget* NewGridWidget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UI")
	float tileSize = 160.f;

	UPROPERTY(meta = (BindWidget))
	class UBorder* BackgroundBorder;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* maxCost;

public:
	int32 DecreaseCurrentCost(int32 cost);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateMaxCost(int32 newCost);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void InitializeItemSlots();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<UW_ItemSlot*> itemSlotWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> itemSlotWidgetClass;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UW_CustomMap> gWidget;*/
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UNewGridWidget> gWidget;

	UW_ItemSlot* itemSlotWidget;
	
	UFUNCTION(BlueprintCallable)
	bool ValidCost();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _currentCost;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* DraggedItem;

	FReply OnBackgroundBorderMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);


};
